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
	int32 imageOffsetX = 10;	//�[�Ɋ��߂���ƑI���������ɐF�������Ȃ��Ȃ邽�߃I�t�Z�b�g���쐬
	int32 imageOffsetY = 10;
public:
	TexturePiece(Texture texture, int32 x, int32 y, int32 wh);
	void turnRight();	//�s�[�X���E�ɉ�]������
	void turnLeft();	//�s�[�X�����ɉ�]������
	bool getSelectFlag();
	void liftSelectFlag();
	void setSelectFlag();	//�s�[�X��I����Ԃɂ���
	TextureRegion getPieceTexture();	//�s�[�X��\������
	Rect getPiece(int32 y, int32 x);
};

