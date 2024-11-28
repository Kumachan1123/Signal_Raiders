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