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
#include <cmath> // �V�����ǉ������w�b�_
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
	DirectX::SimpleMath::Vector3 accele2,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;
	m_position = pos;
	m_velocity = velocity;
	m_accele = accele;
	m_accele2 = accele2;
	m_rotate = rotate;
	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;
	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;
	m_type = type;
}

ParticleUtility::~ParticleUtility()
{
}

/// <summary>
/// �X�V�֐�
/// </summary>
bool ParticleUtility::Update(float elapsedTime)
{
	switch (m_type)
	{
		case ParticleUtility::Type::SPIRAL:
			Spiral(elapsedTime);
			break;
		case ParticleUtility::Type::ONIBI:
			Onibi(elapsedTime);
			break;
		case ParticleUtility::Type::KAMEHAMEHA:
			Kamehameha(elapsedTime);
			break;
		case ParticleUtility::Type::ENEMYTRAIL:
			EnemyTrail(elapsedTime);
			break;
		case ParticleUtility::Type::PLAYERTRAIL:
			PlayerTrail(elapsedTime);
			break;
		default:
			break;
	}
	if (m_life < 0.0f)
	{
		return false;
	}
	return true;
}

// �^�C�v::�X�p�C�����̏���
void ParticleUtility::Spiral(float elapsedTime)
{
	// �X�P�[���ƐF�̕ω�
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// �����x�̓K�p
	if (m_life < 0.6f)
	{
		m_velocity += m_accele2 * elapsedTime;
	}
	else
	{
		m_velocity += m_accele * elapsedTime;
	}

	// �X�p�C���������̒ǉ�
	float spiralFactor = 0.1f; // �X�p�C�����̋���
	m_position.x += sin(m_life * 10.0f) * spiralFactor;
	m_position.y += cos(m_life * 10.0f) * spiralFactor;

	// �g�ł����̒ǉ�
	float waveFactor = 0.5f; // �g�̋���
	m_position.z += sin(m_life * 20.0f) * waveFactor;

	// ���W�X�V
	m_position += m_velocity * elapsedTime;

	// ��]�̓K�p
	m_rotate += SimpleMath::Vector3{ 45.0f, 0.0f, 0.0f } *elapsedTime;

	// ���C�t�̌���
	m_life -= elapsedTime;
}

// �^�C�v::�S�΂̏���
void ParticleUtility::Onibi(float elapsedTime)
{
	// �X�P�[���ƐF�̕ω������̔R���オ����ӎ����Đݒ�
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(
		SimpleMath::Vector4(1.0f, 0.2f, 0.1f, 1.0f),  // ���̊�i�ԁj
		SimpleMath::Vector4(1.0f, 1.0f, 0.1f, 0.5f),  // ���̐�[�i���F�j
		1.0f - m_life / m_startLife
	);

	// �����x��K�p���ď�����ɐ����悭�ړ�
	m_velocity += SimpleMath::Vector3(0.0f, 1.0f, 0.0f) * elapsedTime * 50.0f; // �㏸������
	m_velocity += m_accele * elapsedTime; // ���X�̉����x���K�p

	// ���W���X�V
	m_position += m_velocity * elapsedTime;

	// ��]�̓K�p�͉��̗h�炬���Č�
	m_rotate += SimpleMath::Vector3{ 45.0f, 15.0f, 0.0f } *elapsedTime; // �����h��銴��������

	// ���C�t������
	m_life -= elapsedTime;
}

// �^�C�v::���߂͂ߔg�̏���
void ParticleUtility::Kamehameha(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �c��̃��C�t�䗦
	float lifeRatio = m_life / m_startLife;

	// �����̏W�܂蓮��
	if (lifeRatio > 0.7f)
	{
		// �W�܂鑬�x�𒲐��i�W�܂�͂���������j
		SimpleMath::Vector3 gatherForce = -m_position * 0.05f;
		m_velocity += gatherForce * elapsedTime;
	}
	else
	{
		// �r�[�����ˌ�̑��x�𑝉�������
		m_velocity += m_accele * elapsedTime * 10.0f; // �r�[���̑��x�𑬂�����
	}

	// �T�C�Y�̊g�又�� (�r�[�������˂����Ƃ��ɃT�C�Y���傫���Ȃ�)
	if (lifeRatio < 0.5f)
	{
		m_nowScale = SimpleMath::Vector3::Lerp(m_nowScale, m_endScale * 3.0f, elapsedTime * 5.0f); // �T�C�Y��傫��
	}
	else
	{
		m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - lifeRatio);
	}

	// �F�̕ω� (�r�[�����ˎ��ɖ��邢�F�ɕς��)
	if (lifeRatio < 0.5f)
	{
		m_nowColor = SimpleMath::Color::Lerp(m_nowColor, Vector4(0, 0.5, 1, 10), elapsedTime * 2.0f); // �������ɕω�
	}
	else
	{
		m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - lifeRatio);
	}

	// ���W�̌v�Z
	m_position += m_velocity * elapsedTime;

	// ��]��������i�K�v�ɉ����Ē����\�j
	m_rotate += SimpleMath::Vector3{ 0.0f, 10.0f, 0.0f } *elapsedTime;

	// ���C�t�����炷
	m_life -= elapsedTime;
}

// �^�C�v::�G�̒e�̏���
void ParticleUtility::EnemyTrail(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �X�P�[���ƐF�̕ω�
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// �����x�̓K�p
	m_velocity += m_accele * elapsedTime;

	// ���W�X�V
	m_position += m_velocity * elapsedTime;

	// ���C�t�̌���
	m_life -= elapsedTime;
}

// �^�C�v::�O�Ղ̏���
void ParticleUtility::PlayerTrail(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �X�P�[���ƐF�̕ω�
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// �����x�̓K�p
	m_velocity += m_accele * elapsedTime;

	// ���W�X�V
	m_position += m_velocity * elapsedTime;

	// ���C�t�̌���
	m_life -= elapsedTime;
}