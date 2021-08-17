
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
    String pixelValue = U"";
    Array<String> headerData;
    Array<String> pixelData;

    // 行数の表示用のカウント
    size_t i = 0;

    // 終端に達するまで 1 行ずつ読み込む
    while (reader.readLine(line))
    {
        if (i > 5) {
            pixelData << line;
        }
        else {
            headerData << line;
        }
        i++;
    }
    for (const auto& pixelLine : pixelData) {
        pixelValue += pixelLine;
    }
    Array<String> diviNum = headerData[1].split(U' ');
    const int32 verDiviNum = Parse<int32>(diviNum[1]);
    const int32 horDiviNum = Parse<int32>(diviNum[2]);
    Array<String> seleLimNumStr = headerData[2].split(U' ');
    const int32 seleLimNum = Parse<int32>(seleLimNumStr[1]);
    Array<String> cost = headerData[3].split(U' ');
    const int32 selectCost = Parse<int32>(cost[1]);
    const int32 changeCost = Parse<int32>(cost[2]);
    Array<String> imageSize = headerData[4].split(U' ');
    const int32 imageWidth = Parse<int32>(imageSize[0]);
    const int32 imageHeight = Parse<int32>(imageSize[1]);
    Image image(imageWidth, imageHeight, Palette::White);
    Array<String> pixel = pixelValue.split(U' ');
    for (int32 y = 0; y < imageHeight; y++) {
        for (int32 x = 0; x < imageWidth; x++) {
            image[y][x] = Color(Parse<uint8>(pixel[(y * imageWidth + x) * 3]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 1]), Parse<uint8>(pixel[(y * imageWidth + x) * 3 + 2]));
        }
    }
    const Texture texture(image);
	while (System::Update())
	{
		// ボタンが押されたら
		if (SimpleGUI::Button(U"Read PPM File", Vec2(600, 20)))
		{
            for (const auto& line_data : headerData)
            {
                Print << U"{}"_fmt(line_data);
            }
		}
        texture.draw(10, 10);
	}
}