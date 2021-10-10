import sys
import urllib.request

args = sys.argv

add_str = args[1]   #通信する相手側のipアドレスを指定する

# URL,保存するファイルのパスを指定
url = "http://" + add_str + ":8080/resultdata.txt" # URLを指定
save_name = "result.txt" #保存したい相対パスを指定

# ダウンロードを実行
urllib.request.urlretrieve(url, save_name)