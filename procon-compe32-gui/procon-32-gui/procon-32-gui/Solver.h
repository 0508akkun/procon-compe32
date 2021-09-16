#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

class Solver
{
	Array<Piece> board; //�ǂ��̒[���P�̃s�[�X���`�����Ă��邩���ʂ���l
	int32 pieceWH;	//�s�[�X�̏c���̑傫��
	int32 horDivNum;
	int32 verDivNum;
	Array<Array<int32>> graph;
	Array<std::pair<int32, PieceInfo>> getBaseDiffList();
	Array<std::pair<int32, int32>> classifyPiece();
	Array<Array<Color>> edgePixelData;
	std::tuple<int32, PieceInfo, PieceInfo> searchConnectPiece(Array<std::pair<int32, PieceInfo>>, PieceInfo bl);
	void connectPiece(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl);
	int32 dfs(int32 index, int32 depth);
public:
	Solver(Image image, int32 WH, int32 hdn, int32 vdn);
};

