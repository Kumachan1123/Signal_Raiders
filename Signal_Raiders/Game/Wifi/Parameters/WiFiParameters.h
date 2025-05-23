/*
*	@file	WiFiParameters.h
*	@brief	Wi-Fiに関するパラメータを定義するヘッダーファイル
*/
#pragma once
class WiFiParameters
{
public:// public定数
	// Wi-Fiを取得できない場合の敵の総数
	static constexpr int MAX_ENEMY = 30;
	// Wi-Fiを取得できない場合の敵の体力
	static constexpr int DEFAULT_ENEMY_HP = 100;
	// Wi-Fiを取得できない場合の敵の種類
	static constexpr int DEFAULT_ENEMY_TYPE = 0;
	// 更新を終える時間
	static constexpr float MAX_TIME = 5.0f;
};