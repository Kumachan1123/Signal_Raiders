/*
	@file	ParticleUtility.cpp
	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#include "pch.h"
#include "ParticleUtility.h"
#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <cmath>
#include <random>

using namespace DirectX;

const static float ENDLESS = -100.0f;

/// <summary>
/// �R���X�g���N�^
/// �����͏����ݒ�p�̐��l
/// </summary>
ParticleUtility::ParticleUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotateAccele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;
	m_position = pos;
	m_startPosition = pos;
	m_velocity = velocity;
	m_accele = accele;
	m_rotateAccele = rotateAccele;
	m_rotate = rotate;
	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;
	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;
	m_type = type;
	m_elapsedTime = 0.0f;
}

ParticleUtility::~ParticleUtility()
{
}

/// <summary>
/// �X�V�֐�
/// </summary>
bool ParticleUtility::Update(float elapsedTime)
{

	// �X�P�[���ƐF�̕ω�
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// �����x�̓K�p�i�j�Ђ���юU������A�������ƌ����j
	m_velocity += m_accele * elapsedTime;

	// ���W�X�V
	m_position += m_velocity * elapsedTime;

	// ���C�t�̌���
	m_life -= elapsedTime;

	// �^�C�v�ɂ���ď�����ύX
	SwitchType(elapsedTime);
	if (m_life < 0.0f)
	{
		return false;
	}
	return true;
}


void ParticleUtility::SwitchType(float elapsedTime)
{
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		Trail(elapsedTime);
		break;
	case ParticleUtility::Type::BARRIERDESTROYED:
		BarrierDestroyed(elapsedTime);
		break;
	default:
		break;
	}
}

// �^�C�v::�e�̋O�Ղ̏���
void ParticleUtility::Trail(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

}

void ParticleUtility::BarrierDestroyed(float elapsedTime)
{
	// �����_���ȓ�����ǉ����邽�߂̐ݒ�
	std::random_device seed;
	std::default_random_engine engine(seed());

	// ���C�t�� 3/4 �����̎��A���ׂȃ����_��������ǉ�
	if (m_life < m_startLife * 0.75f)
	{
		std::uniform_real_distribution<> dist(-3.0f, 3.0f); // �����_���ȗ͈͂̔�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;

		// �������ʂ�K�p
		m_velocity *= 0.95f; // ���x������
	}

	// ���C�t�� 1/2 �����̎��A�j�Ђ����}���Ɍ������A�����_���ȓ���������
	if (m_life < m_startLife * 0.5f)
	{
		std::uniform_real_distribution<> dist(-5.0f, 5.0f); // �������߂̃����_���ȗ�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;

		// ����������ɋ���
		m_velocity *= 0.8f; // ���x������ɗ}����
	}

	// ������ 1/2 �����̏ꍇ�ɉ�]��ǉ�
	if (m_life < m_startLife * 0.75f)
	{
		// ��]�p���x������i�����ɉ����ď��X�ɑ����Ȃ�j
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife); // ��]���x�������ɔ��

		// ���_����̑��Έʒu���v�Z
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;

		// ��]�����iY�����̉�]�j
		float sinTheta = std::sin(rotationSpeed * elapsedTime);
		float cosTheta = std::cos(rotationSpeed * elapsedTime);

		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta;

		// �X�V���ꂽ���Έʒu�����ɖ߂�
		relativePos.x = newX;
		relativePos.z = newZ;

		// �V�����ʒu��ݒ�
		m_position = m_startPosition + relativePos;
	}

	// ���C�t�� 1/4 �����̎��A�j�Ђ��قƂ�Ǐ����钼�O�ɓ���������
	if (m_life < m_startLife * 0.25f)
	{
		std::uniform_real_distribution<> dist(-10.0f, 10.0f); // �Ō�̌����������_���ȗ�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime;
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;

		// ��u������������^����
		m_velocity *= 1.2f; // ���x�𑝉�
	}
}

