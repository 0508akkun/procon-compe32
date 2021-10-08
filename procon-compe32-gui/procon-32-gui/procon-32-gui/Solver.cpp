#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 hdn, int32 vdn)
    :pieceWH(WH),
     horDivNum(hdn),
     verDivNum(vdn)
    {
    Array<Color> edgePixel;
    //�[�̉�f�f�[�^���擾
    for (int32 y = 0; y < image.height(); y++) {    //���ɒT�����Ă���
        if (y % pieceWH == 0) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 x = 0; x < image.width(); x++) { //�z��ɉ�����
                edgePixel << image[y][x];
                if (x % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
        if (y % pieceWH == pieceWH - 1) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 x = image.width() - 1; x >= 0; x--) { //�z��ɉ�����
                edgePixel << image[y][x];
                if (x % pieceWH == 0) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 x = 0; x < image.width(); x++) {    //�c�ɒT�����Ă���
        if (x % pieceWH == 0) {   //���Ă���ꏊ���[�ɂ��鎞��
            for (int32 y = image.height() - 1; y >= 0; y--) { //�z��ɉ�����
                edgePixel << image[y][x];
                if (y % pieceWH == 0) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
        if (x % pieceWH == pieceWH - 1) {   //���Ă���ꏊ���[�ɂ��鎞��
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
            edgeIndex << y * horDivNum + horDivNum - x - 1 + horDivNum + horDivNum * y;
            edgeIndex << horDivNum * verDivNum * 2 + verDivNum - y - 1 + verDivNum * x * 2;
            edgeIndex << horDivNum * verDivNum * 2 + y + verDivNum * x * 2 + verDivNum;
            Array<Array<Color>> epd = { edgePixelData[edgeIndex[0]], edgePixelData[edgeIndex[1]], edgePixelData[edgeIndex[2]], edgePixelData[edgeIndex[3]] };
            //Print << y * horDivNum + x << U" " << edgeIndex[0] << U" " << edgeIndex[1] << U" " << edgeIndex[2] << U" " << edgeIndex[3];
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

Array<std::pair<Array<Color>, PieceInfo>> Solver::getBaseDataList()
{
    Array<std::pair<Array<Color>, PieceInfo>> edgeBaseDiff;
    for (int32 i = 0; i < board.size(); i++) {
        Array<std::pair<Array<Color>, PieceInfo>> edgeBaseDiffList = board[i].getEdgeBaseData();   //�s�[�X���ꂼ��̊�{�F�Ƃ̍�
        for (int32 j = 0; j < edgeBaseDiffList.size(); j++) {
            edgeBaseDiff << edgeBaseDiffList[j];
        }
    }
    return edgeBaseDiff;
}

Array<Array<std::pair<int32, int32>>> Solver::solveImage()
{
    //�s�[�X���O���[�v�ɕ�����
    Array<std::pair<Array<Color>, PieceInfo>> baseDataList = getBaseDataList();
    Array<std::tuple<int32, PieceInfo, PieceInfo>> connectList;
    Array<std::tuple<int32, PieceInfo, PieceInfo>> resultList;
    for (int32 i = 0; i < baseDataList.size(); i++) {
        resultList = searchConnectPiece(baseDataList, baseDataList[i]);
        for (int32 j = 0; j < resultList.size(); j++) {
            connectList << resultList[j];
        }
    }
    std::sort(connectList.begin(), connectList.end(), compare_connectList);
    //for (int32 i = 50; i < 75; i++) {
    //    Print << std::get<0>(connectList[i]) << U" " << std::get<1>(connectList[i]).pieceId << U" " << std::get<1>(connectList[i]).edgeIndex << U" " << std::get<2>(connectList[i]).pieceId << U" " << std::get<2>(connectList[i]).edgeIndex;
    //}
    //�s�[�X��T������
    int32 x = horDivNum / 2;
    int32 y = verDivNum / 2;
    resultArray[y][x] = std::make_pair(0, 0);
    dfs(connectList, 0, graphMemo, resultArray, x, y, 0);
    for (int32 i = 0; i < resultData.size(); i++) {
        Print << std::get<0>(resultData[i]) << U" " << std::get<1>(resultData[i]) << U" " << std::get<2>(resultData[i]) << U" " << std::get<3>(resultData[i]);
    }
    calcResultData(resultData, resultArray);
    return resultArray;    //�߂�l��
}

void Solver::dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 pi, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32 x, int32 y, int32 rt)
{
    if (gm[pi] == 1) return; //�T����͒T���ς�
    gm[pi] = 1;    //���݋���ꏊ��T���ς݂�
    setResultData(pi, x, y, rt, resultData);
    for (int32 i = 0; i < cl.size(); i++) {
        if (pi == std::get<1>(cl[i]).pieceId) { //�ړI��pieceId�ȊO�͒T�����Ȃ�
            for (int32 j = 0; j < cl.size(); j++) {
                int32 sx = x, sy = y, srt = rt;
                if (std::get<0>(cl[i]) == std::get<0>(cl[j])) {
                    if (std::get<1>(cl[i]).edgeIndex == std::get<2>(cl[j]).edgeIndex && std::get<2>(cl[i]).edgeIndex == std::get<1>(cl[j]).edgeIndex) { //�݂��ɓ����[���Q�Ƃ�����
                        if (gm[std::get<2>(cl[i]).pieceId] == 1)continue;
                        srt += fromRotateConnectPiece(std::get<1>(cl[i]).edgeIndex);
                        srt %= 4; //��]����0�`3�͈̔͂�
                        if (srt == 0) {
                            sy--;
                        }
                        else if (srt == 2) {
                            sy++;
                        }
                        else if (srt == 3) {
                            sx--;
                        }
                        else {
                            sx++;
                        }
                        srt = toRotateConnectPiece(std::get<2>(cl[i]).edgeIndex, srt);
                        dfs(cl, std::get<2>(cl[i]).pieceId, gm, ra, sx, sy, srt);
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
    Array<Point> coord;
    std::sort(rd.begin(), rd.end());
    for (int32 i = 0; i < rd.size(); i++) {
        int32 x = std::get<0>(rd[i]) - std::get<0>(rd[0]);
        int32 y = std::get<1>(rd[i]) - std::get<1>(rd[0]);
        coord << Point(x, y);
    }
    for (int32 i = 0; i < ra.size(); i++) {
        for (int32 j = 0; j < ra[i].size(); j++) {
            for (int32 k = 0; k < rd.size(); k++) {
                if (coord[k].y >= 0 && coord[k].y < verDivNum && coord[k].x >= 0 && coord[k].x < horDivNum) {
                    ra[coord[k].y][coord[k].x] = std::make_pair(std::get<2>(rd[k]), std::get<3>(rd[k]));
                }
            }
        }
    }
}

int32 Solver::fromRotateConnectPiece(int32 index)   //edgeIndex����ǂ̕��������Ă�̂���������
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

int32 Solver::toRotateConnectPiece(int32 index, int32 r)
{
    if (r == 0) {
        if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //��r�O�̃G�b�W�ԍ��������ŏ㑤�̏ꍇ
            return 2;
        }
        else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //��r�O�̃G�b�W�ԍ��������ŉ����̏ꍇ
            return 0;
        }
        else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //��r�O�̃G�b�W�ԍ��������ō����̏ꍇ
            return 3;
        }
        else {
            return 1;
        }
    }
    else if (r == 1) {
        if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //��r�O�̃G�b�W�ԍ��������ŏ㑤�̏ꍇ
            return 3;
        }
        else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //��r�O�̃G�b�W�ԍ��������ŉ����̏ꍇ
            return 1;
        }
        else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //��r�O�̃G�b�W�ԍ��������ō����̏ꍇ
            return 0;
        }
        else {
            return 2;
        }
    }
    else if (r == 2) {
        if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //��r�O�̃G�b�W�ԍ��������ŏ㑤�̏ꍇ
            return 0;
        }
        else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //��r�O�̃G�b�W�ԍ��������ŉ����̏ꍇ
            return 2;
        }
        else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //��r�O�̃G�b�W�ԍ��������ō����̏ꍇ
            return 1;
        }
        else {
            return 3;
        }
    }
    else {
        if (index % (horDivNum * 2) < horDivNum && index < horDivNum * verDivNum * 2) { //��r�O�̃G�b�W�ԍ��������ŏ㑤�̏ꍇ
            return 1;
        }
        else if (index % (horDivNum * 2) >= horDivNum && index < horDivNum * verDivNum * 2) {    //��r�O�̃G�b�W�ԍ��������ŉ����̏ꍇ
            return 3;
        }
        else if (index % (verDivNum * 2) < verDivNum && index >= horDivNum * verDivNum * 2) {   //��r�O�̃G�b�W�ԍ��������ō����̏ꍇ
            return 2;
        }
        else {
            return 0;
        }
    }
}

Array<std::tuple<int32, PieceInfo, PieceInfo>> Solver::searchConnectPiece(Array<std::pair<Array<Color>, PieceInfo>>& cl, std::pair<Array<Color>, PieceInfo> bl)
{
    Array<std::tuple<int32, PieceInfo, PieceInfo>> scoreList;
    Array<std::tuple<int32, PieceInfo, PieceInfo>> resultList;
    for (int32 i = 0; i < cl.size(); i++) {
        if (cl[i].second.pieceId != bl.second.pieceId) {    //�����s�[�X�ł̔�r�͖���
            int32 absRevValue = 0;
            for (int32 j = 0; j < cl[i].first.size(); j++) {
                //�Ȃ�����f�̒l�̐�Βl�����Ȃ��̂ő��������
                Array<Color> rEdgePixelData = cl[i].first.reversed();
                int32 absValueR = rEdgePixelData[j].r - bl.first[j].r;
                absValueR = Abs(absValueR);
                int32 absValueG = rEdgePixelData[j].g - bl.first[j].g;
                absValueG = Abs(absValueG);
                int32 absValueB = rEdgePixelData[j].b - bl.first[j].b;
                absValueB = Abs(absValueB);
                absRevValue += absValueR + absValueG + absValueB;
            }
            scoreList << std::make_tuple(absRevValue, PieceInfo(bl.second.pieceId, bl.second.edgeIndex), PieceInfo(cl[i].second.pieceId, cl[i].second.edgeIndex));
        }
    }
    if (!scoreList.empty()) {
        std::sort(scoreList.begin(), scoreList.end(), compare_scoreList);
        for (int32 i = 0; i < scoreList.size(); i++) {
            if (std::get<0>(scoreList[i]) == std::get<0>(scoreList[0])) {
                resultList << scoreList[i];
            }
        }
    }
    return resultList;
}

