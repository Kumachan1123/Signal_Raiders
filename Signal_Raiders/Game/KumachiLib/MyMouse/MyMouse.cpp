/*
*	@file MyMouse.cpp
*	@brief InputManagerのみではできないマウスの入力を拡張するクラス
*/
#include <pch.h>
#include "MyMouse.h"

// 前回のマウスボタンの状態を初期化
bool MyMouse::m_prevMouseButtonState = false;

/*
*	@brief 左クリックが押された瞬間を取得
*	@details マウスの状態を受け取り、左クリックが押された瞬間を判定する
*	@param currentState マウスの現在の状態
*	@return 左クリックが押された瞬間ならtrue、そうでなければfalse
*/
bool MyMouse::IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState)
{
	// 前回のマウスボタンの状態と現在の状態を比較して、左クリックが押された瞬間を判定する
	bool isPressed = false;
	// 左クリックが押されていて、前回の状態では押されていなかった場合
	if (currentState.leftButton && !m_prevMouseButtonState)
	{
		// 左クリックが押された瞬間と判定
		isPressed = true;
	}
	// 前回のマウスボタンの状態を更新
	m_prevMouseButtonState = currentState.leftButton;
	// 判定結果を返す
	return isPressed;
}
