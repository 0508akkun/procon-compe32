#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum)
    :pieceWH(WH) {
    Array<Array<Color>> edgePixelData;
    Array<Color> edgePixel;
    //�[�̉�f�f�[�^���擾
    for (int32 y = 0; y < image.height(); y++) {    //���ɒT�����Ă���
        if (y % pieceWH == 0 || y % pieceWH == pieceWH - 1) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 x = 0; x < image.width(); x++) { //�z��ɉ�����
                edgePixel << image[y][x];
                if (x % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 x = 0; x < image.height(); x++) {    //�c�ɒT�����Ă���
        if (x % pieceWH == 0 || x % pieceWH == pieceWH - 1) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 y = 0; y < image.width(); y++) { //�z��ɉ�����
                edgePixel << image[y][x];
                if (y % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 y = 0; y < verDivNum; y++) { //���ꂼ��̒[��Index��ݒ�
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

Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> Solver::searchConnectPiece(int32 index)  //index�Ŏw�肳�ꂽ�s�[�X�ɂ��ĒT��
{
    Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> resultList;
    for (int32 i = 0; i < board[index].getEdgePixelData().size(); i++) {    //�w�肳�ꂽ�s�[�X�̒��ɑ��݂���G�b�W��S�T��
        Array<std::tuple<int32, PieceInfo, PieceInfo>> result;
        for (int32 j = 0; j < board.size(); j++) {  //�s�[�X���m��T��
            if (j == index)break;   //�����s�[�X�͒T�����Ȃ�
            for(int32 k = 0; k < board[j].getEdgePixelData().size(); k++){  //���̃G�b�W�Ɣ�r���鎞�ɑ��̃G�b�W�Ƃ̍����s�[�X�P�ʂœZ�߂ĕԂ��Ă��܂��̂ŕ���
                result << std::make_tuple(board[j].calcEdgeDiff(board[index].getEdgePixelData()[i])[k], PieceInfo(board[j].getPieceId(), board[j].getEdgeIndex()[k]), PieceInfo(board[index].getPieceId(), board[index].getEdgeIndex()[k])); //�w�肳�ꂽ�G�b�W�Ƒ��̃G�b�W�Ƃ̍��A�w�肳�ꂽ�G�b�W�Ɣ�r�����G�b�W�͂ǂ̃G�b�W�Ȃ̂��A�w�肳�ꂽ�G�b�W�Ɣ�r�����G�b�W�͂ǂ̃s�[�X�ɑ�����̂�
            }
        }
        if (!result.isEmpty()) {
            std::sort(result.begin(), result.end());
            result.resize(5);
            resultList << result;
        }
    }
    return resultList;    //�߂�l��
}

void Solver::connectPiece()
{
    Array<Array<std::tuple<int32, PieceInfo, PieceInfo>>> scoreList;    //�e�G�b�W�̍����L�^���Ă����z��
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
