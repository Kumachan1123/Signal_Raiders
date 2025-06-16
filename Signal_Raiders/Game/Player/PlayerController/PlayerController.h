/*
*	@file	PlayerController.h
*	@brief	プレイヤーコントローラークラス
*/
#pragma once
#ifndef PLAYERCONTROLLER_DEFINED
#define PLAYERCONTROLLER_DEFINED
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectXライブラリ
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/MyMouse/MyMouse.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"

//前方宣言
class CommonResources;
class Mouse;
class Player;

// プレイヤーコントローラークラス
class PlayerController
{
public:
	// アクセサ
	// カメラのX軸回転取得
	float GetYaw()const { return m_yaw; }
	// カメラのY軸回転取得
	float GetPitch()const { return m_pitch; }
	// プレイヤーの位置取得
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	// プレイヤーの位置設定
	void SetPlayetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// プレイヤーの回転取得
	float GetRotate()const { return m_rotate; }
	// ダッシュ時間取得
	float GetDashTime()const { return m_dashTime; }
	// マウス感度設定
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }
public:
	// public関数
	// コンストラクタ
	PlayerController(Player* pPlayer);
	// デストラクタ
	~PlayerController();
	// 初期化
	void Initialize(CommonResources* resources);
	// 移動停止
	void MoveStop();
	// 更新
	void Update(float elapsedTime);
	// デバッグコマンド
	void DebugCommand();
	// プレイヤーが弾を撃つ
	void Shoot();
	// 弾をリロード（補充）する
	void Reload();
public:
	// 定数
	// 通常移動速度
	static const float MOVE_SPEED;
	// ダッシュ移動速度
	static const float DASH_SPEED;
	// ダッシュ時間
	static const float DASH_TIME;
	// 重力
	static const float GRAVITY;
	// ピッチの制限
	static const float PITCH_LIMIT;
	// マウス感度
	static const float MOUSE_SENSITIVE;
	// マウスX軸感度
	static const float MOUSE_X_SENSITIVE;
	// マウスY軸感度
	static const float MOUSE_Y_SENSITIVE;
private:
	// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 一人称視点カメラの座標
	DirectX::SimpleMath::Vector3 m_position;
	// プレイヤーの移動ベクトル
	DirectX::SimpleMath::Vector3 m_velocity;
	// カメラ回転
	float m_yaw, m_pitch;
	// pitchの制限
	float m_pitchLimit;
	// プレイヤー回転(ミニマップに渡す用)
	float m_rotate;
	// マウス感度
	float m_sensitive;
	// マウスのX座標
	float m_lastMouseX;
	// マウスのY座標
	float m_lastMouseY;
	// ウィンドウハンドル
	HWND m_hWnd;
	// マウス座標
	POINT m_point;
	// プレイヤー
	Player* m_pPlayer;
	// ダッシュ
	float m_dash;
	// 走れる時間
	float m_dashTime;
};
#endif // PLAYERCONTROLLER_DEFINED

