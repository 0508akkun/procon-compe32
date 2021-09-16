#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 hdn, int32 vdn)
    :pieceWH(WH),
     horDivNum(hdn),
     verDivNum(vdn)
    {
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
    graph = Array<Array<int32>>(board.size());  //�G�b�W���̗̈�����
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

Array<std::pair<int32, int32>> Solver::classifyPiece()  //�s�[�X���O���[�v�ɕ�����
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
    return resultList;    //�߂�l��
}

std::tuple<int32, PieceInfo, PieceInfo> Solver::searchConnectPiece(Array<std::pair<int32, PieceInfo>> cl, PieceInfo bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second.pieceId == bl.pieceId)break;    //�����s�[�X�ł̔�r�͖���
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
    int32 cornerIndex = -1; //���ɑ��݂���s�[�X�̃C���f�b�N�X���L�^����
    int32 subCornerIndex = -1;  //���ɑ��݂���s�[�X��������Ȃ�����1�̃s�[�X���A�����Ă��鎞�Ɍ�����̃C���f�b�N�X�ɔF��
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
}

int32 Solver::dfs(int32 index, int32 depth)
{
    int32 cornerIndex = -1;
    int32 returnValue = -1;
    if (graph[index].size() < 2 && depth == Min(horDivNum, verDivNum) - 1) {  //�����������ꍇ�A�C���f�b�N�X��߂�l��
        return index;
    }
    if (depth == Min(horDivNum, verDivNum)) {   //�ŒZ�̃s�[�X�̐����[�����[���Ȃ��Ă�����̋��͌������Ă��ׂ荇�����ł͖����̂ł��̑O�ɒT�����I��
        return -1;
    }
    depth++;
    for (int32 i = 0; i < graph[index].size(); i++) {
        returnValue = dfs(graph[index][i], depth);
        if (returnValue > -1) {
            cornerIndex = returnValue;
            return cornerIndex;
        }
    }
    return cornerIndex;
}

PieceInfo::PieceInfo(int32 pi, int32 ei)
    :pieceId(pi),
     edgeIndex(ei)
{
}
