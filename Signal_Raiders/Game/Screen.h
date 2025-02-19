/*
	@file	Screen.h
	@brief	画面にまつわる定数をまとめたクラス
*/
#pragma once

class Screen
{
public:
	// UIにおける画面サイズ
	static const int UI_WIDTH = 1280;
	static const int UI_HEIGHT = 720;

	static const int UI_CENTER_X = UI_WIDTH / 2;
	static const int UI_CENTER_Y = UI_HEIGHT / 2;

	static const int UI_TOP = 0;
	static const int UI_BOTTOM = UI_HEIGHT;
	static const int UI_LEFT = 0;
	static const int UI_RIGHT = UI_WIDTH;


	// 画面サイズ
	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;

	static const int CENTER_X = WIDTH / 2;
	static const int CENTER_Y = HEIGHT / 2;

	static const int TOP = 0;
	static const int BOTTOM = HEIGHT;
	static const int LEFT = 0;
	static const int RIGHT = WIDTH;

	Screen() = default;
	~Screen() = default;
};
