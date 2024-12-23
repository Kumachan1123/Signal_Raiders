/*
	@file	ParticleUtility.h
	@brief	�p�[�e�B�N�����[�e�B���e�B�N���X
*/
#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
class ParticleUtility
{
public:
	enum class Type
	{
		ENEMYTRAIL,//	�G�̒e�̋O��
		PLAYERTRAIL,//	�v���C���[�̒e�̋O��
		BARRIERDESTROYED,//	�o���A�j��
	};
	//	�ϐ�


private:
	//	�֐�
	//	���W
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_startPosition;
	//	���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// ��]
	DirectX::SimpleMath::Vector3 m_rotate;
	//	�����x
	DirectX::SimpleMath::Vector3 m_accele;
	DirectX::SimpleMath::Vector3 m_rotateAccele;

	//	�X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//	��������
	float m_life;
	float m_startLife;

	// �o�ߎ���
	float m_elapsedTime;

	//	�J���[
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;

	//	�^�C�v
	Type m_type;

public:
	//	�֐�
	//	�R���X�g���N�^�i�����ݒ�j
	ParticleUtility(
		float life,//	��������
		DirectX::SimpleMath::Vector3 pos,//	���W
		DirectX::SimpleMath::Vector3 velocity,//	���x
		DirectX::SimpleMath::Vector3 accele,//	�����x
		DirectX::SimpleMath::Vector3 rotateAccele,// ��]�����x
		DirectX::SimpleMath::Vector3 rotate,// ��]
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,//	�X�P�[��(�ŏ��̑傫���A�Ō�̑傫��)
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,//	�J���[(�ŏ��̐F�A�Ō�̐F)
		Type type);//	�^�C�v
	//	�f�X�g���N�^
	~ParticleUtility();

	//	�X�V
	bool Update(float elapsedTime);

	//	getter
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }

	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }

	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }

	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }

	float GetLife() const { return m_life; }
	float GetStartLife() const { return m_startLife; }
	DirectX::SimpleMath::Color GetNowColor() const { return m_nowColor; }
	DirectX::SimpleMath::Color GetStartColor() const { return m_startColor; }
	DirectX::SimpleMath::Color GetEndColor() const { return m_endColor; }

private:
	void SwitchType(float elapsedTime);
	// �^�C�v���Ƃ̏���
	void Trail(float elapsedTime);// �e�̋O��
	void BarrierDestroyed(float elapsedTime);//	�o���A�j��
};