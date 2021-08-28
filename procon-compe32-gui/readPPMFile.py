import cv2
import numpy as np

filename = 'procon-compe32-gui/images/windmill.ppm'

# 画像ファイルパスから読み込み
img = cv2.imread(filename)

# バイナリから読み込み(python3なのでbinaryモードで読み込み)
with open(filename, 'rb') as f:
    binary = f.read()

# 一度ndarrayに変換してからdecodeします。reshapeだけしてると思われます.
arr = np.asarray(bytearray(binary), dtype=np.uint8)
img = cv2.imdecode(arr, -1)  # 'load it as it is'

# numpy配列の取得、インスタンス自体がndarrayです
type(img) # numpy.ndarray

cv2.imshow("img", img)