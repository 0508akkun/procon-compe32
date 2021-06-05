### 全体
- インデント : スペース4個
- procon30固有の変数名 : https://scrapbox.io/ict-science-club/競技29命名法

- コメントアウトを多用してください。
- 自分では分かるコードも他人からしてみればさっぱりです。

#### API の 書き方

ヘッダファイルに記述する関数名にはなるべくサマリーを書いてください。
`<remarks>`は書かなくてもいいですが、余力があるときは書いてください。

```api.h

/// <summary>
/// マルチバイト ASCII 文字列をワイド文字列に変換します。
/// </summary>
/// <param name="asciiStr">
/// ASCII 文字で構成されたマルチバイト文字列
/// </param>
/// <remarks>
/// Widen() 関数より高速に動作します。
/// </remarks>
/// <returns>
/// 変換されたワイド文字列
/// </returns>
String WidenAscii(CStringView asciiStr);

```


### C++

- https://scrapbox.io/ict-science-club/Siv3Dコーディングスタイル を参考にしてコーディングしてください。

- ヘッダファイルの拡張子は `.h `で統一します


### Python
読みやすく書いてください
参考にするサイトがあったら追記
