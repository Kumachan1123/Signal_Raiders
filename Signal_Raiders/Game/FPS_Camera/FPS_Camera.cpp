/*
*	@file	FPS_Camera.cpp
*	@brief	FPS�J�����N���X
*/
#include <pch.h>
#include "FPS_Camera.h"
/*
*	@brief	�R���X�g���N�^
*	@details FPS�J�����̏��������s��
*	@param   eye	�J�������W
*	@param   target	�����_
*	@param   up		�J�����̓��̕���
*/
FPS_Camera::FPS_Camera(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target,
	const DirectX::SimpleMath::Vector3& up)
	: m_view{}// �r���[�s��
	, m_projection{} // �v���W�F�N�V�����s��
	, m_eye{ eye }// �J�������W
	, m_target{ target }// �����_
	, m_up{ up }// �J�����̓��̕���
{
	// �r���[�s��v�Z
	CalculateViewMatrix();
	// �v���W�F�N�V�����s��v�Z
	CalculateProjectionMatrix();
}
/*
*	@brief �X�V����
*	@details �J�����̈ʒu�ƒ����_���X�V����
*	@param newEye	�V����eye���W
*	@param pitch	�s�b�`�p�x
*/
void FPS_Camera::Update(const DirectX::SimpleMath::Vector3& newEye, float pitch)
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
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationY(pitch);
	// �J�����̕����x�N�g�����v�Z����
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	// ��]�s����g���ĕ����x�N�g������]������
	direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);
	// ��]��̕����x�N�g�����g����target���X�V����
	m_target = m_eye + direction;
	// �r���[�s����X�V����
	CalculateViewMatrix();
}
/*
*	@brief �r���[�s����v�Z����
*	@details �J�����̈ʒu�ƒ����_����Ƀr���[�s����v�Z����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void FPS_Camera::CalculateViewMatrix()
{
	// �r���[�s����v�Z����
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}
/*
*	@brief �v���W�F�N�V�����s����v�Z����
*	@details �E�B���h�E�T�C�Y����Ƀv���W�F�N�V�����s����v�Z����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void FPS_Camera::CalculateProjectionMatrix()
{
	// �E�B���h�E�T�C�Y���擾
	// �E�B���h�E��
	const float width = static_cast<float>(Screen::WIDTH);
	// �E�B���h�E����
	const float height = static_cast<float>(Screen::HEIGHT);
	// �A�X�y�N�g����v�Z
	const float aspectRatio = width / height;
	// �v���W�F�N�V�����s����v�Z����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
}
/*
*	@brief �J�����̕����x�N�g�����擾����
*	@details �J�����̒����_�ƈʒu��������x�N�g�����v�Z���A���K�����ĕԂ�
*	@param �Ȃ�
*	@return �J�����̕����x�N�g��
*/
DirectX::SimpleMath::Vector3 FPS_Camera::GetDirection() const
{
	// target��eye�̍���������x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;
	// �����x�N�g���𐳋K��
	direction.Normalize();
	// �����x�N�g����Ԃ�
	return direction;
}

