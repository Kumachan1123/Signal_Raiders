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
	CalculateViewMatrix();// �r���[�s��v�Z
	CalculateProjectionMatrix();// �v���W�F�N�V�����s��v�Z
}
/*
*	@brief �X�V����
*	@details �J�����̈ʒu�ƒ����_���X�V����
*	@param newEye	�V����eye���W
*	@param pitch	�s�b�`�p�x
*/
void FPS_Camera::Update(const DirectX::SimpleMath::Vector3& newEye, float pitch)
{
	DirectX::SimpleMath::Vector3 oldEye = m_eye;	// �Â�eye��ێ�����
	m_eye = newEye;	// eye�̈ʒu���X�V����
	DirectX::SimpleMath::Vector3 velocity = m_eye - oldEye;	// �ړ��x�N�g�������߂�
	m_target += velocity;	// target�̈ʒu���X�V����
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateRotationY(pitch); 	// ��]�s����쐬����
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;	// �J�����̕����x�N�g�����v�Z����
	direction = DirectX::SimpleMath::Vector3::Transform(direction, rotation);	// ��]�s����g���ĕ����x�N�g������]������
	m_target = m_eye + direction;	// ��]��̕����x�N�g�����g����target���X�V����
	CalculateViewMatrix();	// �r���[�s����X�V����
}
/*
*	@brief �r���[�s����v�Z����
*	@details �J�����̈ʒu�ƒ����_����Ƀr���[�s����v�Z����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void FPS_Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);// �r���[�s����v�Z����
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
	const float width = static_cast<float>(Screen::WIDTH);// �E�B���h�E��
	const float height = static_cast<float>(Screen::HEIGHT);// �E�B���h�E����
	const float aspectRatio = width / height;// �A�X�y�N�g��v�Z
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(// �v���W�F�N�V�����s����v�Z����
		FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);
}
/*
*	@brief �J�����̕����x�N�g�����擾����
*	@details �J�����̒����_�ƈʒu��������x�N�g�����v�Z���A���K�����ĕԂ�
*	@param �Ȃ�
*	@return �J�����̕����x�N�g��
*/
DirectX::SimpleMath::Vector3 FPS_Camera::GetDirection() const
{
	DirectX::SimpleMath::Vector3 direction = m_target - m_eye;// target��eye�̍���������x�N�g�����v�Z
	direction.Normalize();	// ���K��
	return direction;// �����x�N�g����Ԃ�
}

