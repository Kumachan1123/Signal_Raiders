/*
	@file	PlayerController.cpp
	@brief	プレイヤーコントローラークラス
	作成者：くまち
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Player/PlayerController/PlayerController.h"
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
PlayerController::PlayerController()
	: m_position{}
	, m_velocity{}
	, m_yawX{}
	, m_beforeYawX{}
	, m_pitchY{}
	, m_dash{}
	, m_commonResources{}
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

	m_lastMouseX = m_point.x;
	m_lastMouseY = m_point.y;
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
	yawX = 0.0f;
	yawY = 0.0f;
	m_dash = 0.1f;
}

void PlayerController::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
}

void PlayerController::Update(const std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>& kb,
							  DirectX::SimpleMath::Vector3& Direction, float elapsedTime)
{
	MoveStop();
	ShowCursor(TRUE);//カーソルを見えないようにする
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
	const float mouseSensitivity = 0.04f;
	float x = deltaX * mouseSensitivity * 10.0f * elapsedTime;
	float y = deltaY * mouseSensitivity * 100.0f * elapsedTime;

	// 左右回転（yaw）
	m_yawX -= x;

	// 上下回転（pitch）
	m_pitchY -= y;

	// 上下の回転制限
	constexpr float pitchLimit = XMConvertToRadians(80.0f);
	m_pitchY = clamp(m_pitchY, -pitchLimit, pitchLimit);

	// 前後移動
	if (kb->lastState.W)
	{
		// カメラが向いている方向に移動する
		m_velocity.x += Direction.x;
		m_velocity.z += Direction.z;
	}
	if (kb->lastState.S)
	{
		// カメラが逆向きの方向に移動する
		m_velocity.x -= Direction.x;
		m_velocity.z -= Direction.z;
	}

	// カメラの右方向ベクトルを計算する
	Vector3 rightDirection;
	rightDirection.x = Direction.z;
	rightDirection.z = -Direction.x;

	// 左右移動
	if (kb->lastState.A)
	{
		// カメラの左方向に移動する 
		m_velocity.x += rightDirection.x;
		m_velocity.z += rightDirection.z;
	}
	if (kb->lastState.D)
	{
		// カメラの右方向に移動する
		m_velocity.x -= rightDirection.x;
		m_velocity.z -= rightDirection.z;
	}

	// ダッシュ
	if (kb->lastState.LeftShift)
	{
		m_dash = 0.3f;
	}


	// 移動量を正規化する
	if (m_velocity.LengthSquared() > 0) m_velocity.Normalize();

	// 移動量を補正する
	m_velocity *= m_dash;

	// 実際に移動する
	m_position += m_velocity;

	// 地面の高さに制限
	if (m_position.y <= 2.0f) m_position.y = 2.0f;

	// マウス位置を中央に戻す
	SetCursorPos(m_point.x, m_point.y);
}
