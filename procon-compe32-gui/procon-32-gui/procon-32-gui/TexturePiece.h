#pragma once
#include <Siv3D.hpp>

class TexturePiece
{
	TextureRegion pieceTexture;	//�s�[�X�̃e�N�X�`��
	Quad piece;
	bool selectFlag;	//�s�[�X���I������Ă��邩
	int32 rotatedNum;	//��]���
	int32 pieceWH;	//�s�[�X�̏c���̑傫��(�����`)
	Vec2 pos;	//�s�[�X�̈ʒu
	int32 imageOffsetX = 10;	//�[�Ɋ��߂���ƑI���������ɐF�������Ȃ��Ȃ邽�߃I�t�Z�b�g���쐬
	int32 imageOffsetY = 10;
	int32 imageWidth;
	int32 imageHeight;
	int32 pieceID;
public:
	void setRotate(int32 rotate);
	TexturePiece(Texture texture, int32 id, Vec2 p, int32 wh, int32 iw, int32 ih);
	void turnRight();	//�s�[�X���E�ɉ�]������
	void turnLeft();	//�s�[�X�����ɉ�]������
	bool getSelectFlag();
	void liftSelectFlag();
	void setSelectFlag();	//�s�[�X��I����Ԃɂ���
	TextureRegion getPieceTexture();	//�s�[�X��\������
	Quad getPiece();
	Vec2 getCoordinate();
	void setCoordinate(Vec2 vec);
	int32 getPieceID();
	int32 getRotate();
};

