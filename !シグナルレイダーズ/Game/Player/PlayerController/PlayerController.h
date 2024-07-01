/*
	@file	PlayerController.h
	@brief	�v���C���[�R���g���[���[�N���X
	�쐬�ҁF���܂�
*/
#pragma once
//�O���錾
class CommonResources;
class Mouse;

class PlayerController
{
			//�ϐ�
public:
	DirectX::SimpleMath::Vector3 m_position;		// ��l�̎��_
	DirectX::SimpleMath::Vector3 m_velocity;		// �ړ��x�N�g��
	float m_yawX, m_pitchY;

	float m_lastMouseX;
	float m_lastMouseY;
	HWND m_hWnd;
	POINT m_point;
	// �J�������E��]
	float yawX = 0.0f;
	float m_beforeYawX;
	// �J�����㉺��]
	float yawY = 0.0f;
	// �_�b�V��
	float m_dash;
			//�֐�
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources; float DmouseX = 0;
public:
	PlayerController();
	~PlayerController();
	void Initialize(CommonResources* resources);
	void MoveStop();
	void Update(const std::unique_ptr < DirectX::Keyboard::KeyboardStateTracker>& kb, DirectX::SimpleMath::Vector3& Direction, float elapsedTime);
	
	//Getter
	float GetYawX()const { return m_yawX; }
	float GetYawY()const { return m_pitchY; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	float GetPlayerPositionY()const { return m_position.y; }
	//Setter
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
};

template <typename T>
T clamp(const T& value, const T& min, const T& max)
{
    return (value < min) ? min : (value > max) ? max : value;
}
