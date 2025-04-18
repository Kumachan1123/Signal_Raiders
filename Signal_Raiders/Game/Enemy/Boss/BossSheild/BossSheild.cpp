/*
	@file	BossSheild.cpp
	@brief	�{�X�V�[���h�N���X
*/
#include "pch.h"
#include "BossSheild.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*
*	@brief	�R���X�g���N�^
*	@param sheildHP �V�[���h�̑ϋv�l
*	@param pBoss �{�X�N���X�̃|�C���^
*	@return	�Ȃ�
*/
BossSheild::BossSheild(int sheildHP, Boss* pBoss)
	: m_commonResources{}// ���ʃ��\�[�X
	, m_isSheild(false)// �V�[���h�W�J�t���O
	, m_isParticle(false)// �p�[�e�B�N���Đ��t���O
	, m_sheildSize(Vector3::Zero)// �V�[���h�̃T�C�Y
	, m_sheildPosition(Vector3::Zero)// �V�[���h�̍��W
	, m_sheildHP(sheildHP)// �V�[���h��HP
	, m_pBoss(pBoss)// �{�X�N���X�̃|�C���^
{
}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
BossSheild::~BossSheild() {}

/*
*	@brief	������
*	@param resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void BossSheild::Initialize(CommonResources* resources)
{
	m_commonResources = resources;// ���ʃ��\�[�X��ݒ�
	auto device = resources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X�擾
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERBREAK, 0.0f);// 0.0f�Ȃ̂͂��łɓ����Őݒ肳��Ă��邩��
	m_pParticle->Initialize(m_commonResources);// �p�[�e�B�N��������
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);// �G�t�F�N�g�t�@�N�g���[�쐬
	fx->SetDirectory(L"Resources/Models/Boss");// �f�B���N�g���ݒ�
	m_sheildModel = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss_Barrier.cmo", *fx);// ���f���쐬
}

void BossSheild::Update(float elapsedTime)
{
	if (m_isSheild)// �V�[���h���W�J����Ă����
	{
		m_pBoss->PlayBarrierSE();// �V�[���hSE�Đ�
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3::One, EnemyParameters::BOSS_SHIELD_SCALE_SPEED);// �V�[���h�̃T�C�Y���g��
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::BOSS_SHIELD_RADIUS;// �{�X�̋��E�����V�[���h�̑傫���ɍ��킹��
		m_pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);// �U���̊Ԋu�𑬂�����
		m_pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);// ����U���̊Ԋu�𑬂�����
	}
	if (m_sheildHP <= 0)// �V�[���h���j�󂳂ꂽ��
	{
		m_isParticle = true;// �p�[�e�B�N���Đ�
		m_isSheild = false;// �V�[���h�j��
		m_pBoss->GetBoundingSphere().Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// �{�X�̋��E�������ɖ߂�
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// �{�X�̈ʒu��ݒ�
		m_pParticle->Update(elapsedTime);// �p�[�e�B�N���X�V
	}
}
/*
*	@brief	�`��
*	@param context �f�o�C�X�R���e�L�X�g
*	@param states �X�e�[�g
*	@param world ���[���h�s��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossSheild::Render(ID3D11DeviceContext1* context, DirectX::DX11::CommonStates* states, Matrix world, Matrix view, Matrix proj)
{
	if (m_isSheild)// �V�[���h���W�J����Ă����
	{
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;// �V�[���h�̃��[���h�s��
		m_sheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()// �V�[���h�`��
			{
				context->OMSetDepthStencilState(states->DepthRead(), 0);// �[�x�X�e���V���X�e�[�g�ݒ�
				context->RSSetState(states->CullClockwise());// ���X�^���C�U�[�X�e�[�g�ݒ�
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);// �u�����h�X�e�[�g�ݒ�
			});
	}
	if (m_isParticle)// �p�[�e�B�N���Đ��t���O�������Ă����
	{
		m_pParticle->CreateBillboard(m_pBoss->GetCameraTarget(), m_pBoss->GetCameraEye(), m_pBoss->GetCameraUp());// �r���{�[�h�쐬
		m_pParticle->Render(view, proj);// �p�[�e�B�N���`��
	}
}
