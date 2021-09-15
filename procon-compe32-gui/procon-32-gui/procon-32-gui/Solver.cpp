#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum)
    :pieceWH(WH) {
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
    graph = Array<Array<int32>>(board.size());
}

Array<std::pair<int32, int32>> Solver::getBaseDiffList()
{
    Array<std::pair<int32, int32>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<int32, int32>> edgeBaseDiffList = board[i].getEdgeBaseDiff();
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    edgeBaseDiff.sort();
    return edgeBaseDiff;
}

Array<std::pair<int32, int32>> Solver::classifyPiece()  //�s�[�X���O���[�v�ɕ�����
{
    Array<std::pair<int32, int32>> baseDiffList = getBaseDiffList();
    for (int32 i = 0; i < baseDiffList.size(); i++) {
        Array<std::pair<int32, int32>> candiList;
        if (i < 5) {
            std::copy(baseDiffList.begin(), baseDiffList.begin() + i + 5, candiList.begin());
        }
        else if (i > baseDiffList.size() - 6) {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.end(), candiList.begin());
        }
        else {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.begin() + i + 5, baseDiffList.begin());
        }
        searchConnectPiece(candiList, baseDiffList[i].second);
    }
    return resultList;    //�߂�l��
}

void Solver::searchConnectPiece(Array<std::pair<int32, int32>> cl, int32 index)
{
    Array<std::tuple<int32, int32, int32>> scoreList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second == index)break;    //�����s�[�X�ł̔�r�͖���
        for (int32 j = 0; j < edgePixelData[0].size(); j++) {
            scoreList << std::make_tuple(Abs(edgePixelData[cl[i].second][j].r - edgePixelData[index][j].r) + Abs(edgePixelData[cl[i].second][j].g - edgePixelData[index][j].g) + Abs(edgePixelData[cl[i].second][j].b - edgePixelData[index][j].b), index, cl[i].second);
            Array<Color> rEdgePixelData = edgePixelData[cl[i].second].reversed();
            scoreList << std::make_tuple(Abs(rEdgePixelData[j].r - edgePixelData[index][j].r) + Abs(rEdgePixelData[j].g - edgePixelData[index][j].g) + Abs(rEdgePixelData[j].b - edgePixelData[index][j].b), index, cl[i].second);
        }
    }
    scoreList.sort();
    graph[std::get<1>(scoreList[0])] << std::get<2>(scoreList[0]);
}

void Solver::connectPiece()
{
    
}

PieceInfo::PieceInfo(int32 pi, int32 ei)
    :pieceId(pi),
     edgeIndex(ei)
{
}
