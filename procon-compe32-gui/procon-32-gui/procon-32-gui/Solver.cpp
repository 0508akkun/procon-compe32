#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 hdn, int32 vdn)
    :pieceWH(WH),
     horDivNum(hdn),
     verDivNum(vdn)
    {
    Array<Color> edgePixel;
    //端の画素データを取得
    for (int32 y = 0; y < image.height(); y++) {    //横に探索していく
        if (y % pieceWH == 0) {   //見ている場所が端にある時は
            for (int32 x = 0; x < image.width(); x++) { //配列に加える
                edgePixel << image[y][x];
                if (x % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
        if (y % pieceWH == pieceWH - 1) {   //見ている場所が端にある時は
            for (int32 x = image.width() - 1; x >= 0; x--) { //配列に加える
                edgePixel << image[y][x];
                if (x % pieceWH == 0) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 x = 0; x < image.width(); x++) {    //縦に探索していく
        if (x % pieceWH == 0) {   //見ている場所が端にある時は
            for (int32 y = image.height() - 1; y >= 0; y--) { //配列に加える
                edgePixel << image[y][x];
                if (y % pieceWH == 0) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
        if (x % pieceWH == pieceWH - 1) {   //見ている場所が端にある時は
            for (int32 y = 0; y < image.height(); y++) { //配列に加える
                edgePixel << image[y][x];
                if (y % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 y = 0; y < verDivNum; y++) { //それぞれの端のIndexを設定
        for (int32 x = 0; x < horDivNum; x++) {
            Array<int32> edgeIndex;
            edgeIndex << y * horDivNum + x + horDivNum * y;
            edgeIndex << y * horDivNum + horDivNum - x - 1 + horDivNum + horDivNum * y;
            edgeIndex << horDivNum * verDivNum * 2 + verDivNum - y - 1 + verDivNum * x * 2;
            edgeIndex << horDivNum * verDivNum * 2 + y + verDivNum * x * 2 + verDivNum;
            Array<Array<Color>> epd = { edgePixelData[edgeIndex[0]], edgePixelData[edgeIndex[1]], edgePixelData[edgeIndex[2]], edgePixelData[edgeIndex[3]] };
            //Print << y * horDivNum + x << U" " << edgeIndex[0] << U" " << edgeIndex[1] << U" " << edgeIndex[2] << U" " << edgeIndex[3];
            board << Piece(y * horDivNum + x, edgeIndex, epd);
        }
    }
    graphMemo = Array<int32>(board.size(), -1);
    resultArray = Array<Array<std::pair<int32, int32>>>(verDivNum, Array<std::pair<int32, int32>>(horDivNum, std::make_pair(0, 0)));
}

//sort比較用の関数
bool compare_edge(std::pair<int32, PieceInfo>& edge1, std::pair<int32, PieceInfo>& edge2)
{
    return edge1.first < edge2.first;
}

bool compare_connectList(const std::tuple<int32, PieceInfo, PieceInfo>& list1, const std::tuple<int32, PieceInfo, PieceInfo>& list2)
{
    return std::get<0>(list1) < std::get<0>(list2);
}

bool compare_scoreList(const std::tuple<int32, PieceInfo, PieceInfo>& list1, const std::tuple<int32, PieceInfo, PieceInfo>& list2)
{
    return std::get<0>(list1) < std::get<0>(list2);
}

Array<std::pair<Array<Color>, PieceInfo>> Solver::getBaseDataList()
{
    Array<std::pair<Array<Color>, PieceInfo>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<Array<Color>, PieceInfo>> edgeBaseDiffList = board[i].getEdgeBaseData();   //ピースそれぞれの基本色との差
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    return edgeBaseDiff;
}

Array<Array<std::pair<int32, int32>>> Solver::solveImage()
{
    //ピースをグループに分ける
    Array<std::pair<Array<Color>, PieceInfo>> baseDataList = getBaseDataList();
    Array<std::tuple<int32, PieceInfo, PieceInfo>> connectList;
    Array<std::tuple<int32, PieceInfo, PieceInfo>> resultList;
    for (int32 i = 0; i < baseDataList.size(); i++) {
        resultList = searchConnectPiece(baseDataList, baseDataList[i]);
        for (int32 j = 0; j < resultList.size(); j++) {
            connectList << resultList[j];
        }
    }
    std::sort(connectList.begin(), connectList.end(), compare_connectList);
    for (int32 i = 50; i < 75; i++) {
        Print << std::get<0>(connectList[i]) << U" " << std::get<1>(connectList[i]).pieceId << U" " << std::get<1>(connectList[i]).edgeIndex << U" " << std::get<2>(connectList[i]).pieceId << U" " << std::get<2>(connectList[i]).edgeIndex;
    }
    //ピースを探索する
    int32 x = horDivNum / 2;
    int32 y = verDivNum / 2;
    resultArray[y][x] = std::make_pair(0, 0);
    dfs(connectList, 0, graphMemo, resultArray, x, y, 0);
    /*for (int32 i = 0; i < resultData.size(); i++) {
        Print << std::get<0>(resultData[i]) << U" " << std::get<1>(resultData[i]) << U" " << std::get<2>(resultData[i]) << U" " << std::get<3>(resultData[i]);
    }*/
    calcResultData(resultData, resultArray);
    return resultArray;    //戻り値に
}

void Solver::dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 pi, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32 x, int32 y, int32 rt)
{
    if (gm[pi] == 1) return; //探索先は探索済み
    gm[pi] = 1;    //現在居る場所を探索済みに
    rt %= 4;    //回転情報を0～3の範囲へ
    setResultData(pi, x, y, rt, resultData);
    for (int32 i = 0; i < cl.size(); i++) {
        if (pi == std::get<1>(cl[i]).pieceId) { //目的のpieceId以外は探索しない
            for (int32 j = 0; j < cl.size(); j++) {
                int32 sx = x, sy = y;
                if (std::get<0>(cl[i]) == std::get<0>(cl[j])) {
                    if (std::get<1>(cl[i]).edgeIndex == std::get<2>(cl[j]).edgeIndex && std::get<2>(cl[i]).edgeIndex == std::get<1>(cl[j]).edgeIndex) { //互いに同じ端を参照したら
                        if (gm[std::get<2>(cl[i]).pieceId] == 1)continue;
                        rt += rotateConnectPiece(std::get<1>(cl[i]).edgeIndex);
                        rt %= 4;
                        if (rt == 0) {
                            sy--;
                            rt += 2;
                        }
                        else if (rt == 2) {
                            sy++;
                            rt += 2;
                        }
                        else if (rt == 3) {
                            sx--;
                        }
                        else {
                            sx++;
                        }
                        rt += rotateConnectPiece(std::get<2>(cl[i]).edgeIndex);
                        dfs(cl, std::get<2>(cl[i]).pieceId, gm, ra, sx, sy, rt);
                        break;
                    }
                }
            }
        }
    }
    return;
}

void Solver::setResultData(int32 pieceId, int32 x, int32 y, int32 rt, Array<std::tuple<int32, int32, int32, int32>>& rd)
{
    rd << std::make_tuple(x, y, pieceId, rt);
}

void Solver::calcResultData(Array<std::tuple<int32, int32, int32, int32>> rd, Array<Array<std::pair<int32, int32>>>& ra)
{
    Array<Point> coord;
    std::sort(rd.begin(), rd.end());
    for (int32 i = 0; i < rd.size(); i++) {
        int32 x = std::get<0>(rd[i]) - std::get<0>(rd[0]);
        int32 y = std::get<1>(rd[i]) - std::get<1>(rd[0]);
        coord << Point(x, y);
    }
    for (int32 i = 0; i < ra.size(); i++) {
        for (int32 j = 0; j < ra[i].size(); j++) {
            for (int32 k = 0; k < rd.size(); k++) {
                if (coord[k].y >= 0 && coord[k].y < verDivNum && coord[k].x >= 0 && coord[k].x < horDivNum) {
                    ra[coord[k].y][coord[k].x] = std::make_pair(std::get<2>(rd[k]), std::get<3>(rd[k]));
                }
            }
        }
    }
}

int32 Solver::rotateConnectPiece(int32 index)   //edgeIndexからどの方向を見てるのか推測する
{
    if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //比較前のエッジ番号が初期で上側の場合
        return 0;
    }
    else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //比較前のエッジ番号が初期で下側の場合
        return 2;
    }
    else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //比較前のエッジ番号が初期で左側の場合
        return 3;
    }
    else {
        return 1;
    }
}

Array<std::tuple<int32, PieceInfo, PieceInfo>> Solver::searchConnectPiece(Array<std::pair<Array<Color>, PieceInfo>>& cl, std::pair<Array<Color>, PieceInfo> bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    Array<std::tuple<int32, PieceInfo, PieceInfo>> resultList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second.pieceId != bl.second.pieceId) {    //同じピースでの比較は無視
            int32 absRevValue = 0;
            for (int32 j = 0; j < cl[i].first.size(); j++) {
                //なぜか画素の値の絶対値が取れないので代入し直す
                Array<Color> rEdgePixelData = cl[i].first.reversed();
                int32 absValueR = rEdgePixelData[j].r - bl.first[j].r;
                absValueR = Abs(absValueR);
                int32 absValueG = rEdgePixelData[j].g - bl.first[j].g;
                absValueG = Abs(absValueG);
                int32 absValueB = rEdgePixelData[j].b - bl.first[j].b;
                absValueB = Abs(absValueB);
                absRevValue += absValueR + absValueG + absValueB;
            }
            scoreList << std::make_tuple(absRevValue, PieceInfo(bl.second.pieceId, bl.second.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
        }
    }
    if (!scoreList.empty()) {
        std::sort(scoreList.begin(), scoreList.end(), compare_scoreList);
        for (int32 i = 0; i < scoreList.size(); i++) {
            if (std::get<0>(scoreList[i]) == std::get<0>(scoreList[0])) {
                resultList << scoreList[i];
            }
        }
    }
    return resultList;
}

