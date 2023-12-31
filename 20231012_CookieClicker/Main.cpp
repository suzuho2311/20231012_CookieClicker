﻿# include <Siv3D.hpp>

/// @brief アイテムのボタン
/// @param rect ボタンの領域
/// @param texture ボタンの絵文字
/// @param font 文字描画に使うフォント
/// @param name アイテムの名前
/// @param desc アイテムの説明
/// @param count アイテムの所持数
/// @param enabled ボタンを押せるか
/// @return ボタンが押された場合 true, それ以外の場合は false
bool Button(const Rect& rect, const Texture& texture, const Font& font, const String& name, const String& desc, int32 count, bool enabled)
{
	if (enabled)
	{
		rect.draw(ColorF{ 0.3, 0.5, 0.9, 0.8 });

		rect.drawFrame(2, 2, ColorF{ 0.5, 0.7, 1.0 });

		if (rect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}
	else
	{
		rect.draw(ColorF{ 0.0, 0.4 });

		rect.drawFrame(2, 2, ColorF{ 0.5 });
	}

	texture.scaled(0.5).drawAt(rect.x + 50, rect.y + 50);

	font(name).draw(30, rect.x + 100, rect.y + 15, Palette::White);

	font(desc).draw(18, rect.x + 102, rect.y + 60, Palette::White);

	font(count).draw(50, Arg::rightCenter((rect.x + rect.w - 20), (rect.y + 50)), Palette::White);

	return (enabled && rect.leftClicked());
}

void Main()
{
	// ケーキの絵文字
	const Texture texture{ U"🍰"_emoji };

	// 普通のケーキ屋の絵文字
	const Texture cake1Emoji{ U"🏠"_emoji };

	// おいしいケーキ屋の絵文字
	const Texture cake2Emoji{ U"🏡"_emoji };

	// 大人気のケーキ屋の絵文字
	const Texture cake3Emoji{ U"🏘"_emoji };

	// フォント
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// ケーキのクリック円
	const Circle cakeCircle{ 170, 300, 100 };

	// ケーキの表示サイズ（倍率）
	double cakeScale = 1.5;

	// ケーキの個数
	double cakes = 0;

	// 普通のケーキ屋の所有数
	int32 cake1Count = 0;

	// おいしいケーキ屋の所有数
	int32 cake2Count = 0;

	// 大人気のケーキ屋の所有数
	int32 cake3Count = 0;

	// 普通のケーキ屋の価格
	int32 cake1Cost = 10;

	// おいしいケーキ屋の価格
	int32 cake2Cost = 100;

	// 大人気のケーキ屋の価格
	int32 cake3Cost = 500;

	// ゲームの経過時間の蓄積
	double accumulatedTime = 0.0;

	while (System::Update())
	{
		// ケーキの毎秒の生産量 (cookies per second) を計算する
		const int32 cps = (cake1Count + (cake2Count * 10) + (cake3Count * 50));

		// ゲームの経過時間を加算する
		accumulatedTime += Scene::DeltaTime();

		// 0.1 秒以上蓄積していたら
		if (0.1 <= accumulatedTime)
		{
			accumulatedTime -= 0.1;

			// 0.1 秒分のケーキ生産を加算する
			cakes += (cps * 0.1);
		}

		// 普通のケーキ屋の価格を計算する
		cake1Cost = 10 + (cake1Count * 10);

		// おいしいケーキ屋の価格を計算する
		cake2Cost = 100 + (cake2Count * 100);

		// 大人気のケーキ屋の価格を計算する
		cake3Cost = 500 + (cake3Count * 500);

		// ケーキ円上にマウスカーソルがあれば
		if (cakeCircle.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		// ケーキ円が左クリックされたら
		if (cakeCircle.leftClicked())
		{
			cakeScale = 1.3;
			++cakes;
		}

		// ケーキの表示サイズを回復する
		cakeScale += Scene::DeltaTime();

		if (1.5 < cakeScale)
		{
			cakeScale = 1.5;
		}

		// 背景を描く
		Rect{ 0, 0, 800, 600 }.draw(Arg::top = ColorF{ 0.9, 0.5, 0.5 }, Arg::bottom = ColorF{ 1.0, 1.0, 1.0 });

		// ケーキの数を整数で表示する
		font(U"{:.0f}"_fmt(cakes)).drawAt(60, 170, 100);

		// ケーキの生産量を表示する
		font(U"毎秒: {}"_fmt(cps)).drawAt(24, 170, 160);

		// ケーキを描画する
		texture.scaled(cakeScale).drawAt(cakeCircle.center);

		// 普通のケーキ屋ボタン
		if (Button(Rect{ 340, 40, 420, 100 }, cake1Emoji, font, U"普通のケーキ屋", U"C{} / 1 CPS"_fmt(cake1Cost), cake1Count, (cake1Cost <= cakes)))
		{
			cakes -= cake1Cost;
			++cake1Count;
		}

		// おいしいケーキ屋ボタン
		if (Button(Rect{ 340, 160, 420, 100 }, cake2Emoji, font, U"おいしいケーキ屋", U"C{} / 10 CPS"_fmt(cake2Cost), cake2Count, (cake2Cost <= cakes)))
		{
			cakes -= cake2Cost;
			++cake2Count;
		}

		// 大人気のケーキ屋ボタン
		if (Button(Rect{ 340, 280, 420, 100 }, cake3Emoji, font, U"大人気のケーキ屋", U"C{} / 50 CPS"_fmt(cake3Cost), cake3Count, (cake3Cost <= cakes)))
		{
			cakes -= cake3Cost;
			++cake3Count;
		}
	}
}
