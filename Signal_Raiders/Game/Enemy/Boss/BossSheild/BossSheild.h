/*
*	@file	BossSheild.h
*	@brief	�{�X�V�[���h�N���X
*/
#pragma once
#ifndef BOSS_SHEILD_DEFINED
#define BOSS_SHEILD_DEFINED
// �W�����C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Interface/IState.h"
#include "Game/Particle/Particle.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Enemy/BossBase/BossBase.h"
// �N���X�̑O���錾
class IEnemy;
class Player;
class CommonResources;
class IState;
class BossBase;

class BossSheild
{
public:
	// �{�X�̎��
	enum class BossShieldType
	{
		BOSS = 0,	// �{�X
		LASTBOSS,	// ���X�{�X
	};
public:
	// �A�N�Z�T
	bool GetSheild() const { return m_isSheild; }// �V�[���h�W�J�t���O�擾
	void SetSheild(bool isSheild) { m_isSheild = isSheild; }// �V�[���h�W�J�t���O�ݒ�
	int GetSheildHP() const { return m_sheildHP; }// �V�[���h��HP�擾
	void SetSheildHP(int sheildHP) { m_sheildHP = sheildHP; }// �V�[���h��HP�ݒ�
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_sheildPosition = pos; }// �V�[���h�̍��W�ݒ�
	void SetRotation(DirectX::SimpleMath::Quaternion rot) { m_sheildRotation = rot; }// �V�[���h�̉�]�ݒ�
	void SetUp(int sheildHP, IEnemy* pBoss);// �V�[���h�̏�����

public:
	// public�֐�
	BossSheild();// �R���X�g���N�^
	~BossSheild();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void Update(float elapsedTime);// �X�V
	void Render(ID3D11DeviceContext1* context,// �`��
		DirectX::DX11::CommonStates* states,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj);
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �V�[���h���f��
	DirectX::Model* m_pSheildModel;
	// �p�[�e�B�N��
	std::unique_ptr<Particle> m_pParticle;
	// �{�X�|�C���^�[
	IEnemy* m_pBoss;
	// �{�X�̎��
	BossShieldType m_bossType;
	// �V�[���h�̃T�C�Y
	DirectX::SimpleMath::Vector3 m_sheildSize;
	// �V�[���h�̍��W
	DirectX::SimpleMath::Vector3 m_sheildPosition;
	// �V�[���h�̉�]
	DirectX::SimpleMath::Quaternion m_sheildRotation;
	// �V�[���h�W�J�t���O
	bool m_isSheild;
	// �p�[�e�B�N���Đ��\�t���O
	bool m_isParticle;
	// �V�[���h��HP
	int m_sheildHP;

};
#endif //BOSS_SHEILD_DEFINED
