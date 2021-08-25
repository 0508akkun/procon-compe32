#pragma once
#include "TexturePiece.h"

class TextureControl
{
	Array<TexturePiece> board;	//�s�[�X�S�̂̏W��
	Array<int32> pieceID;	//�s�[�X�̌ŗLID(�ŏ��̕��т��ǂ��Ȃ��Ă��邩)
	int32 selectCost;	//�I���R�X�g
	int32 changeCost;	//�����R�X�g
	int32 selectLimitNum;	//�I�����E��
	int32 horDiviNum;	//���̕�����
	int32 verDiviNum;	//�c�̕�����
	int32 imageWidth;	//�摜���̂̕�
	int32 imageHeight;	//�摜���̂̍���
	Image image;	//�摜���̂���
	int32 pieceWH;
	Texture texture;
public:
	TextureControl();
	void pieceSwap();
	Array<int32> setPieceID();
	void showBoard();
};

