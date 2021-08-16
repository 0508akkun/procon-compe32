
# include <Siv3D.hpp> // OpenSiv3D v0.4.3

void Main()
{
    // ファイルをオープンする
    TextReader reader(U"../../../images/windmillnum.ppm");

    // オープンに失敗
    if (!reader)
    {
        throw Error(U"Failed to open `test.txt`");
    }

    // 行の内容を読み込む変数
    String line;

    Array<String> lines;

    // 行数の表示用のカウント
    size_t i = 0;

    // 終端に達するまで 1 行ずつ読み込む
    while (reader.readLine(line))
    {
        if (i > 5) {
            break;
        }
        lines << line;
        i++;
    }
    Array<String> diviNum = lines[1].split(U' ');
    const int32 verDiviNum = Parse<int32>(diviNum[1]);
    const int32 horDiviNum = Parse<int32>(diviNum[2]);
    Array<String> seleLimNumStr = lines[2].split(U' ');
    const int32 seleLimNum = Parse<int32>(seleLimNumStr[1]);
    Array<String> cost = lines[3].split(U' ');
    const int32 selectCost = Parse<int32>(cost[1]);
    const int32 changeCost = Parse<int32>(cost[2]);
    Array<String> imageSize = lines[4].split(U' ');
    const int32 width = Parse<int32>(imageSize[0]);
    const int32 height = Parse<int32>(imageSize[1]);
    Image image(width, height, Palette::White);
    DynamicTexture texture(image);

	while (System::Update())
	{
		// ボタンが押されたら
		if (SimpleGUI::Button(U"Read PPM File", Vec2(600, 20)))
		{
            for (const auto& line_data : lines)
            {
                Print << U"{}"_fmt(line_data);
            }
		}
        texture.draw(10, 10);
	}
}