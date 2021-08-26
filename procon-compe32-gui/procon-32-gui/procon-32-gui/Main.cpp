# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include "TextureControl.h"

void Main()
{
	TextureControl tc;
	while (System::Update())
	{
		tc.checkSelectFlag();
		tc.showBoard();
	}
}