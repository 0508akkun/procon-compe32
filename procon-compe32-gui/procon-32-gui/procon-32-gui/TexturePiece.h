#pragma once
#include <Siv3D.hpp>

class TexturePiece
{
	Texture pieceTexture;	//�s�[�X�̃e�N�X�`��
	bool selectFlag;	//�s�[�X���I������Ă��邩
	int32 rotatedNum;	//��]���
	int32 pieceWH;	//�s�[�X�̏c���̑傫��(�����`)
	int32 x, y;	//�s�[�X�̈ʒu
	void setSelectFlag();	//�s�[�X��I����Ԃɂ���
	void liftSelectFlag();	//�s�[�X�̑I����Ԃ���������
	bool checkSelectFlag();	//�I������Ă��邩�m�F����
	void turnRight();	//�s�[�X���E�ɉ�]������
	void turnLeft();	//�s�[�X�����ɉ�]������
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	TextureRegion showPieceTexture();	//�s�[�X��\������
};

