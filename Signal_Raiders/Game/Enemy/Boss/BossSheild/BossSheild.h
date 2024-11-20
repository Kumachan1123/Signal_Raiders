/*
	@file	BossSheild.h
	@brief	�{�X�V�[���h�N���X
*/
#pragma once
#ifndef BOSS_SHEILD_DEFINED
#define BOSS_SHEILD_DEFINED
#include "Game/CommonResources.h"
#include "Game/Interface/IState.h"
#include "Game/Particle/Particle.h"
#include "Game/Enemy/Boss/Boss.h"
class Boss;
class Player;
class CommonResources;
class IState;
class BossSheild
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �V�[���h���f��
	std::unique_ptr < DirectX::Model> m_sheildModel;
	std::unique_ptr<Particle> m_pParticle;// �p�[�e�B�N��
	Boss* m_pBoss;// �{�X
	DirectX::SimpleMath::Vector3 m_sheildSize;// �V�[���h�̃T�C�Y
	DirectX::SimpleMath::Vector3 m_sheildPosition;// �V�[���h�̍��W
	DirectX::SimpleMath::Quaternion m_sheildRotation;// �V�[���h�̉�]
	// �V�[���h�W�J�t���O
	bool m_isSheild;
	// �p�[�e�B�N���Đ��\�t���O
	bool m_isParticle;
	// �V�[���h��HP
	int m_sheildHP;

public:
	// �����X�e�[�^�X��ݒ�
	BossSheild(int sheildHP, Boss* pBoss);
	~BossSheild();
	void Initialize(CommonResources* resources);
	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext1* context,
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
public:
	// getter
	bool GetSheild() const { return m_isSheild; }
	int GetSheildHP() const { return m_sheildHP; }
	// setter
	void SetSheildHP(int sheildHP) { m_sheildHP = sheildHP; }
	void SetSheild(bool isSheild) { m_isSheild = isSheild; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_sheildPosition = pos; }
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_sheildRotation = rot; }
};
#endif //BOSS_SHEILD_DEFINED
