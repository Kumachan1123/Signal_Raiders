/*
	@file	BossSheild.cpp
	@brief	�{�X�V�[���h�N���X
*/
#include "pch.h"
#include "BossSheild.h"

/*
*	@brief	�R���X�g���N�^
*	@return	�Ȃ�
*/
BossSheild::BossSheild()
	: m_commonResources{}// ���ʃ��\�[�X
	, m_isSheild(false)// �V�[���h�W�J�t���O
	, m_isParticle(false)// �p�[�e�B�N���Đ��t���O
	, m_sheildSize(DirectX::SimpleMath::Vector3::Zero)// �V�[���h�̃T�C�Y
	, m_sheildPosition(DirectX::SimpleMath::Vector3::Zero)// �V�[���h�̍��W
	, m_sheildHP(0)// �V�[���h��HP
	, m_bossType(BossShieldType::BOSS)// �{�X�̎��
	, m_pBoss(nullptr)// �{�X�N���X�̃|�C���^
{
}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
BossSheild::~BossSheild() {}

/*
*	@brief	�V�[���h�̏�����
*	@param[in] sheildHP �V�[���h��HP
*	@param[in] pBoss �{�X�N���X�̃|�C���^
*	@return	�Ȃ�
*/
void BossSheild::SetUp(int sheildHP, IEnemy* pBoss)
{
	using namespace DirectX::SimpleMath;
	m_sheildHP = sheildHP;// �V�[���h��HP
	m_pBoss = pBoss;// �{�X�N���X�̃|�C���^
	m_isSheild = false;// �V�[���h�W�J�t���O�𗧂Ă�
	m_sheildSize = Vector3::Zero;// �V�[���h�̃T�C�Y��������
	m_sheildPosition = pBoss->GetPosition();// �{�X�̈ʒu���V�[���h�̈ʒu�ɐݒ�
}

/*
*	@brief	������
*	@param[in] resources ���ʃ��\�[�X
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

/*
*	@brief	�X�V
*	@param[in] elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void BossSheild::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);// �{�X�N���X�̃|�C���^���擾
	if (m_isSheild)// �V�[���h���W�J����Ă����
	{
		pBoss->PlayBarrierSE();// �V�[���hSE�Đ�
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3(0.4f), EnemyParameters::BOSS_SHIELD_SCALE_SPEED);// �V�[���h�̃T�C�Y���g��
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefensiveHitRadius();// �{�X�̋��E�����V�[���h�̑傫���ɍ��킹��
		pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);// �U���̊Ԋu�𑬂�����
		pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);// ����U���̊Ԋu�𑬂�����
	}
	if (m_sheildHP <= 0)// �V�[���h���j�󂳂ꂽ��
	{
		m_isParticle = true;// �p�[�e�B�N���Đ�
		m_isSheild = false;// �V�[���h�j��
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefaultHitRadius();// �{�X�̋��E�������ɖ߂�
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());// �{�X�̈ʒu��ݒ�
		m_pParticle->SetBarrierBreakSize(pBoss->GetBarrierBreakSize());
		m_pParticle->Update(elapsedTime);// �p�[�e�B�N���X�V
	}
}
/*
*	@brief	�`��
*	@param[in] context �f�o�C�X�R���e�L�X�g
*	@param[in] states �X�e�[�g
*	@param[in] world ���[���h�s��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossSheild::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	DirectX::SimpleMath::Matrix world,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix proj)
{

	using namespace DirectX::SimpleMath;

	// m_pBoss��Boss�N���X�̃|�C���^�ɃL���X�g
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
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
		m_pParticle->CreateBillboard(pBoss->GetCameraTarget(), pBoss->GetCameraEye(), pBoss->GetCameraUp());// �r���{�[�h�쐬
		m_pParticle->Render(view, proj);// �p�[�e�B�N���`��
	}
}
