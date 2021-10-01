#include "Piece.h"

//デフォルトコンストラクタ
PieceInfo::PieceInfo() {

}

PieceInfo::PieceInfo(int32 pi, int32 ei)
    :pieceId(pi),
    edgeIndex(ei)
{
}

Piece::Piece(int32 pi, Array<int32>& ei, Array<Array<Color>>& epd)
    :pieceId(pi),
     edgeIndex(ei)
{
    for (int32 i = 0; i < epd.size(); i++) {
        edgePixelData << epd[i];
    }
    edgeBaseDiff = calcEdgeBaseDiff();
}

Array<std::pair<int32, PieceInfo>> Piece::calcEdgeBaseDiff()   //基準となる色との差を取る事で比較的近い色を持った端の部分が配列の隣同士になりやすくなるはず
{
    Array<std::pair<int32, PieceInfo>> result;
    Array<Color> grayArray(edgePixelData.size(), Color(127, 127, 127));   //何か基準となる色と比較する事で近いであろう端の色を持つピースを割り出す
    for (int32 i = 0; i < edgeIndex.size(); i++){
        int32 pixelDiff = 0;
        for (int32 j = 0; j < grayArray.size(); j++) {
            int32 absValueR = grayArray[j].r - edgePixelData[i][j].r;
            absValueR = Abs(absValueR);
            int32 absValueG = grayArray[j].g - edgePixelData[i][j].g;
            absValueG = Abs(absValueG);
            int32 absValueB = grayArray[i].b - edgePixelData[i][j].b;
            absValueB = Abs(absValueB);
            pixelDiff = absValueR + absValueG + absValueB;
        }
        result << std::make_pair(pixelDiff, PieceInfo(pieceId, edgeIndex[i]));
    }
    return result;
}

int32 Piece::getPieceId()
{
    return pieceId;
}

Array<int32> Piece::getEdgeIndex()
{
    return edgeIndex;
}

Array<Array<Color>> Piece::getEdgePixelData()
{
    return edgePixelData;
}

Array<std::pair<int32, PieceInfo>> Piece::getEdgeBaseDiff()
{
    return edgeBaseDiff;
}

Array<int32> Piece::calcEdgeDiff(Array<Color> ed)   //このピースの４辺の差を返す
{
    Array<int32> result;
    for (int32 i = 0; i < edgePixelData.size(); i++) {
        for (int32 j = 0; j < edgePixelData[i].size(); j++) {
            int32 absValueR = edgePixelData[i][j].r - ed[j].r;
            absValueR = Abs(absValueR);
            int32 absValueG = edgePixelData[i][j].g - ed[j].g;
            absValueG = Abs(absValueG);
            int32 absValueB = edgePixelData[i][j].b - ed[j].b;
            absValueB = Abs(absValueB);
            result << absValueR + absValueG + absValueB;
        }
    }
    return result;
}

