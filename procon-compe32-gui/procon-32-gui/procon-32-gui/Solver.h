#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

struct PieceInfo {
	int32 pieceId;
	int32 edgeIndex;
	PieceInfo(int32 pi, int32 ei);
};

class Solver
{
	Array<Piece> board; //�ǂ��̒[���P�̃s�[�X���`�����Ă��邩���ʂ���l
	int32 pieceWH;	//�s�[�X�̏c���̑傫��
	Array<Array<int32>> graph;
	Array<std::pair<int32, int32>> getBaseDiffList();
	Array<std::pair<int32, int32>> classifyPiece();
	Array<Array<Color>> edgePixelData;
	void searchConnectPiece(Array<std::pair<int32, int32>> cl, int32 index);
	void connectPiece();
public:
	Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum);
};

