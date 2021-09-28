#include "TextureControl.h"
#include "Solver.h"

TextureControl::TextureControl()
{
    // �t�@�C�����I�[�v������
    TextReader reader(U"../../../images/windmillnum.ppm");

    // �I�[�v���Ɏ��s
    if (!reader)
    {
        throw Error(U"Failed to open `test.txt`");
    }

    // �s�̓��e��ǂݍ��ޕϐ�
    String line;
    String pixelValue = U"";
    Array<String> headerData;
    Array<String> pixelData;

    // �s���̕\���p�̃J�E���g
    size_t count = 0;

    // �I�[�ɒB����܂� 1 �s���ǂݍ���
    while (reader.readLine(line))
    {
        //�w�b�_���̃R�����g��ǂݎ��
        if (count > 5) {
            pixelData << line;
        }
        //�摜�f�[�^��ǂݎ��
        else {
            headerData << line;
        }
        count++;
    }
    //���̂܂܂��Ɖ摜�f�[�^���z��ɂȂ��Ă���̂ň�U1�̕�����ɕϊ�
    for (const auto& pixelLine : pixelData) {
        pixelValue += pixelLine;
    }
    //�w�b�_���̃R�����g�̏��𐔒l�ɕϊ�
    Array<String> diviNum = headerData[1].split(U' ');
    horDiviNum = Parse<int32>(diviNum[1]);
    verDiviNum = Parse<int32>(diviNum[2]);
    Array<String> seleLimNumStr = headerData[2].split(U' ');
    selectLimitNum = Parse<int32>(seleLimNumStr[1]);
    Array<String> cost = headerData[3].split(U' ');
    selectCost = Parse<int32>(cost[1]);
    changeCost = Parse<int32>(cost[2]);
    Array<String> imageSize = headerData[4].split(U' ');
    imageWidth = Parse<int32>(imageSize[0]);
    imageHeight = Parse<int32>(imageSize[1]);
    image = Image(imageWidth, imageHeight, Palette::White);
    Array<String> pixel = pixelValue.split(U' ');
    //��̃C���[�W�ɓǂݍ��񂾉摜�f�[�^����
    for (int32 y = 0; y < image.height(); y++) {
        for (int32 x = 0; x < image.width(); x++) {
            image[y][x] = Color(Parse<uint8>(pixel[(y * imageWidth + x) * 3]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 1]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 2]));
        }
    }
    pieceWH = imageHeight / verDiviNum;
    texture = Texture(image);
    //piece�̐���
    for (int32 i = 0; i < verDiviNum; i++) {
        for (int32 j = 0; j < horDiviNum; j++) {
            board << TexturePiece(texture, horDiviNum * i + j, Vec2(j, i), pieceWH);
        }
    }
    swapFlag = false;
}

void TextureControl::checkSelectFlag()
{
    int32 count = 0;
    Array<int32> swapNum;
    for (int i = 0; i < horDiviNum * verDiviNum; i++) {
        if (board[i].getSelectFlag()) {
            swapNum << i;
            count++;
        }
        if (swapNum.size() == 2) {
            pieceSwap(board, swapNum[0], swapNum[1]);
            return;
        }
    }
    if (count == 0) {
        return;
    }
    else {
        rotatedPiece(board, swapNum);
    }
    return;
}

void TextureControl::pieceSwap(Array<TexturePiece> &bo, int32 source, int32 to)
{
    Vec2 vec = bo[source].getCoordinate();
    bo[source].setCoordinate(bo[to].getCoordinate());
    bo[to].setCoordinate(vec);
    std::swap(bo[source], bo[to]);
    bo[source].liftSelectFlag();
    bo[to].liftSelectFlag();
}

Array<int32> TextureControl::getPieceID()
{
    Array<int32> idList;
    for (auto& piece : board) {
        idList << piece.getPieceID();
    }
    return idList;
}

void TextureControl::showBoard()
{
    for (int32 i = 0; i < verDiviNum; i++) {
        for (int32 j = 0; j < horDiviNum; j++) {
            board[i * horDiviNum + j].setSelectFlag();
            if (board[i * horDiviNum + j].getSelectFlag()) {
                board[i * horDiviNum + j].getPiece()(board[i * horDiviNum + j].getPieceTexture()).draw().drawFrame(0, 3, Palette::Orange);
            }
            else {
                board[i * horDiviNum + j].getPiece()(board[i * horDiviNum + j].getPieceTexture()).draw();
            }
        }
    }
}

void TextureControl::setSolverData()
{
    Solver s = Solver(image, pieceWH, horDiviNum, verDiviNum);
    Array<Array<std::pair<int32, int32>>> result = s.solveImage();
    moveSolverResult(board, result);
}

void TextureControl::moveSolverResult(Array<TexturePiece>& bo, Array<Array<std::pair<int32, int32>>>& result)
{
    for (int32 i = 0; i < result.size(); i++) {
        for (int32 j = 0; j < result[i].size(); j++) {
            if (bo[i * horDiviNum + j].getPieceID() != result[i][j].first) {
                for (int32 k = i * horDiviNum + j; k < bo.size(); k++) {
                    if (bo[k].getPieceID() == result[i][j].first) {
                        pieceSwap(bo, k, i * horDiviNum + j);    //���̃s�[�X�̔z����\���o���������ʂ�ɕ��ג���
                    }
                    if (result[i][j].second == 1) { //��]�����f����
                        bo[i * horDiviNum + j].turnLeft();
                    }
                    else if (result[i][j].second == 2) {
                        bo[i * horDiviNum + j].turnLeft();
                        bo[i * horDiviNum + j].turnLeft();
                    }
                    else if (result[i][j].second == 3) {
                        bo[i * horDiviNum + j].turnRight();
                    }
                    else {
                    }
                }
            }
        }
    }
}

void TextureControl::rotatedPiece(Array<TexturePiece>& bo, Array<int32> swapNum)
{
    if (KeyA.down()) {
        bo[swapNum[0]].turnLeft();
    }
    if (KeyD.down()) {
        bo[swapNum[0]].turnRight();
    }
}
