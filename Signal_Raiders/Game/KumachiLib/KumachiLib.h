#pragma once
#include <SimpleMath.h>
#include <cmath>
#include <algorithm>
#include <random>
#include <type_traits> 

// ���`���
template <typename T>
inline T Lerp(const T& start, const T& end, float t)
{
	return start + t * (end - start);
}
// �͈͐���
template <typename T>
inline T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}
// �~�`���
template <typename T>
inline T Slerp(const T& start, const T& end, float t)
{
	float dot = start.Dot(end);
	dot = Clamp(dot, -1.0f, 1.0f);
	float theta = std::acos(dot) * t;
	T relative = end - start * dot;
	relative.Normalize();
	return ((start * std::cos(theta)) + (relative * std::sin(theta)));
}

// eye����target�ւ̊p�x���v�Z����
inline float CalculateAngle(const DirectX::SimpleMath::Vector3& eye,
	const DirectX::SimpleMath::Vector3& target)
{
	DirectX::SimpleMath::Vector3 direction = target - eye;
	direction.Normalize();
	float angle = std::atan2(direction.x, direction.z);
	return angle;
}


// ����A������B�Ɍ������ē���(�����͖���)
inline DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& A,
	const DirectX::SimpleMath::Vector3& B,
	float maxSpeed)
{
	DirectX::SimpleMath::Vector3 desired = B - A;
	desired.Normalize();
	desired *= maxSpeed;
	desired.y = 0;
	return desired;
}

// �����^�p�̃����_���Ȕ{���𐶐�����֐�
inline int GenerateRandomMultiplier(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

// ���������_�^�p�̃����_���Ȕ{���𐶐�����֐�
inline float GenerateRandomMultiplier(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E�����m�̏ꍇ�j
inline DirectX::SimpleMath::Vector3 CheckHitOtherObject(DirectX::BoundingSphere A, DirectX::BoundingSphere B)
{
	using namespace DirectX::SimpleMath;
	// �����߂�����
	// �`�̒��S�Ƃa�̒��S�Ƃ̍����x�N�g���i�a����`�Ɍ������x�N�g���j�c�@
	Vector3 diffVector = A.Center - B.Center;
	// �`�̒��S�Ƃa�̒��S�Ƃ̋����i�@�̒����j�c�A
	float distance = diffVector.Length();
	// �`�̔��a�Ƃa�̔��a�̍��v�c�B
	float sumRadius = A.Radius + B.Radius;
	// �i�`���a�Ɂj�߂荞�񂾋����i�B�|�A�j�c�C
	float penetrationDistance = sumRadius - distance;
	// �@�𐳋K������c�D
	diffVector.Normalize();
	// �����߂��x�N�g�����v�Z����i�D�ƇC�ŕ\������j�c�E
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// �E���g�p���āA�`�̍��W�Ƃ`�̃R���C�_�[���W���X�V����i���ۂɉ����߂��j
	Vector3 newPosition = A.Center + pushBackVec;
	A.Center = newPosition;
	return newPosition;
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E���Ƌ��E�{�b�N�X�̏ꍇ�j
inline DirectX::SimpleMath::Vector3 CheckHitWall(DirectX::BoundingSphere A, DirectX::BoundingBox& B)
{
	using namespace DirectX::SimpleMath;
	// �����߂��x�N�g�����v�Z
	Vector3 pushBackVec = Vector3::Zero;
	// ���̂̒��S�ƃ{�b�N�X�̃N�����v���ꂽ�ʒu�̍��������߂�
	Vector3 closestPoint; // �{�b�N�X�̍ł��߂��_

	// �e���ŃN�����v���āA�ł��߂��ʒu���擾
	closestPoint.x = std::max(B.Center.x - B.Extents.x, std::min(A.Center.x, B.Center.x + B.Extents.x));
	closestPoint.y = std::max(B.Center.y - B.Extents.y, std::min(A.Center.y, B.Center.y + B.Extents.y));
	closestPoint.z = std::max(B.Center.z - B.Extents.z, std::min(A.Center.z, B.Center.z + B.Extents.z));

	// ���̂̒��S�ƍł��߂��_�̃x�N�g����
	Vector3 diffVector = A.Center - closestPoint;

	// �������v�Z
	float distance = diffVector.Length();

	// ���������̂̔��a��菬�����ꍇ�͉����߂�����
	if (distance < A.Radius)
	{
		// �����߂��ʂ��v�Z (���K�����ĉ����߂��x�N�g�����쐬)
		float penetrationDistance = A.Radius - distance;
		diffVector.Normalize();
		pushBackVec = diffVector * penetrationDistance;
		Vector3 newPosition = A.Center + pushBackVec;
		A.Center = newPosition;
		return newPosition;
	}
	return A.Center;
}