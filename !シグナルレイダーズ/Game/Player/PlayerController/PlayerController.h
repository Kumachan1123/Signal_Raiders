/*
	@file	PlayerController.h
	@brief	�v���C���[�R���g���[���[�N���X
	�쐬�ҁF���܂�
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

private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources; float DmouseX = 0;
public:
	PlayerController(Player* pPlayer);
	~PlayerController();
	void Initialize(CommonResources* resources);
	void MoveStop();
	void Update(const std::unique_ptr < DirectX::Keyboard::KeyboardStateTracker>& kb, DirectX::SimpleMath::Vector3& Direction, float elapsedTime);

	//Getter
	float GetYawX()const { return m_yawX; }
	float GetPitch()const { return m_pitchY; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	float GetRotate()const { return m_rotate; }
	//Setter
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }
};

template <typename T>
T clamp(const T& value, const T& min, const T& max)
{
	return (value < min) ? min : (value > max) ? max : value;
}
