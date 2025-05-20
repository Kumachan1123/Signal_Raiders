/*
*	@file	ParticleUtility.h
*	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <cmath>
#include <random>
// DirectX���C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
class ParticleUtility
{
public:
	// �񋓌^
	enum class Type// �p�[�e�B�N���̎��
	{
		ENEMYTRAIL,//	�G�̒e�̋O��
		PLAYERTRAIL,//	�v���C���[�̒e�̋O��
		BARRIERBREAK,//	�o���A�j��
	};
public:
	// public�֐�
	ParticleUtility(	// �R���X�g���N�^�@
		float life,// ��������
		DirectX::SimpleMath::Vector3 pos,//	���W
		DirectX::SimpleMath::Vector3 velocity,// ���x
		DirectX::SimpleMath::Vector3 accele,// �����x
		DirectX::SimpleMath::Vector3 rotateAccele,// ��]�����x
		DirectX::SimpleMath::Vector3 rotate,// ��]
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,// �X�P�[��(�ŏ��̑傫���A�Ō�̑傫��)
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,// �J���[(�ŏ��̐F�A�Ō�̐F)
		Type type);// �^�C�v
	~ParticleUtility();// �f�X�g���N�^


	bool Update(float elapsedTime);// �X�V

	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// ���W�擾
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }// ���x�擾
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }// �����x�擾
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }// ���݂̃X�P�[���擾
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }// �J�n�X�P�[���擾
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }// �I���X�P�[���擾
	float GetLife() const { return m_life; }// �������Ԏ擾
	float GetStartLife() const { return m_startLife; } // �J�n�������Ԏ擾
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }// ���݂̐F�擾
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; } // �J�n�F�擾
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; } // �I���F�擾
private:
	// private�֐�
	void SwitchType(float elapsedTime);// �^�C�v�̐؂�ւ�
	// �^�C�v���Ƃ̏���
	void Trail(float elapsedTime);// �e�̋O��
	void BarrierDestroyed(float elapsedTime);//	�o���A�j��
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
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;// ���݂̃X�P�[��
	DirectX::SimpleMath::Vector3 m_startScale;// �J�n�X�P�[��
	DirectX::SimpleMath::Vector3 m_endScale;// �I���X�P�[��
	// ��������
	float m_life;
	// �J�n��������
	float m_startLife;
	// �o�ߎ���
	float m_elapsedTime;
	// �J���[
	DirectX::SimpleMath::Color m_nowColor;// ���݂̐F
	DirectX::SimpleMath::Color m_startColor;// �J�n�F
	DirectX::SimpleMath::Color m_endColor;// �I���F
	// �^�C�v
	Type m_type;
};