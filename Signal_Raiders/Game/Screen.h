/*
*	@file	Screen.h
*	@brief	画面にまつわる定数をまとめたクラス
*/
#pragma once
class Screen
{
public:// コンストラクタ・デストラクタ
	Screen() = default;
	~Screen() = default;
public:// 定数
	// UIにおける画面サイズ
	static const int UI_WIDTH = 1280;				// 幅
	static const int UI_HEIGHT = 720;				// 高さ

	static const int UI_CENTER_X = UI_WIDTH / 2;	// 中央X座標
	static const int UI_CENTER_Y = UI_HEIGHT / 2;	// 中央Y座標

	static const int UI_TOP = 0;					// 上端
	static const int UI_BOTTOM = UI_HEIGHT;			// 下端
	static const int UI_LEFT = 0;					// 左端
	static const int UI_RIGHT = UI_WIDTH;			// 右端


	// 画面サイズ
	static const int WIDTH = 1920;					// 幅
	static const int HEIGHT = 1080;					// 高さ

	static const int CENTER_X = WIDTH / 2;			// 中央X座標
	static const int CENTER_Y = HEIGHT / 2;			// 中央Y座標

	static const int TOP = 0;						// 上端
	static const int BOTTOM = HEIGHT;				// 下端
	static const int LEFT = 0;						// 左端
	static const int RIGHT = WIDTH;					// 右端
};
