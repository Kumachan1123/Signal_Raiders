//--------------------------------------------------------------------------------------
// File: ParticleUtil.h
//
// �p�[�e�B�N�����[�e�B���e�B�N���X
//
//-------------------------------------------------------------------------------------

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
		SPIRAL = 0,//	����
		KAMEHAMEHA,//	���߂͂ߔg
		ONIBI,//	�S��
		ENEMYTRAIL,//	�Ή�
		PLAYERTRAIL//	�O��
	};
	//	�ϐ�


private:
	//	�֐�
	//	���W
	DirectX::SimpleMath::Vector3 m_position;

	//	���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// ��]
	DirectX::SimpleMath::Vector3 m_rotate;
	//	�����x
	DirectX::SimpleMath::Vector3 m_accele;
	DirectX::SimpleMath::Vector3 m_accele2;

	//	�X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//	��������
	float m_life;
	float m_startLife;

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
		float life,
		DirectX::SimpleMath::Vector3 pos,
		DirectX::SimpleMath::Vector3 velocity,
		DirectX::SimpleMath::Vector3 accele,
		DirectX::SimpleMath::Vector3 accele2,
		DirectX::SimpleMath::Vector3 rotate,
		DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
		DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor,
		Type type);
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
	// �^�C�v���Ƃ̏���
	void Spiral(float elapsedTime);//	����
	void Onibi(float elapsedTime);//	�S��
	void Kamehameha(float elapsedTime);//	���߂͂ߔg
	void Trail(float elapsedTime);//	�Ή�

};