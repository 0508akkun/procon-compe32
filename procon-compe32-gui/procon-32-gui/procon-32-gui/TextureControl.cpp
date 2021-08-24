#include "TextureControl.h"

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
    size_t i = 0;

    // �I�[�ɒB����܂� 1 �s���ǂݍ���
    while (reader.readLine(line))
    {
        //�w�b�_���̃R�����g��ǂݎ��
        if (i > 5) {
            pixelData << line;
        }
        //�摜�f�[�^��ǂݎ��
        else {
            headerData << line;
        }
        i++;
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
    const int32 pieceWH = imageHeight / verDiviNum;
    const Texture texture(image);
    //piece�̐���
    for (int32 i = 0; i < verDiviNum; i++) {
        for (int32 j = 0; j < horDiviNum; j++) {
            board << TexturePiece(texture, pieceWH * j, pieceWH * i, pieceWH);
        }
    }
}

void TextureControl::pieceSwap()
{

}

Array<int32> TextureControl::setPieceID()
{
	return pieceID;
}
