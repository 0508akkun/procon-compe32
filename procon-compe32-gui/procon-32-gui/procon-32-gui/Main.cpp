
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

	while (System::Update())
	{
		// ボタンが押されたら
		if (SimpleGUI::Button(U"Read PPM File", Vec2(600, 20)))
		{
            for (const auto& line_data : lines)
            {
                Print << U"{}"_fmt(line_data);
            }
            Array<String> imageSize = lines[4].split(U' ');
            const int32 width = Parse<int32>(imageSize[0]);
            const int32 height = Parse<int32>(imageSize[1]);
            Print << width << height;
		}
	}
}