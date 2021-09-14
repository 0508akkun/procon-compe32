#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum)
    :pieceWH(WH) {
    Array<Array<Color>> edgePixelData;
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
}

Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> Solver::searchConnectPiece(int32 index)  //indexで指定されたピースについて探索
{
    Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> resultList;
    for (int32 i = 0; i < board[index].getEdgePixelData().size(); i++) {    //指定されたピースの中に存在するエッジを全探索
        Array<std::tuple<int32, PieceInfo, PieceInfo>> result;
        for (int32 j = 0; j < board.size(); j++) {  //ピース同士を探索
            if (j == index)break;   //同じピースは探索しない
            for(int32 k = 0; k < board[j].getEdgePixelData().size(); k++){  //他のエッジと比較する時に他のエッジとの差をピース単位で纏めて返してしまうので分解
                result << std::make_tuple(board[j].calcEdgeDiff(board[index].getEdgePixelData()[i])[k], PieceInfo(board[j].getPieceId(), board[j].getEdgeIndex()[k]), PieceInfo(board[index].getPieceId(), board[index].getEdgeIndex()[k])); //指定されたエッジと他のエッジとの差、指定されたエッジと比較したエッジはどのエッジなのか、指定されたエッジと比較したエッジはどのピースに属するのか
            }
        }
        if (!result.isEmpty()) {
            std::sort(result.begin(), result.end());
            result.resize(5);
            resultList << result;
        }
    }
    return resultList;    //戻り値に
}

void Solver::connectPiece()
{
    Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> scoreList;    //各エッジの差を記録しておく配列
    for (int32 i = 0; i < board.size(); i++) {
        Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> result = searchConnectPiece(i);
        for (int32 j = 0; j < result.size(); j++) {
            scoreList << result[j];
        }
    }
    std::sort(scoreList.begin(), scoreList.end());
}

PieceInfo::PieceInfo(int32 pi, int32 ei)
    :pieceId(pi),
     edgeIndex(ei)
{
}
