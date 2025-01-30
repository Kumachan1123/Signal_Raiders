/*
	@file	BossSheild.cpp
	@brief	�{�X�V�[���h�N���X
*/
#include "pch.h"
#include "BossSheild.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include "Libraries/Microsoft/ReadData.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

BossSheild::BossSheild(int sheildHP, Boss* pBoss)
	: m_commonResources{}
	, m_isSheild{ false }
	, m_isParticle{ false }
	, m_sheildSize{ 0,0,0 }
	, m_sheildPosition{ 0,0,0 }
	, m_sheildHP{ sheildHP }// �V�[���h�̑ϋv�l
	, m_pBoss{ pBoss }

{
}

BossSheild::~BossSheild()
{
}

void BossSheild::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// �p�[�e�B�N������
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERDESTROYED, 0.0f);// 0.0f�Ȃ̂͂��łɓ����Őݒ肳��Ă��邩��
	m_pParticle->Initialize(m_commonResources);

	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// �V�[���h���f����ǂݍ���
	m_sheildModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Barrier.cmo", *fx);
}

void BossSheild::Update(float elapsedTime)
{
	if (m_isSheild)// �V�[���h���W�J����Ă����
	{
		m_pBoss->PlayBarrierSE();// �V�[���hSE�Đ�
		// �V�[���h���W�J����Ă���Ԃ̓T�C�Y���g��
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3::One, EnemyParameters::BOSS_SHIELD_SCALE_SPEED);
		// �{�X�̋��E����傫������
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::BOSS_SHIELD_RADIUS;
		// �U���̊Ԋu�𑬂�����
		m_pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);
	}
	// �V�[���h���j�󂳂ꂽ��p�[�e�B�N�����Đ�
	if (m_sheildHP <= 0)
	{
		m_isParticle = true;// �p�[�e�B�N���Đ�
		m_isSheild = false;// �V�[���h�j��
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// �{�X�̋��E�������ɖ߂�
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// �{�X�̈ʒu��ݒ�
		m_pParticle->Update(elapsedTime);// �p�[�e�B�N���X�V
	}
}

void BossSheild::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, Matrix world, Matrix view, Matrix proj)
{
	if (m_isSheild)
	{
		// �V�[���h�p�̃X�P�[���s���K�p
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;

		// �V�[���h
		m_sheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()
			{
				context->OMSetDepthStencilState(states->DepthRead(), 0);
				context->RSSetState(states->CullClockwise());
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);
			});
	}

	// �p�[�e�B�N���`��
	if (m_isParticle)
	{
		m_pParticle->CreateBillboard(m_pBoss->GetCameraTarget(), m_pBoss->GetCameraEye(), m_pBoss->GetCameraUp());
		m_pParticle->Render(view, proj);
	}
}
