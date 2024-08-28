/*
@file	FPS_Camera.h
@brief	FPS�J�����N���X
*/
#pragma once
#include <SimpleMath.h>

class FPS_Camera
{
private:
	// ��p
	const float FOV = DirectX::XMConvertToRadians(45.0f);

	// �߂����e��
	const float NEAR_PLANE = 1.0f;

	// �������e��
	const float FAR_PLANE = 1000.0f;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_projection;

	// �J�������W
	DirectX::SimpleMath::Vector3 m_eye;

	// �����_
	DirectX::SimpleMath::Vector3 m_target;

	// �J�����̓��̕���
	DirectX::SimpleMath::Vector3 m_up;

	float m_yaw, m_pitch;
public:
	// �R���X�g���N�^
	FPS_Camera(
		const DirectX::SimpleMath::Vector3& eye = DirectX::SimpleMath::Vector3{ 0.0f,2.0f,10.0f },
		const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3{ 0.0f,0.0f,0.0f },
		const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY


	);

	// �f�X�g���N�^
	~FPS_Camera() = default;

	// �X�V����
	void Update(const DirectX::SimpleMath::Vector3& newEye, float yawX);


	// getter
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }

	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }

	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }

	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }

	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; }

	// setter
	void SetTargetPositionY(float target) { m_target.y += target; }

	DirectX::SimpleMath::Vector3 GetDirection() const;
private:
	// �r���[�s����v�Z����
	void CalculateViewMatrix();
	// ���l�␳
	//float correctValue(float value);

	// �v���W�F�N�V�����s����v�Z����
	void CalculateProjectionMatrix();
};
