#include "Piece.h"

Piece::Piece(int32 pi, Array<int32> ei, Array<Array<Color>> epd)
    :pieceId(pi),
     edgeIndex(ei)
{
    for (int32 i = 0; i < ei.size(); i++) {
        edgePixelData << epd[ei[i]];
    }
    edgeBaseDiff = calcEdgeBaseDiff();
}

Array<int32> Piece::calcEdgeBaseDiff()   //基準となる色との差を取る事で比較的近い色を持った端の部分が配列の隣同士になりやすくなるはず
{
    Array<int32> result;
    Array<Color> grayArray(edgePixelData.size(), Color(127, 127, 127));   //何か基準となる色と比較する事で近いであろう端の色を持つピースを割り出す
    for (int32 i = 0; i < edgeIndex.size(); i++){
        for (int32 j = 0; j < grayArray.size(); j++) {
            //下の式は基準の色との差なので絶対値はいらない？(要検証)
            result << grayArray[j].r - edgePixelData[i][j].r + grayArray[j].g - edgePixelData[i][j].g + grayArray[i].b - edgePixelData[i][j].b;
        }
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

Array<int32> Piece::getEdgeBaseDiff()
{
    return edgeBaseDiff;
}

Array<int32> Piece::calcEdgeDiff(Array<Color> ed)   //このピースの４辺の差を返す
{
    Array<int32> result;
    for (int32 i = 0; i < edgePixelData.size(); i++) {
        for (int32 j = 0; j < edgePixelData[i].size(); j++) {
            result << Abs(edgePixelData[i][j].r - ed[j].r) + Abs(edgePixelData[i][j].g - ed[j].g) + Abs(edgePixelData[i][j].b - ed[j].b);
        }
    }
    return result;
}

