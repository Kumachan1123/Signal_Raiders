/*
*	@file	PlayerController.h
*	@brief	�v���C���[�R���g���[���[�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX���C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/CommonResources.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"
//�O���錾
class CommonResources;
class Mouse;
class Player;
class PlayerController
{
public:
	// �A�N�Z�T
	float GetYaw()const { return m_yaw; }// �J������X����]�擾
	float GetPitch()const { return m_pitch; }// �J������Y����]�擾
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }// �v���C���[�̈ʒu�擾
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }// �v���C���[�̈ʒu�ݒ�
	float GetRotate()const { return m_rotate; }// �v���C���[�̉�]�擾
	float GetDashTime()const { return m_dashTime; }	// �_�b�V�����Ԏ擾
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }	// �}�E�X���x�ݒ�
	// public�֐�
	PlayerController(Player* pPlayer);// �R���X�g���N�^
	~PlayerController();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void MoveStop();// �ړ���~
	void Update(float elapsedTime);// �X�V
	void DebugCommand();// �f�o�b�O�R�}���h
	void Shoot();// �v���C���[���e������
	void Reload();// �e�������[�h�i��[�j����
public:// �萔
	static const float MOVE_SPEED;// �ʏ�ړ����x
	static const float DASH_SPEED;// �_�b�V���ړ����x
	static const float DASH_TIME;// �_�b�V������
	static const float GRAVITY;// �d��
	static const float PITCH_LIMIT;// �s�b�`�̐���
	static const float MOUSE_SENSITIVE;// �}�E�X���x
	static const float MOUSE_X_SENSITIVE;// �}�E�XX�����x
	static const float MOUSE_Y_SENSITIVE;// �}�E�XY�����x

private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
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

