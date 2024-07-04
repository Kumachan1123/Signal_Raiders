/*
	@file	PlayerController.cpp
	@brief	�v���C���[�R���g���[���[�N���X
	�쐬�ҁF���܂�
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
// �R���X�g���N�^
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

	m_lastMouseX = m_point.x;
	m_lastMouseY = m_point.y;
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
	ShowCursor(TRUE);//�J�[�\���������Ȃ��悤�ɂ���
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
	const float mouseSensitivity = 0.04f;
	float x = deltaX * mouseSensitivity * 10.0f * elapsedTime;
	float y = deltaY * mouseSensitivity * 100.0f * elapsedTime;

	// ���E��]�iyaw�j
	m_yawX -= x;

	// �㉺��]�ipitch�j
	m_pitchY -= y;

	// �㉺�̉�]����
	constexpr float pitchLimit = XMConvertToRadians(80.0f);
	m_pitchY = clamp(m_pitchY, -pitchLimit, pitchLimit);

	// �O��ړ�
	if (kb->lastState.W)
	{
		// �J�����������Ă�������Ɉړ�����
		m_velocity.x += Direction.x;
		m_velocity.z += Direction.z;
	}
	if (kb->lastState.S)
	{
		// �J�������t�����̕����Ɉړ�����
		m_velocity.x -= Direction.x;
		m_velocity.z -= Direction.z;
	}

	// �J�����̉E�����x�N�g�����v�Z����
	Vector3 rightDirection;
	rightDirection.x = Direction.z;
	rightDirection.z = -Direction.x;

	// ���E�ړ�
	if (kb->lastState.A)
	{
		// �J�����̍������Ɉړ����� 
		m_velocity.x += rightDirection.x;
		m_velocity.z += rightDirection.z;
	}
	if (kb->lastState.D)
	{
		// �J�����̉E�����Ɉړ�����
		m_velocity.x -= rightDirection.x;
		m_velocity.z -= rightDirection.z;
	}

	// �_�b�V��
	if (kb->lastState.LeftShift)
	{
		m_dash = 0.3f;
	}


	// �ړ��ʂ𐳋K������
	if (m_velocity.LengthSquared() > 0) m_velocity.Normalize();

	// �ړ��ʂ�␳����
	m_velocity *= m_dash;

	// ���ۂɈړ�����
	m_position += m_velocity;

	// �n�ʂ̍����ɐ���
	if (m_position.y <= 2.0f) m_position.y = 2.0f;

	// �}�E�X�ʒu�𒆉��ɖ߂�
	SetCursorPos(m_point.x, m_point.y);
}
