/*
*	@file	ParticleUtility.cpp
*	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#include "pch.h"
#include "ParticleUtility.h"

const static float ENDLESS = -100.0f;// ������̒l

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
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotateAccele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
	Type type)
{
	m_startLife = m_life = life;// ������ݒ�
	m_position = pos;// ���W��ݒ�
	m_startPosition = pos;// �������W��ݒ�
	m_velocity = velocity;// ���x��ݒ�
	m_accele = accele;// �����x��ݒ�
	m_rotateAccele = rotateAccele;// ��]�����x��ݒ�
	m_rotate = rotate;// ��]��ݒ�
	m_startScale = m_nowScale = startScale;// ���݂̃X�P�[����ݒ�
	m_endScale = endScale;// �ŏI�X�P�[����ݒ�
	m_startColor = m_nowColor = startColor;// ���݂̐F��ݒ�
	m_endColor = endColor;// �ŏI�F��ݒ�
	m_type = type;// �^�C�v��ݒ�
	m_elapsedTime = 0.0f;// �o�ߎ��Ԃ�������
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
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);// �X�P�[���̕��
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);// �F�̕��
	m_velocity += m_accele * elapsedTime;// �����x�̓K�p�i�j�Ђ���юU������A�������ƌ����j
	m_position += m_velocity * elapsedTime;// ���W�X�V
	m_life -= elapsedTime;// ���C�t�̌���
	SwitchType(elapsedTime);// �^�C�v�ɂ���ď�����ύX
	if (m_life < 0.0f)return false;// ���C�t�� 0 �ȉ��ɂȂ����� false ��Ԃ�
	else return true;// ���C�t�� 0 ���傫���ꍇ�� true ��Ԃ�
}
/*
*	@brief	�p�[�e�B�N���̎�ނ̐ؑ�
*	@detail �^�C�v�ɂ���ď�����ύX����
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void ParticleUtility::SwitchType(float elapsedTime)
{
	switch (m_type)// �^�C�v�ɂ���ď�����ύX
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		Trail(elapsedTime);// �e�̋O�Ղ̏���
		break;
	case ParticleUtility::Type::BARRIERBREAK:// �o���A�j��
		BarrierDestroyed(elapsedTime);// �o���A�j��̏���
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
	UNREFERENCED_PARAMETER(elapsedTime);// ���g�p�ϐ��̌x���𖳎�
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
	std::random_device seed;// �V�[�h�l�𐶐�
	std::default_random_engine engine(seed());// �G���W����������
	if (m_life < m_startLife * 0.75f)// ���C�t�� 3/4 �����̎��A���ׂȃ����_��������ǉ�
	{
		// �����_���ȗ͈͂̔�
		std::uniform_real_distribution<> dist(-3.0f, 3.0f);// -3.0f ���� 3.0f �͈̔͂ŗ����𐶐�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;// X �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 0.5f;// Y �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime * 0.5f; // Z �������̑��x�ɉ��Z
		m_velocity *= 0.95f; // �������ʂ�K�p
	}
	if (m_life < m_startLife * 0.5f)// ���C�t�� 1/2 �����̎��A�j�Ђ����}���Ɍ������A�����_���ȓ���������(�������߂̃����_���ȗ�)
	{
		std::uniform_real_distribution<> dist(-5.0f, 5.0f);// -5.0f ���� 5.0f �͈̔͂ŗ����𐶐�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime;		 // X �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime * 2;	 // Y �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime;		 // Z �������̑��x�ɉ��Z
		m_velocity *= 0.8f; // ���x������ɗ}����
	}
	if (m_life < m_startLife * 0.75f)// ������ 1/2 �����̏ꍇ�ɉ�]��ǉ�
	{
		float rotationSpeed = 5.0f * (1.0f - m_life / m_startLife);// ��]�p���x������i�����ɉ����ď��X�ɑ����Ȃ�j
		SimpleMath::Vector3 relativePos = m_position - m_startPosition;// ���_����̑��Έʒu���v�Z
		// ��]�s����g�p���đ��Έʒu����]
		float sinTheta = std::sin(rotationSpeed * elapsedTime);// �T�C���l���v�Z
		float cosTheta = std::cos(rotationSpeed * elapsedTime); // �R�T�C���l���v�Z
		// ���Έʒu����]
		float newX = relativePos.x * cosTheta - relativePos.z * sinTheta;// �V���� X ���W
		float newZ = relativePos.x * sinTheta + relativePos.z * cosTheta; // �V���� Z ���W
		// �X�V���ꂽ���Έʒu�����ɖ߂�
		relativePos.x = newX; // �V���� X ���W��ݒ�
		relativePos.z = newZ; // �V���� Z ���W��ݒ�
		m_position = m_startPosition + relativePos;// �V�����ʒu��ݒ�
	}
	if (m_life < m_startLife * 0.25f)// ���C�t�� 1/4 �����̎��A�j�Ђ��قƂ�Ǐ����钼�O�ɓ���������
	{
		// �Ō�̌����������_���ȗ�
		std::uniform_real_distribution<> dist(-10.0f, 10.0f);// -10.0f ���� 10.0f �͈̔͂ŗ����𐶐�
		m_velocity.x += static_cast<float>(dist(engine)) * elapsedTime; // X �������̑��x�ɉ��Z
		m_velocity.y += static_cast<float>(dist(engine)) * elapsedTime; // Y �������̑��x�ɉ��Z
		m_velocity.z += static_cast<float>(dist(engine)) * elapsedTime; // Z �������̑��x�ɉ��Z
		m_velocity *= 1.2f;// ��u������������^����
	}
}