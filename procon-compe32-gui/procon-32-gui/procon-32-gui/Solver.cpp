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
    for (int32 x = 0; x < image.width(); x++) {    //�c�ɒT�����Ă���
        if (x % pieceWH == 0 || x % pieceWH == pieceWH - 1) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 y = 0; y < image.height(); y++) { //�z��ɉ�����
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
            edgeIndex << horDivNum * verDivNum * 2 + y + verDivNum * x * 2;
            edgeIndex << horDivNum * verDivNum * 2 + y + verDivNum * x * 2 + verDivNum;
            Array<Array<Color>> epd = { edgePixelData[edgeIndex[0]], edgePixelData[edgeIndex[1]], edgePixelData[edgeIndex[2]], edgePixelData[edgeIndex[3]] };
            board << Piece(y * horDivNum + x, edgeIndex, epd);
        }
    }
    graphMemo = Array<int32>(board.size(), -1);
    resultArray = Array<Array<std::pair<int32, int32>>>(verDivNum, Array<std::pair<int32, int32>>(horDivNum, std::make_pair(0, 0)));
}

//sort��r�p�̊֐�
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

Array<std::pair<int32, PieceInfo>> Solver::getBaseDiffList()
{
    Array<std::pair<int32, PieceInfo>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<int32, PieceInfo>> edgeBaseDiffList = board[i].getEdgeBaseDiff();   //�s�[�X���ꂼ��̊�{�F�Ƃ̍�
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    std::sort(edgeBaseDiff.begin(), edgeBaseDiff.end(), compare_edge);
    return edgeBaseDiff;
}

Array<Array<std::pair<int32, int32>>> Solver::solveImage()
{
    //�s�[�X���O���[�v�ɕ�����
    Array<std::pair<int32, PieceInfo>> baseDiffList = getBaseDiffList();
    Array<std::tuple<int32, PieceInfo, PieceInfo>> connectList;
    for (int32 i = 0; i < baseDiffList.size(); i++) {
        Array<std::pair<int32, PieceInfo>> candiList;
        if (i < 5) {
            std::copy(baseDiffList.begin(), baseDiffList.begin() + i + 5, back_inserter(candiList));
        }
        else if (i > baseDiffList.size() - 6) {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.end(), back_inserter(candiList));
        }
        else {
            std::copy(baseDiffList.begin() + i - 5, baseDiffList.begin() + i + 5, back_inserter(candiList));
        }
        connectList << searchConnectPiece(candiList, baseDiffList[i].second);
    }
    std::sort(connectList.begin(), connectList.end(), compare_connectList);
    /*for (int32 i = 0; i < connectList.size(); i++) {
        Print << std::get<0>(connectList[i]) << U" " << std::get<1>(connectList[i]).pieceId << U" " << std::get<1>(connectList[i]).edgeIndex << U" " << std::get<2>(connectList[i]).pieceId << U" " << std::get<2>(connectList[i]).edgeIndex;
    }*/
    //�s�[�X��T������
    int32 x = horDivNum / 2;
    int32 y = verDivNum / 2;
    resultArray[y][x] = std::make_pair(0, 0);
    dfs(connectList, 0, graphMemo, resultArray, x, y, 0);
    return resultArray;    //�߂�l��
}

void Solver::dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 pi, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32 x, int32 y, int32 rt)
{
    if (gm[pi] == 1) return; //�T����͒T���ς�
    gm[pi] = 1;    //���݋���ꏊ��T���ς݂�
    rt %= 4;    //��]����0�`3�͈̔͂�
    setResultData(pi, x, y, rt, resultData);
    for (int32 i = 0; i < cl.size(); i++) {
        if (pi == std::get<1>(cl[i]).pieceId) { //�ړI��pieceId�ȊO�͒T�����Ȃ�
            for (int32 j = 0; j < cl.size(); j++) {
                int32 sx = x, sy = y;
                if (std::get<0>(cl[i]) == std::get<0>(cl[j])) {
                    if (std::get<1>(cl[i]).edgeIndex == std::get<2>(cl[j]).edgeIndex && std::get<2>(cl[i]).edgeIndex == std::get<1>(cl[j]).edgeIndex) { //�݂��ɓ����[���Q�Ƃ�����
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
    std::sort(rd.begin(), rd.end());
    for (int32 i = 0; i < rd.size(); i++) {
        std::get<0>(rd[i]) -= std::get<0>(rd[0]);
        std::get<1>(rd[i]) -= std::get<1>(rd[0]);
    }
    for (int32 i = 0; i < ra.size(); i++) {
        for (int32 j = 0; j < ra[i].size(); j++) {
            for (int32 k = 0; k < rd.size(); i++) {
                if (std::get<1>(rd[k]) >= 0 && std::get<1>(rd[k]) < verDivNum && std::get<0>(rd[k]) >= 0 && std::get<0>(rd[k]) < horDivNum) {
                    ra[std::get<1>(rd[k])][std::get<0>(rd[k])] = std::make_pair(std::get<2>(rd[k]), std::get<3>(rd[k]));
                }
            }
        }
    }
}



int32 Solver::rotateConnectPiece(int32 index)   //edgeIndex����ǂ̕��������Ă�̂���������
{
    if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //��r�O�̃G�b�W�ԍ��������ŏ㑤�̏ꍇ
        return 0;
    }
    else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //��r�O�̃G�b�W�ԍ��������ŉ����̏ꍇ
        return 2;
    }
    else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //��r�O�̃G�b�W�ԍ��������ō����̏ꍇ
        return 3;
    }
    else {
        return 1;
    }
}

std::tuple<int32, PieceInfo, PieceInfo> Solver::searchConnectPiece(Array<std::pair<int32, PieceInfo>>& cl, PieceInfo bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second.pieceId != bl.pieceId) {    //�����s�[�X�ł̔�r�͖���
            for (int32 j = 0; j < edgePixelData[cl[i].second.edgeIndex].size(); j++) {
                //�Ȃ�����f�̒l�̐�Βl�����Ȃ��̂ő��������
                int32 absValueR = edgePixelData[cl[i].second.edgeIndex][j].r - edgePixelData[bl.edgeIndex][j].r;
                absValueR = Abs(absValueR);
                int32 absValueG = edgePixelData[cl[i].second.edgeIndex][j].g - edgePixelData[bl.edgeIndex][j].g;
                absValueG = Abs(absValueG);
                int32 absValueB = edgePixelData[cl[i].second.edgeIndex][j].b - edgePixelData[bl.edgeIndex][j].b;
                absValueB = Abs(absValueB);
                int32 absValue = absValueR + absValueG + absValueB;
                Array<Color> rEdgePixelData = edgePixelData[cl[i].second.edgeIndex].reversed();
                absValueR = rEdgePixelData[j].r - edgePixelData[bl.edgeIndex][j].r;
                absValueR = Abs(absValueR);
                absValueG = rEdgePixelData[j].g - edgePixelData[bl.edgeIndex][j].g;
                absValueG = Abs(absValueG);
                absValueB = rEdgePixelData[j].b - edgePixelData[bl.edgeIndex][j].b;
                absValueB = Abs(absValueB);
                int32 absRevValue = absValueR + absValueG + absValueB;
                scoreList << std::make_tuple(Min(absValue, absRevValue), PieceInfo(bl.pieceId, bl.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
            }
        }
    }
    std::sort(scoreList.begin(), scoreList.end(), compare_scoreList);
    return scoreList[0];
}

