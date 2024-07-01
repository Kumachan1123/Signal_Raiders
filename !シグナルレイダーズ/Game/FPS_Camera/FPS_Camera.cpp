/*
	@file	FPS_Camera.cpp
	@brief	FPS�J�����N���X
*/
#include "pch.h"
#include "FPS_Camera.h"
#include "Game/Screen.h"

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
FPS_Camera::FPS_Camera(
	const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target,
	const DirectX::SimpleMath::Vector3& up
)
	:
	m_view{},
	m_projection{},
	m_eye{ eye },
	m_target{ target },
	m_up{ up }
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void FPS_Camera::Update(const DirectX::SimpleMath::Vector3& newEye, float yawX)
{
	
	// �Â�eye��ێ�����
	DirectX::SimpleMath::Vector3 oldEye = m_eye;
	// eye�̈ʒu���X�V����
	m_eye = newEye;
	// �ړ��x�N�g�������߂�
	DirectX::SimpleMath::Vector3 velocity = m_eye - oldEye;
	// target�̈ʒu���X�V����
	m_target += velocity;

	// ��]�s����쐬����
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationY(yawX);
	// �J�����̕����x�N�g�����v�Z����
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	// ��]�s����g���ĕ����x�N�g������]������
	direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);
	// ��]��̕����x�N�g�����g����target���X�V����
	m_target = m_eye + direction;
	// �r���[�s����X�V����
	CalculateViewMatrix();
}

//-------------------------------------------------------------------
// �r���[�s����v�Z����
//-------------------------------------------------------------------
void FPS_Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

//-------------------------------------------------------------------
// �v���W�F�N�V�����s����v�Z����
//-------------------------------------------------------------------
void FPS_Camera::CalculateProjectionMatrix()
{
	// �E�B���h�E�T�C�Y
	const float width = static_cast<float>(Screen::WIDTH);
	const float height = static_cast<float>(Screen::HEIGHT);
	// ��ʏc����
	const float aspectRatio = width / height;
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
}


DirectX::SimpleMath::Vector3 FPS_Camera::GetDirection() const
{
	// target��eye�̍���������x�N�g�����v�Z���A���K�����ĕԂ�
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	direction.Normalize();
	return direction;
}

