/*
*	@file    KumachiLib.h
*	@brief   ����̃��C�u�����̃w�b�_�[�t�@�C��
*	@details C++14�ŕW�����ڂ���Ă��Ȃ��֐��₻�̑��֗��Ȋ֐��̃e���v���[�g�Ȃǂ��܂Ƃ߂�����
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 
// �O�����C�u����
#include "Game/Screen.h"

/*
*	@brief   2D�x�N�g���̐��`���
*	@details 2D�x�N�g���̐��`��Ԃ��s���֐�
*	@param   start �J�n�_
*	@param   end �I���_
*	@param   t ��ԌW�� (0.0f <= t <= 1.0f)
*	@return  ��Ԃ��ꂽ�x�N�g��
*/
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	return start + t * (end - start);// ���`���
}
/*
*	@brief   2D�x�N�g���̃N�����v
*	@details 2D�x�N�g���̃N�����v���s���֐�
*	@param   value �N�����v����l
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �N�����v���ꂽ�l
*/
template <typename T>
inline T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;// �N�����v
}
/*
*	@brief   2D�x�N�g���̃X�t�F���J�����
*	@details 2D�x�N�g���̃X�t�F���J����Ԃ��s���֐�
*	@param   start �J�n�_
*	@param   end �I���_
*	@param   t ��ԌW�� (0.0f <= t <= 1.0f)
*	@return  ��Ԃ��ꂽ�x�N�g��
*/
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	float dot = start.Dot(end);// ����
	dot = Clamp(dot, -1.0f, 1.0f);// �N�����v
	float theta = std::acos(dot) * t;// �p�x
	T relative = end - start * dot;// ���΃x�N�g��
	relative.Normalize();// ���K��
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));// �X�t�F���J�����
}
/*
*	@brief   2D�x�N�g���̊p�x���v�Z
*	@details 2D�x�N�g���̊p�x���v�Z����֐�
*	@param   eye ���_
*	@param   target �^�[�Q�b�g
*	@return  �p�x
*/
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	DirectX::SimpleMath::Vector3 direction = target - eye;// �x�N�g���̍���
	direction.Normalize();// ���K��
	float angle = std::atan2(direction.x, direction.z);// �A�[�N�^���W�F���g
	return angle;// �p�x
}


// ����A������B�Ɍ������ē���(�����͖���)
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	DirectX::SimpleMath::Vector3 desired = B - A;// �ڕW�ʒu
	desired.Normalize();// ���K��
	desired *= maxSpeed;// �ő呬�x
	desired.y = 0;// �����𖳎�
	return desired;// �ڕW�ʒu
}

/*
*	@brief   �����^�p�̃����_���Ȕ{���𐶐�����
*	@details �����^�p�̃����_���Ȕ{���𐶐�����֐�
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �����_���Ȕ{��
*/
inline int GenerateRandomMultiplier(int min, int max)
{
	std::random_device rd;// ����������
	std::mt19937 gen(rd());// �����Z���k�E�c�C�X�^�@
	std::uniform_int_distribution<int> dis(min, max);// ��l���z
	return dis(gen);// �����_���Ȕ{���𐶐�
}

/*
*	@brief   ���������_�^�p�̃����_���Ȕ{���𐶐�����
*	@details ���������_�^�p�̃����_���Ȕ{���𐶐�����֐�
*	@param   min �ŏ��l
*	@param   max �ő�l
*	@return  �����_���Ȕ{��
*/
inline float GenerateRandomMultiplier(float min, float max)
{
	std::random_device rd;// ����������
	std::mt19937 gen(rd());// �����Z���k�E�c�C�X�^�@
	std::uniform_real_distribution<float> dis(min, max);// ��l���z
	return dis(gen);// �����_���Ȕ{���𐶐�
}

/*
*	@brief   ���̓��m���Փ˂����牟���߂�(���̓��m)
*	@details ���̓��m�̏Փ˔�����s���A�����߂��������s���֐�
*	@param   A ����A
*	@param   B ����B
*	@return  �����߂��ꂽ�ʒu
*/
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
{
	using namespace DirectX::SimpleMath;
	// �����߂�����
	Vector3 diffVector = A.Center - B.Center;	// �`�̒��S�Ƃa�̒��S�Ƃ̍����x�N�g���i�a����`�Ɍ������x�N�g���j�c�@
	float distance = diffVector.Length();	// �`�̒��S�Ƃa�̒��S�Ƃ̋����i�@�̒����j�c�A
	float sumRadius = A.Radius + B.Radius;	// �`�̔��a�Ƃa�̔��a�̍��v�c�B
	float penetrationDistance = sumRadius - distance;// �i�`���a�Ɂj�߂荞�񂾋����i�B�|�A�j�c�C
	diffVector.Normalize();// �@�𐳋K������c�D
	Vector3 pushBackVec = diffVector * penetrationDistance;// �����߂��x�N�g�����v�Z����i�D�ƇC�ŕ\������j�c�E
	Vector3 newPosition = A.Center + pushBackVec;// �E���g�p���āA�`�̍��W�Ƃ`�̃R���C�_�[���W���X�V����i���ۂɉ����߂��j
	A.Center = newPosition;// �`�̍��W���X�V����
	return newPosition;// �X�V���ꂽ�`�̍��W��Ԃ�
}
/*
*	@brief   �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E���Ƌ��E�{�b�N�X�̏ꍇ�j
*	@details ���E���Ƌ��E�{�b�N�X�̏Փ˔�����s���A�����߂��������s���֐�
*	@param   A ����A
*	@param   B �{�b�N�XB
*	@return  �����߂��ꂽ�ʒu
*/
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	Vector3 pushBackVec = Vector3::Zero;// �����߂��x�N�g�����v�Z
	Vector3 closestPoint; // �{�b�N�X�̍ł��߂��_
	// �e���ŃN�����v���āA�ł��߂��ʒu���擾
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));// X��
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));// Y��
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));// Z��
	Vector3 diffVector = A.Center - closestPoint;// ���̂̒��S�ƍł��߂��_�̃x�N�g����
	float distance = diffVector.Length();// �������v�Z
	if (distance < A.Radius)// ���������̂̔��a��菬�����ꍇ�͉����߂�����
	{
		float penetrationDistance = A.Radius - distance;// �����߂��ʂ��v�Z (���K�����ĉ����߂��x�N�g�����쐬)
		diffVector.Normalize();// ���K��
		pushBackVec = diffVector * penetrationDistance;// �����߂��x�N�g�����v�Z
		Vector3 newPosition = A.Center + pushBackVec; // �V�����ʒu���v�Z
		A.Center = newPosition; // ���̂̈ʒu���X�V
		return newPosition; // �X�V���ꂽ�ʒu��Ԃ�
	}
	return A.Center;// �Փ˂��Ă��Ȃ��ꍇ�͌��̈ʒu��Ԃ�
}

