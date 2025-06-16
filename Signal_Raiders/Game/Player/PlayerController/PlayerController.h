/*
*	@file	PlayerController.h
*	@brief	�v���C���[�R���g���[���[�N���X
*/
#pragma once
#ifndef PLAYERCONTROLLER_DEFINED
#define PLAYERCONTROLLER_DEFINED
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX���C�u����
#include <SimpleMath.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/MyMouse/MyMouse.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"

//�O���錾
class CommonResources;
class Mouse;
class Player;

// �v���C���[�R���g���[���[�N���X
class PlayerController
{
public:
	// �A�N�Z�T
	// �J������X����]�擾
	float GetYaw()const { return m_yaw; }
	// �J������Y����]�擾
	float GetPitch()const { return m_pitch; }
	// �v���C���[�̈ʒu�擾
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	// �v���C���[�̈ʒu�ݒ�
	void SetPlayetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// �v���C���[�̉�]�擾
	float GetRotate()const { return m_rotate; }
	// �_�b�V�����Ԏ擾
	float GetDashTime()const { return m_dashTime; }
	// �}�E�X���x�ݒ�
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }
public:
	// public�֐�
	// �R���X�g���N�^
	PlayerController(Player* pPlayer);
	// �f�X�g���N�^
	~PlayerController();
	// ������
	void Initialize(CommonResources* resources);
	// �ړ���~
	void MoveStop();
	// �X�V
	void Update(float elapsedTime);
	// �f�o�b�O�R�}���h
	void DebugCommand();
	// �v���C���[���e������
	void Shoot();
	// �e�������[�h�i��[�j����
	void Reload();
public:
	// �萔
	// �ʏ�ړ����x
	static const float MOVE_SPEED;
	// �_�b�V���ړ����x
	static const float DASH_SPEED;
	// �_�b�V������
	static const float DASH_TIME;
	// �d��
	static const float GRAVITY;
	// �s�b�`�̐���
	static const float PITCH_LIMIT;
	// �}�E�X���x
	static const float MOUSE_SENSITIVE;
	// �}�E�XX�����x
	static const float MOUSE_X_SENSITIVE;
	// �}�E�XY�����x
	static const float MOUSE_Y_SENSITIVE;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ��l�̎��_�J�����̍��W
	DirectX::SimpleMath::Vector3 m_position;
	// �v���C���[�̈ړ��x�N�g��
	DirectX::SimpleMath::Vector3 m_velocity;
	// �J������]
	float m_yaw, m_pitch;
	// pitch�̐���
	float m_pitchLimit;
	// �v���C���[��](�~�j�}�b�v�ɓn���p)
	float m_rotate;
	// �}�E�X���x
	float m_sensitive;
	// �}�E�X��X���W
	float m_lastMouseX;
	// �}�E�X��Y���W
	float m_lastMouseY;
	// �E�B���h�E�n���h��
	HWND m_hWnd;
	// �}�E�X���W
	POINT m_point;
	// �v���C���[
	Player* m_pPlayer;
	// �_�b�V��
	float m_dash;
	// ����鎞��
	float m_dashTime;
};
#endif // PLAYERCONTROLLER_DEFINED

