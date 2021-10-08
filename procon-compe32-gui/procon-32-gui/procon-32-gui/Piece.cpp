#include "Piece.h"

//�f�t�H���g�R���X�g���N�^
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

Array<std::pair<Array<Color>, PieceInfo>> Piece::getEdgeBaseData()
{
    Array<std::pair<Array<Color>, PieceInfo>> result;
    Array<Color> grayArray(edgePixelData.size(), Color(127, 127, 127));   //������ƂȂ�F�Ɣ�r���鎖�ŋ߂��ł��낤�[�̐F�����s�[�X������o��
    for (int32 i = 0; i < edgeIndex.size(); i++) {
        result << std::make_pair(edgePixelData[i], PieceInfo(pieceId, edgeIndex[i]));
    }
    return result;
}
