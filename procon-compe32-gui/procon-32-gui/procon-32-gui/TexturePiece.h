#pragma once
#include <Siv3D.hpp>

class TexturePiece
{
	TextureRegion pieceTexture;	//�s�[�X�̃e�N�X�`��
	Rect piece;
	bool selectFlag;	//�s�[�X���I������Ă��邩
	int32 rotatedNum;	//��]���
	int32 pieceWH;	//�s�[�X�̏c���̑傫��(�����`)
	int32 x, y;	//�s�[�X�̈ʒu
	void setSelectFlag();	//�s�[�X��I����Ԃɂ���
	void liftSelectFlag();	//�s�[�X�̑I����Ԃ���������
	void turnRight();	//�s�[�X���E�ɉ�]������
	void turnLeft();	//�s�[�X�����ɉ�]������
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	TextureRegion showPieceTexture();	//�s�[�X��\������
	Rect getPiece(int32 y, int32 x);
	bool checkSelectFlag();	//�I������Ă��邩�m�F����
};

