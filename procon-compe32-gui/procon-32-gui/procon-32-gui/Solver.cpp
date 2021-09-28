#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 hdn, int32 vdn)
    :pieceWH(WH),
     horDivNum(hdn),
     verDivNum(vdn)
    {
    Print << U"IN";
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
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum + verDivNum;
            board << Piece(y * horDivNum + x, edgeIndex, Array<Array<Color>> {edgePixelData[edgeIndex[0]], edgePixelData[edgeIndex[1]], edgePixelData[edgeIndex[2]], edgePixelData[edgeIndex[3]]});
        }
    }
    graphMemo = Array<int32>(board.size(), -1);
    resultArray = Array<Array<std::pair<int32, int32>>>(verDivNum, Array<std::pair<int32, int32>>(horDivNum, std::make_pair(-1, 0)));
}

//sort��r�p�̊֐�
bool compare_edge(std::pair<int32, PieceInfo> edge1, std::pair<int32, PieceInfo> edge2)
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
    Print << U"BD";
    Array<std::pair<int32, PieceInfo>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<int32, PieceInfo>> edgeBaseDiffList = board[i].getEdgeBaseDiff();
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    std::sort(edgeBaseDiff.begin(), edgeBaseDiff.end(), compare_edge);
    return edgeBaseDiff;
}

Array<Array<std::pair<int32, int32>>> Solver::solveImage()
{
    Print << U"SI";
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
    connectList.resize(connectList.size() - (horDivNum * 2 + verDivNum * 2));
    //�s�[�X��T������
    //connectPiece(connectList);
    int32 x = horDivNum / 2;
    int32 y = verDivNum / 2;
    resultArray[y][x] = std::make_pair(0, 0);
    for (int32 i = 0; i < connectList.size(); i++) {
        if (std::get<1>(connectList[i]).pieceId == 0) {
            if (std::get<1>(connectList[i]).edgeIndex % (horDivNum * 2) < horDivNum && std::get<1>(connectList[i]).edgeIndex < horDivNum * verDivNum * 2) {
                dfs(connectList, i, graphMemo, resultArray, x, y, 0);
            }
            else if (std::get<1>(connectList[i]).edgeIndex % (horDivNum * 2) >= horDivNum && std::get<1>(connectList[i]).edgeIndex < horDivNum * verDivNum * 2) {
                dfs(connectList, i, graphMemo, resultArray, x, y, 2);
            }
            if (std::get<1>(connectList[i]).edgeIndex % (horDivNum * 2) < horDivNum && std::get<1>(connectList[i]).edgeIndex >= horDivNum * verDivNum * 2) {
                dfs(connectList, i, graphMemo, resultArray, x, y, 3);
            }
            else {
                dfs(connectList, i, graphMemo, resultArray, x, y, 1);
            }
        }
    }
    return resultArray;    //�߂�l��
}

void Solver::dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 index, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y, int32 rt)
{
    Print << U"DF";
    int32 edgeFromNum = std::get<1>(cl[index]).edgeIndex;   //��r���̃G�b�W�ԍ�
    int32 idFromNum = std::get<1>(cl[index]).pieceId;   //��r���̃C���f�b�N�X�ԍ�
    int32 edgeToNum = std::get<2>(cl[index]).edgeIndex; //��r��̃G�b�W�ԍ�
    int32 idToNum = std::get<2>(cl[index]).pieceId; //��r��̃C���f�b�N�X�ԍ�
    if (gm[idToNum] == 1) return; //�T����͒T���ς�
    gm[idFromNum] = 1;    //���݋���ꏊ��T���ς݂�
    //�s�[�Xid��0�̃s�[�X���������������̂ł��̌��������
    if (edgeFromNum % (horDivNum * 2) < horDivNum && edgeFromNum < horDivNum * verDivNum * 2) {
        for (int32 i= 0; i < cl.size(); i++) {
            if (std::get<1>(cl[i]).edgeIndex == edgeToNum) {    //��r��̃G�b�W�ԍ������̒�����T��
                moveResultData(cl[i], ra, x, y, rt);
                dfs(cl, i, gm, ra, x, y, rt);
                break;
            }
        }
    }
    else if(edgeFromNum % (horDivNum * 2) >= horDivNum && edgeFromNum < horDivNum * verDivNum * 2) {
        int32 rotated = rt + 2;
        if (rotated >= 4) {
            rotated %= 4;
        }
        for (int32 i = 0; i < cl.size(); i++) {
            if (std::get<1>(cl[i]).edgeIndex == edgeToNum) {
                moveResultData(cl[i], ra, x, y, rotated);
                dfs(cl, i, gm, ra, x, y, rotated);
                break;
            }
        }
    }
    else if (edgeFromNum % (verDivNum * 2) < verDivNum && edgeFromNum >= horDivNum * verDivNum * 2) {
        int32 rotated = rt + 3;
        if (rotated >= 4) {
            rotated %= 4;
        }
        for (int32 i = 0; i < cl.size(); i++) {
            if (std::get<1>(cl[i]).edgeIndex == edgeToNum) {
                moveResultData(cl[i], ra, x, y, rotated);
                dfs(cl, i, gm, ra, x, y, rotated);
                break;
            }
        }
    }
    else {
        int32 rotated = rt + 1;
        if (rotated >= 4) {
            rotated %= 4;
        }
        for (int32 i = 0; i < cl.size(); i++) {
            if (std::get<1>(cl[i]).edgeIndex == edgeToNum) {
                moveResultData(cl[i], ra, x, y, rotated);
                dfs(cl, i, gm, ra, x, y, rotated);
                break;
            }
        }
    }
    return;
}

void Solver::moveResultData(std::tuple<int32, PieceInfo, PieceInfo>& cl, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y,const int32 rt)
{
    Print << U"MR";
    if (rt == 0) {  
        if (y <= 0) {   //��ɗP�\��������
            for (int32 i = ra.size() - 1; i >= 0; i--) {
                for (int32 j = 0; j < ra[i].size();j++) {
                    if (i + 1 < ra.size()) {
                        ra[i + 1][j] = ra[i][j];
                    }
                }
            }
            ra[0][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�}������`�ɂȂ�̂Ō��݈ʒu�͍X�V���Ȃ�
        }
        else {
            y--;
            ra[y][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�ʒu���X�V���ċL�^����
        }
    }
    else if (rt == 1) { 
        if (x >= ra[y].size()) {    //�E�ɗP�\��������
            for (int32 i = 0; i < ra.size(); i++) {
                for (int32 j = 0; j < ra[i].size(); j++) {
                    if (j - 1 >= 0) {
                        ra[i][j - 1] = ra[i][j];
                    }
                }
            }
            ra[y][ra[y].size() - 1] = std::make_pair(std::get<2>(cl).pieceId, rt);  //�}������`�ɂȂ�̂Ō��݈ʒu�͍X�V���Ȃ�
        }
        else {
            x++;
            ra[y][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�ʒu���X�V���ċL�^����
        }
    }
    else if (rt == 2) { 
        if (y >= ra.size()) {   //���ɗP�\��������
            for (int32 i = 0; i < ra.size(); i++) {
                for (int32 j = 0; j < ra[i].size(); j++) {
                    if (i - 1 >= 0) {
                        ra[i - 1][j] = ra[i][j];
                    }
                }
            }
            ra[ra.size() - 1][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�}������`�ɂȂ�̂Ō��݈ʒu�͍X�V���Ȃ�
        }
        else {
            y++;
            ra[y][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�ʒu���X�V���ċL�^����
        }
    }
    else {  
        if (x <= 0) {   //���ɗP�\��������
            for (int32 i = 0; ra.size(); i++) {
                for (int32 j = ra[y].size() - 1; j >= 0; j--) {
                    if (j + 1 < ra[y].size()) {
                        ra[i][j + 1] = ra[i][j];
                    }
                }
            }
            ra[y][0] = std::make_pair(std::get<2>(cl).pieceId, rt); //�}������`�ɂȂ�̂Ō��݈ʒu�͍X�V���Ȃ�
        }
        else {
            x--;
            ra[y][x] = std::make_pair(std::get<2>(cl).pieceId, rt); //�ʒu���X�V���ċL�^����
        }
    }
}


std::tuple<int32, PieceInfo, PieceInfo> Solver::searchConnectPiece(Array<std::pair<int32, PieceInfo>>& cl, PieceInfo bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    for (int32 i = 0; i < cl.size(); i++) {
        for (int32 j = 0; j < edgePixelData[cl[i].second.edgeIndex].size(); j++) {
            if (cl[i].second.pieceId == bl.pieceId)break;    //�����s�[�X�ł̔�r�͖���
            scoreList << std::make_tuple(Abs(edgePixelData[cl[i].second.edgeIndex][j].r - edgePixelData[bl.edgeIndex][j].r) + Abs(edgePixelData[cl[i].second.edgeIndex][j].g - edgePixelData[bl.edgeIndex][j].g) + Abs(edgePixelData[cl[i].second.edgeIndex][j].b - edgePixelData[bl.edgeIndex][j].b), PieceInfo(bl.pieceId, bl.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
            Array<Color> rEdgePixelData = edgePixelData[cl[i].second.edgeIndex].reversed();
            scoreList << std::make_tuple(Abs(rEdgePixelData[j].r - edgePixelData[bl.edgeIndex][j].r) + Abs(rEdgePixelData[j].g - edgePixelData[bl.edgeIndex][j].g) + Abs(rEdgePixelData[j].b - edgePixelData[bl.edgeIndex][j].b), PieceInfo(bl.pieceId, bl.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex)); 
        }
    }
    std::sort(scoreList.begin(), scoreList.end(), compare_scoreList);
    return scoreList[0];
}

