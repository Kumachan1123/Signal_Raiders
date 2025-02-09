/*
	@file	PlayerController.h
	@brief	�v���C���[�R���g���[���[�N���X
*/
#pragma once
#include "Game/Player/Player.h"
//�O���錾
class CommonResources;
class Mouse;
class Player;
class PlayerController
{
	//�ϐ�
public:
	DirectX::SimpleMath::Vector3 m_position;		// ��l�̎��_
	DirectX::SimpleMath::Vector3 m_velocity;		// �ړ��x�N�g��
	float m_yawX, m_pitchY;						// �J������]
	float m_rotate;								// �v���C���[��](�~�j�}�b�v�ɓn���p)
	float m_sensitive;							// �}�E�X���x
	float m_lastMouseX;
	float m_lastMouseY;
	HWND m_hWnd;
	POINT m_point;
	// �v���C���[
	Player* m_pPlayer;

	// �_�b�V��
	float m_dash;

	// ����鎞��
	float m_dashTime;

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	float DmouseX = 0;
public:
	PlayerController(Player* pPlayer);
	~PlayerController();
	void Initialize(CommonResources* resources);
	void MoveStop();
	void Update(float elapsedTime);// �X�V
	void DebugCommand();// �f�o�b�O�R�}���h
	void Shoot();// �v���C���[���e������
	void Reload();// �e�������[�h�i��[�j����
	//Getter
	float GetYawX()const { return m_yawX; }
	float GetPitch()const { return m_pitchY; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	float GetRotate()const { return m_rotate; }
	float GetDashTime()const { return m_dashTime; }
	//Setter
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }
};

