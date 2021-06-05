# -*- coding: utf_8 -*-
from datetime import datetime, timedelta
import sched
import time
import json
import threading
import numpy as np
import colorama
from colorama import Fore, Back, Style
from argparse import ArgumentParser
from AccessServer import getMatches, getStatus, postActions, runPing
import solver

def get_option():
    argparser = ArgumentParser()
    argparser.add_argument('-d', '--debug', action='store_true',
                            help='内部データを詳細に出力します')
    argparser.add_argument('-m', '--matchid', type=int, default=1,
                            help='matchIDを指定します')
    argparser.add_argument('-p', '--ping', action='store_true',
                            help='動作確認用APIを実行します')
    return argparser.parse_args()

url = 'http://localhost:8081/matches/'
# ボタンの追加
check = '[' + Fore.MAGENTA + 'v' + Fore.RESET + '] '
success = '[' + Fore.BLUE + 'o' + Fore.RESET + '] '
failure = '[' + Fore.RED + 'x' + Fore.RESET + '] '
disp = '[' + Fore.GREEN + '*' + Fore.RESET + '] '


def parseStatus(status):
    points = status['points']
    tiled = status['tiled']
    allyID = status['teams'][0]['teamID']
    enemyID = status['teams'][1]['teamID']

    return points, tiled, allyID, enemyID

def normalizeTiled(tiled, allyID, enemyID):
    # 正規化
    allyTiled = np.array(tiled)
    enemyTiled = np.array(tiled)
    allyTiled[allyTiled == enemyID] = 0
    allyTiled = allyTiled / allyID
    enemyTiled[enemyTiled == allyID] = 0
    enemyTiled = enemyTiled / enemyID

    return allyTiled, enemyTiled

def checkStatus(status_code, status, matchID = -1):
    if matchID == -1:  # もしmatchIDが指定されていなかったら
        matchID = ''
    else:
        matchID = '  (matchID: ' + str(matchID) + ')'

    if status_code == 200 or status_code == 201:
        print(success + str(status_code) + ' Success!' + matchID, flush=True)
        return True
    elif status_code == 400 and status == 'InvalidMatches':
        print(failure + str(status_code) + ' InvalidMatches' + matchID, flush=True)
        print('    - 参加していない試合へのリクエスト', flush=True)
        return False
    elif status_code == 400 and status == 'TooEarly':
        print(failure + str(status_code) + ' TooEarly' + matchID, flush=True)
        print('    - 試合の開始前にアクセス')
        return False
    elif status_code == 401:
        print(failure + str(status_code) + ' InvalidToken' + matchID, flush=True)
        print('    - トークンが間違っているもしくは存在しない', flush=True)
        return False


def battle(match, debug, error_count=0):
    matchID = match['id']

    # 試合の開始時間を得る(試合直前のカウントダウン中)
    print(check + 'GET: ' + url + str(matchID), flush=True)
    status_code, status = getStatus(id=matchID)
    result = checkStatus(status_code, status, matchID)
    if debug == True:
        print('\n' + disp + 'DISPLAY: status', flush=True)
        print(status, end='\n\n', flush=True)
    
    if result == False:
        # 早すぎたら時間をおいてリスタート
        # 普通あってはならない
        if status == 'TooEarly':
            if error_count >= 5:  # エラーを起こしすぎたら
                print(failure + 'Too Early So Please Retry(matchID: {0})'.format(matchID))
                return
            error_count += 1
            battle(match, debug, error_count)
            return

    for _ in range(match['turns']):
        points, tiled, allyID, enemyID  = parseStatus(status)
        allyTiled, enemyTiled = normalizeTiled(tiled, allyID, enemyID)
        # 制限時間の取得: match['turnMillis']
        # ターンとターンの間の時間: match['intervalMillis']

        # ソルバに解かせる
        print(check + 'RUN: solve()', flush=True)
        start = time.time()
        actions = solver.solver(points, allyTiled, enemyTiled)
        elapsed_time = time.time() - start
        print(check + 'solved! (matchID: {0}) (time: {1} [sec])'.format(matchID, elapsed_time), flush=True)
        if debug == True:
            print('\n' + disp + 'DISPLAY: actions', flush=True)
            print(json.dumps(actions, indent=2), end='\n\n', flush=True)

        # 推論結果をサーバにPOST(送信)
        print(check + 'POST: ' + url + str(matchID), flush=True)
        status_code, status = postActions(matchID, actions)
        checkStatus(status_code, status, matchID)

        # 再び試合状態取得APIを実行
        print(check + 'GET: ' + url + str(matchID), flush=True)
        status_code, status = getStatus(id=matchID)
        result = checkStatus(status_code, status, matchID)
        if debug == True:
            print('\n' + disp + 'DISPLAY: status', flush=True)
            print(status, end='\n\n', flush=True)

def main():
    args = get_option()
    debug = args.debug

    if args.ping == True:
        print(check + 'GET: http://localhost:8081/ping', flush=True)
        status_code, status = runPing()
        checkStatus(status_code, status)
        return


    # 試合前にこれから自分が戦う試合の一覧を得る
    print(check + 'GET: ' + url, flush=True)
    data = None
    status_code, status = getMatches(data=data)
    result = checkStatus(status_code, status)
    if result == False:
        return
    if debug == True:
        print('\n' + disp + 'DISPLAY: matches')
        print(json.dumps(status, indent=2), end='\n\n', flush=True)

    matches = status  # result == True ならstatusには試合の情報が格納されている
    for match in matches:
        thread = threading.Thread(target=battle, args=(match, debug))
        thread.start()
    

if __name__ == "__main__":
    main()