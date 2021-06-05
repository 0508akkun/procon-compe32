import numpy as np

def OverField(tiled, y, x):
    if y < 0 or len(tiled) - 1 < y:
        return True
    if x < 0 or len(tiled[0]) - 1 < x:
        return True

    return False

def AreaSearch(tiled, y, x, index, notSearchedNum = 0):
    dx = [1, -1, 0, 0]
    dy = [0, 0, 1, -1]
    tiled[y][x] = index
    for i in range(4):
        if not OverField(tiled, y + dy[i], x+dx[i]):
            if tiled[y+dy[i]][x+dx[i]] == notSearchedNum:
                tiled[y+dy[i]][x+dx[i]] = index
                AreaSearch(tiled, y + dy[i], x + dx[i], index, notSearchedNum)

# TODO
# コード最適化の余地あり
def AreaCalculate(points, tiled):
    index = 2
    for y in range(len(tiled)):
        for x in range(len(tiled[0])):
            if tiled[y][x] == 0:
                AreaSearch(tiled, y, x, index)

    for y in range(len(points)):
        if tiled[y][0] != 1 and tiled[y][0] != 0:
            AreaSearch(tiled, y, 0, 0, tiled[y][0])
        if tiled[y][len(tiled[0]) - 1] != 1 and tiled[y][len(tiled[0]) - 1] != 0:
            AreaSearch(tiled, y, len(tiled[0]) - 1, 0, tiled[y][len(tiled[0]) - 1])

    for x in range(len(points[0])):
        if tiled[0][x] != 1 and tiled[0][x] != 0:
            AreaSearch(tiled, 0, x, 0, tiled[0][x])
        if tiled[len(tiled) - 1][x] != 1 and tiled[len(tiled) - 1][x] != 0:
            AreaSearch(tiled, len(tiled) - 1, x, 0, tiled[len(tiled) - 1][x])

    # 囲われているところ = 数字が 2 タイル
    res = 0
    for y in range(len(points)):
        for x in range(len(points[0])):
            if tiled[y][x] == 2:
                res += abs(points[y][x])

    return res


def TileCalclulate(points, tiled):
    return np.sum(points*tiled) 

def ScoreCalculate(points, tiled):
    return TileCalclulate(points, tiled) + AreaCalculate(points, tiled)