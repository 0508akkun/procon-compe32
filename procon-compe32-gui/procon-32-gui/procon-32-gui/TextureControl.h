#pragma once
#include "TexturePiece.h"

class TextureControl
{
	Array<TexturePiece> board;	//�s�[�X�S�̂̏W��
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
	Array<Array<std::pair<int32, int32>>> result;
	bool swapFlag;
	void moveSolverResult(Array<TexturePiece>& bo, Array<Array<std::pair<int32, int32>>>& result);
public:
	TextureControl();
	TextureControl(int32 dami);
	void checkSelectFlag();
	void rotatedPiece(Array<TexturePiece>& bo, Array<int32> swapNum);
	void pieceSwap(Array<TexturePiece>& bo, int32 source, int32 to);
	Array<int32> getPieceID();
	void showBoard();
	void setSolverData();
	void writeResultText();
};

