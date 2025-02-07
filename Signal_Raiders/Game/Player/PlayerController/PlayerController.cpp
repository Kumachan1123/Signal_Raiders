/*
	@file	PlayerController.cpp
	@brief	�v���C���[�R���g���[���[�N���X
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
// �R���X�g���N�^
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
	, m_sensitive{ pPlayer->GetMouseSensitive() + 1.0f }// �}�E�X���x:Player->GetMouseSensitive()�̂܂܂��ƍŏ��l��0.0f�ɂȂ�̂� + 1.0f����
	, m_rotate{}
	, m_dashTime{ 10.0f }
{
	// �X�N���[���̉𑜓x���擾
	RECT desktopRect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktopRect);

	// �X�N���[���̒������W���v�Z
	int centerX = (desktopRect.right - desktopRect.left) / 2;
	int centerY = (desktopRect.bottom - desktopRect.top) / 2;

	// �}�E�X�ʒu�𒆉��ɐݒ�
	m_point = { centerX, centerY };
	SetCursorPos(m_point.x, m_point.y);

	m_lastMouseX = (float)(m_point.x);
	m_lastMouseY = (float)(m_point.y);
}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
PlayerController::~PlayerController()
{
}

void PlayerController::MoveStop()
{
	// �ړ��Ɖ�]���~�߂�
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
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	MoveStop();

	// �}�E�X�̌��݂̈ʒu���擾
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	int mouseX = point.x;
	int mouseY = point.y;

	// �}�E�X�̑��Έړ����v�Z
	float deltaX = static_cast<float>(mouseX - m_lastMouseX);
	float deltaY = static_cast<float>(mouseY - m_lastMouseY);

	// �}�E�X���x�̓K�p
	const float mouseSensitivity = 0.005f * m_sensitive;
	float x = deltaX * mouseSensitivity * 10.0f * elapsedTime;
	float y = deltaY * mouseSensitivity * 100.0f * elapsedTime;

	// ���E��]�iyaw�j
	m_yawX -= x;

	// �㉺��]�ipitch�j
	m_pitchY -= y;

	// �㉺�̉�]����
	constexpr float pitchLimit = XMConvertToRadians(80.0f);
	m_pitchY = Clamp(m_pitchY, -pitchLimit, pitchLimit);

	// �O��ړ�
	if (kbTracker->lastState.W)
	{
		// �J�����������Ă�������Ɉړ�����
		m_velocity.x += m_pPlayer->GetPlayerDir().x;
		m_velocity.z += m_pPlayer->GetPlayerDir().z;
	}
	if (kbTracker->lastState.S)
	{
		// �J�������t�����̕����Ɉړ�����
		m_velocity.x -= m_pPlayer->GetPlayerDir().x;
		m_velocity.z -= m_pPlayer->GetPlayerDir().z;
	}

	// �J�����̉E�����x�N�g�����v�Z����
	Vector3 rightDirection;
	rightDirection.x = m_pPlayer->GetPlayerDir().z;
	rightDirection.z = -m_pPlayer->GetPlayerDir().x;

	// ���E�ړ�
	if (kbTracker->lastState.A)
	{
		// �J�����̍������Ɉړ����� 
		m_velocity.x += rightDirection.x;
		m_velocity.z += rightDirection.z;
	}
	if (kbTracker->lastState.D)
	{
		// �J�����̉E�����Ɉړ�����
		m_velocity.x -= rightDirection.x;
		m_velocity.z -= rightDirection.z;
	}

	// �_�b�V��
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


	// �ړ��ʂ𐳋K������
	if (m_velocity.LengthSquared() > 0) m_velocity.Normalize();

	// �ړ��ʂ�␳����
	m_velocity *= m_dash * elapsedTime * 50;

	// ���ۂɈړ�����
	m_position += m_velocity;

	// ����]��ۑ�
	m_rotate = x;

	// �n�ʂ̍����ɐ���
	if (m_position.y <= 2.0f) m_position.y = 2.0f;

	// �n�ʂ̖ʐςɐ���
	if (m_position.x <= -98.5f) m_position.x = -98.5f;
	if (m_position.x >= 98.5f) m_position.x = 98.5f;
	if (m_position.z <= -98.5f) m_position.z = -98.5f;
	if (m_position.z >= 98.5f) m_position.z = 98.5f;
	// �}�E�X�ʒu�𒆉��ɖ߂�
	SetCursorPos(m_point.x, m_point.y);
}

void PlayerController::DebugCommand()
{
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾����
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();
	// �}�E�X�̃g���b�J�[���擾����
	auto& mTracker = m_commonResources->GetInputManager()->GetMouseTracker();
	// �E�N���b�N�œG����|
	if (mTracker->GetLastState().rightButton && m_pPlayer->GetisCheat() == false)
	{
		m_pPlayer->SetisCheat(true);
		for (auto& enemy : m_pPlayer->GetEnemies()->GetEnemies())enemy->SetEnemyHP(1000);
	}
	// �E�N���b�N����ĂȂ��Ƃ��̓`�[�g�R�}���h�𖳌��ɂ���
	if (!mTracker->GetLastState().rightButton)
	{
		m_pPlayer->SetisCheat(false);
	}
	// �X�y�[�X�L�[�Ńv���C���[��HP��0�ɂ���
	if (kbTracker->pressed.Space)m_pPlayer->SetPlayerHP(0.0f);

}

void PlayerController::Shoot()
{
	// �}�E�X�̃g���b�J�[���擾����
	auto& mTracker = m_commonResources->GetInputManager()->GetMouseTracker();

	// ���N���b�N�Œe����
	if (mTracker->GetLastState().leftButton && m_pPlayer->GetBulletManager()->GetIsPlayerShoot() == false)
	{
		m_pPlayer->CreateBullet();
	}
	if (!mTracker->GetLastState().leftButton)m_pPlayer->GetBulletManager()->SetIsPlayerShoot(false);

}
