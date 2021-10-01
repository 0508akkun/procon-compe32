# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include "TextureControl.h"

void Main()
{
	TextureControl tc;
	while (System::Update())
	{
		Window::Resize(Size(1280, 640));
		tc.checkSelectFlag();
		tc.showBoard();
		if (SimpleGUI::Button(U"画像読み込み", Vec2(1000, 100)))
		{
			Print << tc.getPieceID();
		}
		if (SimpleGUI::Button(U"ソルバの実行", Vec2(1000, 150)))
		{
			tc.setSolverData();
		}
		if (SimpleGUI::Button(U"テキスト出力", Vec2(1000, 200)))
		{
			tc.writeResultText();
		}
	}
}