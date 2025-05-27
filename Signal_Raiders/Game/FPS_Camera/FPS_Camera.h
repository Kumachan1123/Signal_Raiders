/*
*	@file	FPS_Camera.h
*	@brief	FPS�J�����N���X
*/
#pragma once
// DirectxX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "Game/Screen.h"
class FPS_Camera
{

public:
	// �A�N�Z�T
	const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_view; }// �r���[�s��擾
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return m_projection; }// �v���W�F�N�V�����s��擾
	const DirectX::SimpleMath::Vector3& GetEyePosition() const { return m_eye; }// �J�������W�擾
	void SetEyePositionY(float eye) { m_eye.y += eye; }// �J�������WY���ݒ�
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const { return m_target; }// �����_�擾
	void SetTargetPositionY(float target) { m_target.y += target; }// �����_Y���ݒ�
	const DirectX::SimpleMath::Vector3& GetUpVector() const { return m_up; } // �J�����̓��̕����擾
	DirectX::SimpleMath::Vector3 GetDirection() const; // �J�����̕����x�N�g���擾
public:
	//public�֐�
	FPS_Camera(// �R���X�g���N�^
		const DirectX::SimpleMath::Vector3& eye = DirectX::SimpleMath::Vector3{ 0.0f,4.0f,10.0f },
		const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3{ 0.0f,0.0f,0.0f },
		const DirectX::SimpleMath::Vector3& up = DirectX::SimpleMath::Vector3::UnitY
	);
	~FPS_Camera() = default;	// �f�X�g���N�^
	void Update(const DirectX::SimpleMath::Vector3& newEye, float pitch);// �X�V����
private:
	//private�֐�
	void CalculateViewMatrix();	// �r���[�s����v�Z����
	void CalculateProjectionMatrix();	// �v���W�F�N�V�����s����v�Z����
private:
	// private�萔
	// ��p
	const float FOV = DirectX::XMConvertToRadians(45.0f);
	// �߂����e��
	const float NEAR_PLANE = 1.0f;
	// �������e��
	const float FAR_PLANE = 1000.0f;
private:
	// private�ϐ�
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
	// ���[�A�s�b�`�i��]�p�x�j
	float m_yaw, m_pitch;
};
