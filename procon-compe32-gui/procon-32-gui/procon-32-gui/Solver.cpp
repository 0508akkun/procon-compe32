#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 hdn, int32 vdn)
    :pieceWH(WH),
     horDivNum(hdn),
     verDivNum(vdn)
    {
    Array<Color> edgePixel;
    //端の画素データを取得
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
    for (int32 y = 0; y < verDivNum; y++) { //それぞれの端のIndexを設定
        for (int32 x = 0; x < horDivNum; x++) {
            Array<int32> edgeIndex;
            edgeIndex << y * horDivNum + x + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum + verDivNum;
            board << Piece(y * horDivNum + x, edgeIndex, Array<Array<Color>> {edgePixelData[edgeIndex[0]], edgePixelData[edgeIndex[1]], edgePixelData[edgeIndex[2]], edgePixelData[edgeIndex[3]]});
        }
    }
    graph = Array<Array<int32>>(board.size());  //エッジ分の領域を取る
    graphMemo = Array<int32>(board.size(), -1);
    prevPath = Array<int32>(Min(vdn, hdn), -1);
    resultArray = Array<Array<int32>>(verDivNum, Array<int32>(horDivNum, -1));
}

Array<std::pair<int32, PieceInfo>> Solver::getBaseDiffList()
{
    Array<std::pair<int32, PieceInfo>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<int32, PieceInfo>> edgeBaseDiffList = board[i].getEdgeBaseDiff();
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    std::sort(edgeBaseDiff.begin(), edgeBaseDiff.end());
    return edgeBaseDiff;
}

Array<std::pair<int32, int32>> Solver::classifyPiece()  //ピースをグループに分ける
{
    Array<std::pair<int32, PieceInfo>> baseDiffList = getBaseDiffList();
    Array<std::tuple<int32, PieceInfo, PieceInfo>> connectList;
    for (int32 i = 0; i < baseDiffList.size(); i++) {
        Array<std::pair<int32, PieceInfo>> candiList;
        if (i < 5) {
            std::copy(baseDiffList.begin(), baseDiffList.begin() + i + 5, candiList.begin());
        }
        else if (i > baseDiffList.size() - 6) {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.end(), candiList.begin());
        }
        else {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.begin() + i + 5, baseDiffList.begin());
        }
        connectList << searchConnectPiece(candiList, baseDiffList[i].second);
    }
    std::sort(connectList.begin(), connectList.end());
    connectList.resize(connectList.size() - (horDivNum * 2 + verDivNum * 2));
    connectPiece(connectList);
    return resultList;    //戻り値に
}

std::tuple<int32, PieceInfo, PieceInfo> Solver::searchConnectPiece(Array<std::pair<int32, PieceInfo>> cl, PieceInfo bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second.pieceId == bl.pieceId)break;    //同じピースでの比較は無視
        for (int32 j = 0; j < edgePixelData[0].size(); j++) {
            scoreList << std::make_tuple(Abs(edgePixelData[cl[i].second.edgeIndex][j].r - edgePixelData[bl.edgeIndex][j].r) + Abs(edgePixelData[cl[i].second.edgeIndex][j].g - edgePixelData[bl.edgeIndex][j].g) + Abs(edgePixelData[cl[i].second.edgeIndex][j].b - edgePixelData[bl.edgeIndex][j].b), PieceInfo(bl.pieceId, bl.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
            Array<Color> rEdgePixelData = edgePixelData[cl[i].second.edgeIndex].reversed();
            scoreList << std::make_tuple(Abs(rEdgePixelData[j].r - edgePixelData[bl.edgeIndex][j].r) + Abs(rEdgePixelData[j].g - edgePixelData[bl.edgeIndex][j].g) + Abs(rEdgePixelData[j].b - edgePixelData[bl.edgeIndex][j].b), PieceInfo(bl.pieceId, bl.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
        }
    }
    std::sort(scoreList.begin(), scoreList.end());
    return scoreList[0];
}

void Solver::connectPiece(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl)
{
    int32 cornerIndex = -1; //隅に存在するピースのインデックスを記録する
    int32 secondCornerIndex = -1;
    int32 subCornerIndex = -1;  //隅に存在するピースが見つからない時は1つのピースが連結している時に限り隅のインデックスに認定
    for (int32 i = 0; i < cl.size(); i++) {
        graph[std::get<1>(cl[i]).pieceId] << std::get<2>(cl[i]).pieceId;
    }
    for (int32 i = 0; i < graph.size(); i++) {
        if (graph[i].size() == 2) {
            cornerIndex = i;
            break;
        }
        if (graph[i].size() == 1) {
            subCornerIndex = i;
        }
    }
    if (cornerIndex != -1) {
        secondCornerIndex = cornerDfs(cornerIndex, 0, graphMemo, prevPath);
    }
    else if(subCornerIndex != -1) {
        secondCornerIndex = cornerDfs(subCornerIndex, 0, graphMemo, prevPath);
    }
    else {
        secondCornerIndex = -1;
        Print << U"正常にコーナーが検出できていません";
    }
    graphMemo = Array<int32>(board.size(), -1);
    for (int32 i = 0; i < prevPath.size(); i++) {
        graphMemo[prevPath[i]] = 1;
    }
    if (horDivNum <= verDivNum) {
        for (int32 i = 1; i < horDivNum; i++) { //起点となる辺のインデックスを確定させる
            resultArray[0][i] = prevPath[i];
        }
        for (int32 i = 0; i < verDivNum; i++) {
            Array<int32> searchArray(horDivNum, -1);
            for (int32 j = 1; j < horDivNum; j++) {
                searchArray = indexDfs(resultArray[i][j], 0, graphMemo, prevPath, searchArray);
                resultArray[i][j] = searchArray[j];
            }
        }
    }
    else {
        for (int32 i = 0; i < verDivNum; i++) {
            resultArray[i][0] = prevPath[i];
        }
        for (int32 i = 0; i < horDivNum; i++) {
            Array<int32> searchArray(verDivNum, -1);
            for (int32 j = 1; j < verDivNum; j++) {
                searchArray = indexDfs(resultArray[j][i], 0, graphMemo, prevPath, searchArray);
                resultArray[j][i] = searchArray[j];
            }
        }
    }
}

int32 Solver::cornerDfs(int32 index, int32 depth, Array<int32> &gm, Array<int32> &pp)   //画像全体の短い辺を構成するピースを探索する
{
    int32 cornerIndex = -1;
    int32 returnValue = -1;
    if (gm[index] != -1)return;  //検索済みならば無視
    if (depth == Min(horDivNum, verDivNum)) {   //最短のピースの分割数より深さが深くなった後にもう一つの隅は見つかっても隣り合う隅では無いのでその前に探索を終了
        return -1;
    }
    gm[index] = 1;   //探索済みにする
    pp[depth] = index;    //どのような経路を辿ったか保存
    if (graph[index].size() < 2 && depth == Min(horDivNum, verDivNum) - 1) {  //隅を見つけた場合、インデックスを戻り値に
        return index;
    }
    for (int32 i = 0; i < graph[index].size(); i++) {
        returnValue = cornerDfs(graph[index][i], depth++, gm, pp);  //隅のインデックスを見つける
        if (returnValue > -1) { //見つけられたら
            cornerIndex = returnValue;  //その隅を2つ目の隅と認定
            return cornerIndex; //その値を返却する
        }
    }
    return cornerIndex; //答えを返却する
}

Array<int32> Solver::indexDfs(int32 index, int32 depth, Array<int32>& gm, Array<int32>& pp, Array<int32>& sa)    //インデックスがどこにあるのか判定する
{
    Array<int32> result;
    if (gm[index] != -1)return sa;
    if (depth == Min(horDivNum, verDivNum) + 1)return sa;
    gm[index] = 1;  //探索済みにする
    for (int32 i = 0; i < graph[index].size(); i++) {
        if (pp[depth - 1] == graph[index][i]) {
            sa[depth - 1] = index;  // resultArrayに結果を記録する
            result = indexDfs(graph[index][i], depth++, gm, pp, sa);
            return result;
        }
    }
    return result;
}

PieceInfo::PieceInfo(int32 pi, int32 ei)
    :pieceId(pi),
     edgeIndex(ei)
{
}
