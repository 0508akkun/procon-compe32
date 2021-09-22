#pragma once
#include <Siv3D.hpp>
#include "Piece.h"

class Solver
{
	Array<Piece> board; //�ǂ��̒[���P�̃s�[�X���`�����Ă��邩���ʂ���l
	int32 pieceWH;	//�s�[�X�̏c���̑傫��
	int32 horDivNum;
	int32 verDivNum;
	Array<int32> graphMemo;	//��x�ʂ����������邩����
	Array<Array<std::pair<int32, int32>>> resultArray;
	Array<std::pair<int32, PieceInfo>> getBaseDiffList();
	Array<Array<std::pair<int32, int32>>> solveImage();
	Array<Array<Color>> edgePixelData;
	std::tuple<int32, PieceInfo, PieceInfo> searchConnectPiece(Array<std::pair<int32, PieceInfo>>, PieceInfo bl);
	void dfs(Array<std::tuple<int32, PieceInfo, PieceInfo>> cl, int32 index, Array<int32>& gm, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y, int32 rt);
	void moveResultData(const std::tuple<int32, PieceInfo, PieceInfo>& cl, Array<Array<std::pair<int32, int32>>>& ra, int32& x, int32& y, const int32 rt);
public:
	Solver(Image image, int32 WH, int32 hdn, int32 vdn);
};

