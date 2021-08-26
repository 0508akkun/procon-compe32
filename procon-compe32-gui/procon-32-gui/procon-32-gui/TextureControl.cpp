#include "TextureControl.h"

TextureControl::TextureControl()
{
    // ファイルをオープンする
    TextReader reader(U"../../../images/windmillnum.ppm");

    // オープンに失敗
    if (!reader)
    {
        throw Error(U"Failed to open `test.txt`");
    }

    // 行の内容を読み込む変数
    String line;
    String pixelValue = U"";
    Array<String> headerData;
    Array<String> pixelData;

    // 行数の表示用のカウント
    size_t i = 0;

    // 終端に達するまで 1 行ずつ読み込む
    while (reader.readLine(line))
    {
        //ヘッダ部のコメントを読み取る
        if (i > 5) {
            pixelData << line;
        }
        //画像データを読み取る
        else {
            headerData << line;
        }
        i++;
    }
    //このままだと画像データが配列になっているので一旦1つの文字列に変換
    for (const auto& pixelLine : pixelData) {
        pixelValue += pixelLine;
    }
    //ヘッダ部のコメントの情報を数値に変換
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
    //空のイメージに読み込んだ画像データを代入
    for (int32 y = 0; y < image.height(); y++) {
        for (int32 x = 0; x < image.width(); x++) {
            image[y][x] = Color(Parse<uint8>(pixel[(y * imageWidth + x) * 3]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 1]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 2]));
        }
    }
    pieceWH = imageHeight / verDiviNum;
    texture = Texture(image);
    //pieceの生成
    for (int32 i = 0; i < verDiviNum; i++) {
        for (int32 j = 0; j < horDiviNum; j++) {
            board << TexturePiece(texture, pieceWH * j, pieceWH * i, pieceWH);
            pieceID << horDiviNum * i + j;
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
            swapNum << pieceID[i];
            count++;
        }
        if (swapNum.size() == 2) {
            pieceSwap(board, pieceID, swapNum[0], swapNum[1]);
            board[swapNum[0]].liftSelectFlag();
            board[swapNum[1]].liftSelectFlag();
            swapNum.clear();
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

void TextureControl::pieceSwap(Array<TexturePiece> &bo, Array<int32> id, int32 source, int32 to)
{
    std::swap(bo[source], bo[to]);
    std::swap(id[source], id[to]);
}

Array<int32> TextureControl::setPieceID()
{
	return pieceID;
}

void TextureControl::showBoard()
{
    for (int32 i = 0; i < verDiviNum; i++) {
        for (int32 j = 0; j < horDiviNum; j++) {
            board[i * horDiviNum + j].setSelectFlag();
            if (board[i * horDiviNum + j].getSelectFlag()) {
                board[i * horDiviNum + j].getPiece(i, j)(board[i * horDiviNum + j].getPieceTexture()).draw().drawFrame(0, 3, Palette::Orange);
            }
            else {
                board[i * horDiviNum + j].getPiece(i, j)(board[i * horDiviNum + j].getPieceTexture()).draw();
            }
        }
    }
}

void TextureControl::rotatedPiece(Array<TexturePiece>& bo, Array<int32> swapNum)
{
    if (KeyA.down()) {
        bo[swapNum[0]].turnLeft();
        bo[swapNum[0]].liftSelectFlag();
    }
    if (KeyD.down()) {
        bo[swapNum[0]].turnRight();
        bo[swapNum[0]].liftSelectFlag();
    }
}
