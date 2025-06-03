/*
*	@file	BossSheild.cpp
*	@brief	�{�X�V�[���h�N���X
*/
#include <pch.h>
#include "BossSheild.h"

/*
*	@brief	�R���X�g���N�^
*	@details �{�X�V�[���h�N���X�̃R���X�g���N�^
*	@return	�Ȃ�
*/
BossSheild::BossSheild()
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_isSheild(false)// �V�[���h�W�J�t���O
	, m_isParticle(false)// �p�[�e�B�N���Đ��t���O
	, m_sheildSize(DirectX::SimpleMath::Vector3::Zero)// �V�[���h�̃T�C�Y
	, m_sheildPosition(DirectX::SimpleMath::Vector3::Zero)// �V�[���h�̍��W
	, m_sheildHP(0)// �V�[���h��HP
	, m_bossType(BossShieldType::BOSS)// �{�X�̎��
	, m_pBoss(nullptr)// �{�X�N���X�̃|�C���^
	, m_pSheildModel(nullptr)// �V�[���h���f��
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �e��|�C���^�[��nullptr�ɐݒ�
*	@return	�Ȃ�
*/
BossSheild::~BossSheild()
{
	// ���ʃ��\�[�X��nullptr�ɐݒ�
	m_pCommonResources = nullptr;
	// �V�[���h���f����nullptr�ɐݒ�
	m_pSheildModel = nullptr;
	// �{�X�N���X�̃|�C���^��nullptr�ɐݒ�
	m_pBoss = nullptr;
	// �p�[�e�B�N����nullptr�ɐݒ�
	m_pParticle = nullptr;
}

/*
*	@brief	�V�[���h�̏�����
*	@param sheildHP �V�[���h��HP
*	@param pBoss �{�X�N���X�̃|�C���^
*	@return	�Ȃ�
*/
void BossSheild::SetUp(int sheildHP, IEnemy* pBoss)
{
	using namespace DirectX::SimpleMath;
	// �V�[���h��HP��ݒ�
	m_sheildHP = sheildHP;
	// �{�X�N���X�̃|�C���^��ݒ�
	m_pBoss = pBoss;
	// �V�[���h�W�J�t���O�𗧂Ă�
	m_isSheild = false;
	// �V�[���h�̃T�C�Y��������
	m_sheildSize = Vector3::Zero;
	// �{�X�̈ʒu���V�[���h�̈ʒu�ɐݒ�
	m_sheildPosition = pBoss->GetPosition();
}
/*
*	@brief	������
*	@param resources ���ʃ��\�[�X
*	@return	�Ȃ�
*/
void BossSheild::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = resources;
	// �p�[�e�B�N���𐶐�
	m_pParticle = std::make_unique<Particle>(ParticleUtility::Type::BARRIERBREAK, 0.0f);
	// �p�[�e�B�N��������
	m_pParticle->Initialize(m_pCommonResources);
	// �}�l�[�W���[����V�[���h���f�����擾
	m_pSheildModel = m_pCommonResources->GetModelManager()->GetModel("Barrier");
}

/*
*	@brief	�X�V
*	@param elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void BossSheild::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �{�X�N���X�̃|�C���^���擾
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
	// �V�[���h���W�J����Ă����
	if (m_isSheild)
	{
		// �V�[���h�W�J�����Đ�����
		pBoss->PlayBarrierSE();
		// �V�[���h�̃T�C�Y�����炩�Ɋg�債�Ă���
		m_sheildSize = Vector3::SmoothStep(m_sheildSize, Vector3(0.4f), EnemyParameters::BOSS_SHIELD_SCALE_SPEED);
		// �{�X�̋��E����h�䃂�[�h�̃T�C�Y�ɐݒ�
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefensiveHitRadius();
		// �ʏ�U���̃N�[���_�E����Z�����čU�����x���グ��
		pBoss->SetBulletCooldown(EnemyParameters::BOSS_SHIELD_ATTACK_COOLDOWN);
		// ����U���̃N�[���_�E�����Z�k����
		pBoss->SetInitSpecialAttacCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN / 2);
	}
	// �V�[���h���j�󂳂ꂽ��
	if (m_sheildHP <= 0)
	{
		// �p�[�e�B�N���Đ��t���O�𗧂Ă�
		m_isParticle = true;
		// �V�[���h������
		m_isSheild = false;
		// ���E���̃T�C�Y��ʏ펞�̑傫���ɖ߂�
		pBoss->GetBoundingSphere().Radius = pBoss->GetDefaultHitRadius();
		// �p�[�e�B�N���̔����ʒu���{�X�̌��݈ʒu�ɐݒ�
		m_pParticle->SetBossPosition(m_pBoss->GetPosition());
		// �p�[�e�B�N���̃T�C�Y���{�X�̐ݒ�l�ɍ��킹��
		m_pParticle->SetBarrierBreakSize(pBoss->GetBarrierBreakSize());
		// �p�[�e�B�N���̍X�V
		m_pParticle->Update(elapsedTime);
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
void BossSheild::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// m_pBoss��Boss�N���X�̃|�C���^�ɃL���X�g
	auto pBoss = dynamic_cast<BossBase*>(m_pBoss);
	// �V�[���h���W�J����Ă����
	if (m_isSheild)
	{
		// �V�[���h�̃��[���h�s����쐬�i�X�P�[�����O�{�e�̃��[���h�s��j
		Matrix shieldWorld = Matrix::CreateScale(m_sheildSize) * world;
		// �V�[���h�`��
		m_pSheildModel->Draw(context, *states, shieldWorld, view, proj, false, [&]()
			{
				// �[�x�X�e���V���X�e�[�g�ݒ�i�ǂݍ��݂̂݁j
				context->OMSetDepthStencilState(states->DepthRead(), 0);
				// ���v���J�����O�ݒ�i���ʂ��J�����O�j
				context->RSSetState(states->CullClockwise());
				// ���Z�����u�����h�X�e�[�g�ݒ�i�����G�t�F�N�g�p�j
				context->OMSetBlendState(states->Additive(), nullptr, 0xffffffff);
			});
	}
	// �p�[�e�B�N���Đ��t���O�������Ă����
	if (m_isParticle)
	{
		// �{�X�̃J���������g���ăr���{�[�h�쐬
		m_pParticle->CreateBillboard(pBoss->GetCameraTarget(), pBoss->GetCameraEye(), pBoss->GetCameraUp());
		// �p�[�e�B�N���`��
		m_pParticle->Render(view, proj);
	}
}

