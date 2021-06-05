
# include <Siv3D.hpp> // OpenSiv3D v0.4.3

Array<TextureRegion> LoadTextureRegions(const FilePath& path, const Size& size)
{
	const Texture texture(path);

	Array<TextureRegion> textureRegions;

	for (auto p : step(Size(texture.width() / size.x, texture.height() / size.y)))
	{
		textureRegions.push_back(texture(p * size, size));
	}

	return textureRegions;
}

void Main()
{
	Image image{ U"P3.ppm" };

	Texture texturePPM{ image };

	const Array<TextureRegion> textureRegions = LoadTextureRegions(U"P3.ppm", Size(40, 20));

	while (System::Update())
	{
		const Point pos = Cursor::Pos();
		const Color pixelColor = image[pos.y][pos.x];
		for (size_t i = 0; i < textureRegions.size(); ++i)
		{
			textureRegions[i].draw(42 * (i % 12), 22 * (i / 12));
		}
		Circle(400, 400, 50).draw(pixelColor);
	}
}

/*
	色の取得は
	const Color pixelColor = image[pos.y][pos.x];
*/

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
