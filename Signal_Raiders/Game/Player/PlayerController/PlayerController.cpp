/*
	@file	PlayerController.cpp
	@brief	プレイヤーコントローラークラス
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Player/PlayerController/PlayerController.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <memory>

using namespace DirectX;
using namespace DirectX::SimpleMath;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
PlayerController::PlayerController(Player* pPlayer)
	: m_pPlayer{ pPlayer }
	, m_position{}
	, m_velocity{}
	, m_yawX{}
	, m_pitchY{}
	, m_dash{}
	, m_commonResources{}
	, m_hWnd{ nullptr }
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// マウス感度:Player->GetMouseSensitive()のままだと最小値が0.0fになるので + 1.0fする
	, m_rotate{}
	, m_dashTime{ 10.0f }
{
	// スクリーンの解像度を取得
	RECT desktopRect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktopRect);

	// スクリーンの中央座標を計算
	int centerX = (desktopRect.right - desktopRect.left) / 2;
	int centerY = (desktopRect.bottom - desktopRect.top) / 2;

	// マウス位置を中央に設定
	m_point = { centerX, centerY };
	SetCursorPos(m_point.x, m_point.y);

	m_lastMouseX = (float)(m_point.x);
	m_lastMouseY = (float)(m_point.y);
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
PlayerController::~PlayerController()
{
}

void PlayerController::MoveStop()
{
	// 移動と回転を止める
	m_velocity = Vector3::Zero;
	m_velocity.y -= 0.75;
	m_yawX = 0.0f;
	m_pitchY = 0.0f;

	m_dash = 0.1f;
}

void PlayerController::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
}

void PlayerController::Update(float elapsedTime)
{
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	MoveStop();

	// マウスの現在の位置を取得
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	int mouseX = point.x;
	int mouseY = point.y;

	// マウスの相対移動を計算
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);

	// マウス感度の適用
	const float mouseSensitivity = 0.005f * m_sensitive;
	float x = deltaX * mouseSensitivity * 10.0f * elapsedTime;
	float y = deltaY * mouseSensitivity * 100.0f * elapsedTime;

	// 左右回転（yaw）
	m_yawX -= x;

	// 上下回転（pitch）
	m_pitchY -= y;

	// 上下の回転制限
	constexpr float pitchLimit = XMConvertToRadians(80.0f);
	m_pitchY = Clamp(m_pitchY, -pitchLimit, pitchLimit);

	// 前後移動
	if (kbTracker->lastState.W)
	{
		// カメラが向いている方向に移動する
		m_velocity.x += m_pPlayer->GetPlayerDir().x;
		m_velocity.z += m_pPlayer->GetPlayerDir().z;
	}
	if (kbTracker->lastState.S)
	{
		// カメラが逆向きの方向に移動する
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;
	}

	// カメラの右方向ベクトルを計算する
	Vector3 rightDirection;
	rightDirection.x = m_pPlayer->GetPlayerDir().z;
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;

	// 左右移動
	if (kbTracker->lastState.A)
	{
		// カメラの左方向に移動する 
		m_velocity.x += rightDirection.x;
		m_velocity.z += rightDirection.z;
	}
	if (kbTracker->lastState.D)
	{
		// カメラの右方向に移動する
		m_velocity.x -= rightDirection.x;
		m_velocity.z -= rightDirection.z;
	}

	// ダッシュ
	if (kbTracker->lastState.LeftShift && m_dashTime > 0.0f)
	{
		m_dash = 0.2f;
		m_dashTime -= elapsedTime;
	}
	else
	{
		m_dash = 0.1f;
		m_dashTime += elapsedTime / 2;
		if (m_dashTime >= 10.0f)m_dashTime = 10.0f;
	}


	// 移動量を正規化する
	if (m_velocity.LengthSquared() > 0) m_velocity.Normalize();

	// 移動量を補正する
	m_velocity *= m_dash * elapsedTime * 50;

	// 実際に移動する
	m_position += m_velocity;

	// 横回転を保存
	m_rotate = x;

	// 地面の高さに制限
	if (m_position.y <= 2.0f) m_position.y = 2.0f;

	// 地面の面積に制限
	if (m_position.x <= -98.5f) m_position.x = -98.5f;
	if (m_position.x >= 98.5f) m_position.x = 98.5f;
	if (m_position.z <= -98.5f) m_position.z = -98.5f;
	if (m_position.z >= 98.5f) m_position.z = 98.5f;
	// マウス位置を中央に戻す
	SetCursorPos(m_point.x, m_point.y);
}

void PlayerController::DebugCommand()
{
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// マウスのトラッカーを取得する
	auto& mTracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// 右クリックで敵を一掃
	if (mTracker->GetLastState().rightButton && m_pPlayer->GetisCheat() == false)
	{
		m_pPlayer->SetisCheat(true);
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->SetEnemyHP(1000);
	}
	// 右クリックされてないときはチートコマンドを無効にする
	if (!mTracker->GetLastState().rightButton)
	{
		m_pPlayer->SetisCheat(false);
	}
	// スペースキーでプレイヤーのHPを0にする
	if (kbTracker->pressed.Space)m_pPlayer->SetPlayerHP(0.0f);

}

void PlayerController::Shoot()
{
	// マウスのトラッカーを取得する
	auto& mTracker = m_commonResources->GetInputManager()->GetMouseTracker();

	// 左クリックで弾発射
	if (mTracker->GetLastState().leftButton && m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false)
	{
		m_pPlayer->CreateBullet();
	}
	if (!mTracker->GetLastState().leftButton)m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);

}
