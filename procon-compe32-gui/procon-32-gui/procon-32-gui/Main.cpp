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
		if (SimpleGUI::Button(U"画像読み込み", Vec2(1100, 200)))
		{
			tc = TextureControl(0);
		}
		if (SimpleGUI::Button(U"ソルバの実行", Vec2(1100, 250)))
		{
			tc.setSolverData();
		}
		if (SimpleGUI::Button(U"テキスト出力", Vec2(1100, 300)))
		{
			tc.writeResultText();
		}
	}
}