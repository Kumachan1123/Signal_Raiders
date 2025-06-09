/*
*	@file MyMouse.h
*	@brief InputManagerのみではできないマウスの入力を拡張するクラス
*/
#pragma once
// DirectX
#include <Mouse.h>

// 拡張マウス入力クラス
class MyMouse
{
public:
	// 関数
	// 左クリックが押された瞬間を取得
	static bool IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState);
public:
	// 変数
	// 前回のマウスボタンの状態
	static bool m_prevMouseButtonState;
};
