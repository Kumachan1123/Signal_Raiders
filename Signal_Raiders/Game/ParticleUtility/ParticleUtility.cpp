/*
*	@file	ParticleUtility.cpp
*	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#include <pch.h>
#include "ParticleUtility.h"
/*
*	@brief	�R���X�g���N�^
*	@detail �p�[�e�B�N���̏��������s��
*	@param float life �p�[�e�B�N���̎���
*	@param DirectX::SimpleMath::Vector3 pos �p�[�e�B�N���̈ʒu
*	@param DirectX::SimpleMath::Vector3 velocity �p�[�e�B�N���̑��x
*	@param DirectX::SimpleMath::Vector3 accele �p�[�e�B�N���̉����x
*	@param DirectX::SimpleMath::Vector3 rotateAccele �p�[�e�B�N���̉�]�����x
*	@param DirectX::SimpleMath::Vector3 rotate �p�[�e�B�N���̉�]
*	@param DirectX::SimpleMath::Vector3 startScale �p�[�e�B�N���̏����X�P�[��
*	@param DirectX::SimpleMath::Vector3 endScale �p�[�e�B�N���̍ŏI�X�P�[��
*	@param DirectX::SimpleMath::Color startColor �p�[�e�B�N���̏����F
*	@param DirectX::SimpleMath::Color endColor �p�[�e�B�N���̍ŏI�F
*	@param Type type �p�[�e�B�N���̃^�C�v
*	@return �Ȃ�
*/
ParticleUtility::ParticleUtility(
	float life,
	const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& accele,
	const DirectX::SimpleMath::Vector3& rotateAccele,
	const DirectX::SimpleMath::Vector3& rotate,
	const DirectX::SimpleMath::Vector3& startScale,
	const DirectX::SimpleMath::Vector3& endScale,
	const DirectX::SimpleMath::Color& startColor,
	const DirectX::SimpleMath::Color& endColor,
	Type type)
{
	// ������ݒ�
	m_startLife = m_life = life;
	// ���W��ݒ�
	m_position = pos;
	// �������W��ݒ�
	m_startPosition = pos;
	// ���x��ݒ�
	m_velocity = velocity;
	// �����x��ݒ�
	m_accele = accele;
	// ��]�����x��ݒ�
	m_rotateAccele = rotateAccele;
	// ��]��ݒ�
	m_rotate = rotate;
	// ���݂̃X�P�[����ݒ�
	m_startScale = m_nowScale = startScale;
	// �ŏI�X�P�[����ݒ�
	m_endScale = endScale;
	// ���݂̐F��ݒ�
	m_startColor = m_nowColor = startColor;
	// �ŏI�F��ݒ�
	m_endColor = endColor;
	// �^�C�v��ݒ�
	m_type = type;
	// �o�ߎ��Ԃ�������
	m_elapsedTime = 0.0f;
}
/*
*	@brief	�f�X�g���N�^
*	@detail �������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
ParticleUtility::~ParticleUtility() {/* do nothing. */ }
/*
*	@brief	�p�[�e�B�N���̍X�V
*	@detail �p�[�e�B�N���̍X�V���s��
*	@param float elapsedTime �o�ߎ���
*	@return bool true:�܂��Đ����� false:�Đ����Ȃ�
*/
bool ParticleUtility::Update(float elapsedTime)
{
	using namespace DirectX;
	// �X�P�[���̕��
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// �F�̕��
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);
	// �����x�̓K�p�i�j�Ђ���юU������A�������ƌ����j
	m_velocity += m_accele * elapsedTime;
	// ���W�X�V
	m_position += m_velocity * elapsedTime;
	// ���C�t�̌���
	m_life -= elapsedTime;
	// �^�C�v�ɂ���ď�����ύX
	SwitchType(elapsedTime);
	// ���C�t��0�ȉ��ɂȂ�����false��Ԃ��A���C�t��0���傫���ꍇ��true��Ԃ�
	if (m_life < 0.0f)return false;
	else return true;
}
/*
*	@brief	�p�[�e�B�N���̎�ނ̐ؑ�
*	@detail �^�C�v�ɂ���ď�����ύX����
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ParticleUtility::SwitchType(float elapsedTime)
{
	// �^�C�v�ɂ���ď�����ύX
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		// �e�̋O�Ղ̏���
		Trail(elapsedTime);
		break;
	case ParticleUtility::Type::BARRIERBREAK:// �o���A�j��
		// �o���A�j��̏���
		BarrierDestroyed(elapsedTime);
		break;
	default:// ���̑�
		break;
	}
}

/*
*	@brief	�O�Ղ̏���
*	@detail �O�Ղ̍X�V����(�����ł͉������Ȃ�)
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ParticleUtility::Trail(float elapsedTime)
{
	// ���g�p�ϐ��̌x���𖳎�
	UNREFERENCED_PARAMETER(elapsedTime);
}
/*
*	@brief	�o���A�j��̏���
*	@detail �o���A�j��̍X�V����
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ParticleUtility::BarrierDestroyed(float elapsedTime)
{
	using namespace DirectX;
	// �V�[�h�l�𐶐�
	std::random_device seed;
	// �G���W����������
	std::default_random_engine engine(seed());
	// ���C�t�� 3/4 �����̎��A���ׂȃ����_��������ǉ�
	if (m_life < m_startLife * 0.75f)
	{
		// -3.0f ���� 3.0f �͈̔͂ŗ����𐶐�
		std::uniform_real_distribution<> dist(-3.0f, 3.0f);
		// X �������̑��x�ɉ��Z
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// Y �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// Z �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;
		// �������ʂ�K�p
		m_velocity *= 0.95f;
	}
	// ���C�t�� 1/2 �����̎��A�j�Ђ����}���Ɍ������A�����_���ȓ���������(�������߂̃����_���ȗ�)
	if (m_life < m_startLife * 0.5f)
	{
		// -5.0f ���� 5.0f �͈̔͂ŗ����𐶐�
		std::uniform_real_distribution<> dist(-5.0f, 5.0f);
		// X �������̑��x�ɉ��Z
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		// Y �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;
		// Z �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;
		// ���x������ɗ}����
		m_velocity *= 0.8f;
	}
	// ������ 1/2 �����̏ꍇ�ɉ�]��ǉ�
	if (m_life < m_startLife * 0.75f)
	{
		// ��]�p���x������i�����ɉ����ď��X�ɑ����Ȃ�j
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife);
		// ���_����̑��Έʒu���v�Z
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;
		// ��]�s����g�p���đ��Έʒu����]
		// �T�C���l���v�Z
		float sinTheta = std::sin(rotationSpeed * elapsedTime);
		// �R�T�C���l���v�Z
		float cosTheta = std::cos(rotationSpeed * elapsedTime);
		// ���Έʒu����]
		// �V����X���W
		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;
		// �V����Z���W
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta;
		// �X�V���ꂽ���Έʒu�����ɖ߂�
		// �V����X���W��ݒ�
		relativePos.x = newX;
		// �V����Z���W��ݒ�
		relativePos.z = newZ;
		// �V�����ʒu��ݒ�
		m_position = m_startPosition + relativePos;
	}
	// ���C�t�� 1/4 �����̎��A�j�Ђ��قƂ�Ǐ����钼�O�ɓ���������
	if (m_life < m_startLife * 0.25f)
	{
		// -10.0f ���� 10.0f �͈̔͂ŗ����𐶐�
		std::uniform_real_distribution<> dist(-10.0f, 10.0f);
		// X �������̑��x�ɉ��Z
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;
		// Y �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime;
		// Z �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;
		// ��u������������^����
		m_velocity *= 1.2f;
	}
}