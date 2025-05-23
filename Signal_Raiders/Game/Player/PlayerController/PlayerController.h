/*
*	@file	PlayerController.h
*	@brief	プレイヤーコントローラークラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
#include <memory>
// DirectXライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"
//前方宣言
class CommonResources;
class Mouse;
class Player;
class PlayerController
{
public:
	// アクセサ
	float GetYaw()const { return m_yaw; }// カメラのX軸回転取得
	float GetPitch()const { return m_pitch; }// カメラのY軸回転取得
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }// プレイヤーの位置取得
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// プレイヤーの位置設定
	float GetRotate()const { return m_rotate; }// プレイヤーの回転取得
	float GetDashTime()const { return m_dashTime; }	// ダッシュ時間取得
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }	// マウス感度設定
	// public関数
	PlayerController(Player* pPlayer);// コンストラクタ
	~PlayerController();// デストラクタ
	void Initialize(CommonResources* resources);// 初期化
	void MoveStop();// 移動停止
	void Update(float elapsedTime);// 更新
	void DebugCommand();// デバッグコマンド
	void Shoot();// プレイヤーが弾を撃つ
	void Reload();// 弾をリロード（補充）する
public:// 定数
	static const float MOVE_SPEED;// 通常移動速度
	static const float DASH_SPEED;// ダッシュ移動速度
	static const float DASH_TIME;// ダッシュ時間
	static const float GRAVITY;// 重力
	static const float PITCH_LIMIT;// ピッチの制限
	static const float MOUSE_SENSITIVE;// マウス感度
	static const float MOUSE_X_SENSITIVE;// マウスX軸感度
	static const float MOUSE_Y_SENSITIVE;// マウスY軸感度

private:// private変数
	// 共通リソース
	CommonResources* m_commonResources;
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

