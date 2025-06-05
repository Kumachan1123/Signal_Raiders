/*
*	@file	ParticleUtility.h
*	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#pragma once
#ifndef PARTICLEUTILITY_DEFINED
#define PARTICLEUTILITY_DEFINED
// �W�����C�u����
#include <vector>
#include <cmath>
#include <random>
// DirectX���C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>

// �p�[�e�B�N�����[�e�B���e�B�N���X
class ParticleUtility
{
public:
	// �񋓌^
	// �p�[�e�B�N���̎��
	enum class Type
	{
		ENEMYTRAIL,//	�G�̒e�̋O��
		PLAYERTRAIL,//	�v���C���[�̒e�̋O��
		BARRIERBREAK,//	�o���A�j��
	};
public:
	// �A�N�Z�T
	// ���W�擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// ���x�擾
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	// �����x�擾
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	// ���݂̃X�P�[���擾
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	// �J�n�X�P�[���擾
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	// �I���X�P�[���擾
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }
	// �������Ԏ擾
	float GetLife() const { return m_life; }
	// �J�n�������Ԏ擾
	float GetStartLife() const { return m_startLife; }
	// ���݂̐F�擾
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }
	// �J�n�F�擾
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; }
	// �I���F�擾
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; }
public:
	// public�֐�
	// �R���X�g���N�^�@
	ParticleUtility(
		float life,// ��������
		const DirectX::SimpleMath::Vector3& pos,//	���W
		const DirectX::SimpleMath::Vector3& velocity,// ���x
		const DirectX::SimpleMath::Vector3& accele,// �����x
		const DirectX::SimpleMath::Vector3& rotateAccele,// ��]�����x
		const DirectX::SimpleMath::Vector3& rotate,// ��]
		const DirectX::SimpleMath::Vector3& startScale, const DirectX::SimpleMath::Vector3& endScale,// �X�P�[��(�ŏ��̑傫���A�Ō�̑傫��)
		const DirectX::SimpleMath::Color& startColor, const DirectX::SimpleMath::Color& endColor,// �J���[(�ŏ��̐F�A�Ō�̐F)
		Type type);// �^�C�v
	// �f�X�g���N�^
	~ParticleUtility();
	// �X�V
	bool Update(float elapsedTime);
private:
	// private�֐�
	// �^�C�v�̐؂�ւ�
	void SwitchType(float elapsedTime);
	// �^�C�v���Ƃ̏���
	// �e�̋O��
	void Trail(float elapsedTime);
	//	�o���A�j��
	void BarrierDestroyed(float elapsedTime);
private:
	// private�ϐ�
	// ���W
	DirectX::SimpleMath::Vector3 m_position;
	// �����ʒu
	DirectX::SimpleMath::Vector3 m_startPosition;
	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// ��]
	DirectX::SimpleMath::Vector3 m_rotate;
	// �����x
	DirectX::SimpleMath::Vector3 m_accele;
	// ��]�����x
	DirectX::SimpleMath::Vector3 m_rotateAccele;
	// ���݂̃X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	// �J�n�X�P�[��
	DirectX::SimpleMath::Vector3 m_startScale;
	// �I���X�P�[��
	DirectX::SimpleMath::Vector3 m_endScale;
	// ��������
	float m_life;
	// �J�n��������
	float m_startLife;
	// �o�ߎ���
	float m_elapsedTime;
	// ���݂̐F
	DirectX::SimpleMath::Color m_nowColor;
	// �J�n�F
	DirectX::SimpleMath::Color m_startColor;
	// �I���F
	DirectX::SimpleMath::Color m_endColor;
	// �^�C�v
	Type m_type;
};
#endif // PARTICLEUTILITY_DEFINED