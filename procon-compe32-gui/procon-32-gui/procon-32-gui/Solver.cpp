#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum)
    :pieceWH(WH) {
    Array<Color> edgePixel;
    for (int32 y = 0; y < image.height(); y++) {    //横に探索していく
        if (y % pieceWH == 0 || y % pieceWH == pieceWH - 1) {   //見ている場所が端にある時は
            for (int32 x = 0; x < image.width(); x++) { //配列に加える
                edgePixel << image[y][x];
                if (x % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 x = 0; x < image.height(); x++) {    //縦に探索していく
        if (x % pieceWH == 0 || x % pieceWH == pieceWH - 1) {   //見ている場所が端にある時は
            for (int32 y = 0; y < image.width(); y++) { //配列に加える
                edgePixel << image[y][x];
                if (y % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    Array<int> edgeIndex;
    for (int y = 0; y < verDivNum; y++) {
        for (int x = 0; x < horDivNum; x++) {
            edgeIndex << y * horDivNum + x + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum + verDivNum;
        }
        pieceEdgeIndex << edgeIndex;
        edgeIndex.clear();
    }
}


Array<std::pair<int32, int32>> Solver::calcEdgeDiff()   //基準となる色との差を取る事で比較的近い色を持った端の部分が配列の隣同士になりやすくなるはず
{
    Array<std::pair<int32, int32>> calcArray;
    Array<Array<Color>> grayArray(edgePixelData.size(), Array<Color>(edgePixelData[0].size(), Color(127, 127, 127)));   //何か基準となる色と比較する事で近いであろう端の色を持つピースを割り出す
    for (int32 i = 0; i < grayArray.size(); i++) {
        for (int32 j = 0; j < grayArray[i].size(); j++) {   //下の式は基準の色との差なので絶対値はいらない？(要検証)
            calcArray << std::make_pair(grayArray[i][j].r - edgePixelData[i][j].r + grayArray[i][j].g - edgePixelData[i][j].g + grayArray[i][j].b - edgePixelData[i][j].b, i);
        }
    }
    calcArray.sort();
    return calcArray;
}

int32 Solver::searchConnectPiece(int32 index)
{
    int32 candidateNum = 5; //探しているインデックスの前後5つのデータを候補に
    int32 minPixelDiff = 10000000000000000000;
    int32 pixelDiff = 0;
    int32 minIndex = 0;
    Array<std::pair<int32, int32>> calcArray = calcEdgeDiff();
    Array<std::pair<int32, int32>> candiList;
    for (int i = 0; i < calcArray.size(); i++) {
        if (calcArray[i].second == index) {
            if (i < candidateNum) { //前の猶予がcandiNum分無かったら最初からの部分で取り出す
                candiList = Array<std::pair<int32, int32>>(calcArray.begin(), calcArray.begin() + index + candidateNum);
            }
            else if (calcArray.size() - i - 1 < candidateNum) { //後ろの猶予がcandiNum分無かったら最後の部分まで取り出す
                candiList = Array<std::pair<int32, int32>>(calcArray.begin() + index - candidateNum, calcArray.end());
            }
            else {
                candiList = Array<std::pair<int32, int32>>(calcArray.begin() + index - candidateNum, calcArray.begin() + index + candidateNum);
            }
            break;
        }
    }
    // indexで指定された端を含むピースの端のindex全てを取得
    Array<Array<int32>> pieceRemoveIndex = pieceEdgeIndex.keep_if([index](Array<int32> inList) {
        for (int32 i = 0; i < inList.size(); i++) {
            if (index == inList[i]) {
                return true;
            }
        }
    });
    for (int i = 0; i < candiList.size(); i++) {
        for (int32 j = 0; j < pieceWH; j++) {
            pixelDiff = Abs(edgePixelData[candiList[i].second][j].r - edgePixelData[index][j].r) + Abs(edgePixelData[candiList[i].second][j].g - edgePixelData[index][j].g) + Abs(edgePixelData[candiList[i].second][j].b - edgePixelData[index][j].b);
            if (pixelDiff < minPixelDiff) {
                for (int32 k = 0; k < pieceRemoveIndex[0].size(); k++) {
                    if (pieceRemoveIndex[0][k] != candiList[i].second) {  // indexで指定された端を含むピースの端を除外
                        minPixelDiff = pixelDiff;
                        minIndex = candiList[i].second;
                    }
                }
            }
        }
        Array<Color> revEdgePixelData = edgePixelData[candiList[i].second].reversed();  //反対向きにして一致するか見る
        for (int32 j = 0; j < pieceWH; j++) {
            pixelDiff = Abs(revEdgePixelData[j].r - edgePixelData[index][j].r) + Abs(revEdgePixelData[j].g - edgePixelData[index][j].g) + Abs(revEdgePixelData[j].b - edgePixelData[index][j].b);
            if (pixelDiff < minPixelDiff) {
                for (int32 k = 0; k < pieceRemoveIndex[0].size(); k++) {
                    if (pieceRemoveIndex[0][k] != candiList[i].second) {  // indexで指定された端を含むピースの端を除外
                        minPixelDiff = pixelDiff;
                        minIndex = candiList[i].second;
                    }
                }
            }
        }
    }
    return minIndex;
}
