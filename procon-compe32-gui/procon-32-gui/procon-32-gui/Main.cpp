# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include "TextureControl.h"

void Main()
{
	TextureControl tc;
	while (System::Update())
	{
		tc.checkSelectFlag();
		tc.showBoard();
		if (SimpleGUI::Button(U"ピースの出力", Vec2(550, 100)))
		{
			Print << tc.getPieceID();
		}
		if (SimpleGUI::Button(U"ソルバの実行", Vec2(550, 150)))
		{
			Print << U"BG";
			tc.setSolverData();
		}
	}
}