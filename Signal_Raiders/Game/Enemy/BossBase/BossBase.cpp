#include <pch.h>
#include "BossBase.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
/*
*	@brief	�R���X�g���N�^
*	@param[in] pPlayer		�v���C���[�̃|�C���^
*	@param[in] resources	���ʃ��\�[�X
*	@param[in] hp			�{�X�̗̑�
*	@return	�Ȃ�
*/
BossBase::BossBase(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)// �e�N���X�̃R���X�g���N�^���Ăяo��
	, m_pPlayer(pPlayer)// �v���C���[�̃|�C���^��ݒ�
	, m_pCamera(pPlayer->GetCamera())// �J�����̃|�C���^��ݒ�
	, m_bossBS{}// ���E���̏�����
	, m_commonResources(resources)// ���ʃ��\�[�X
	, m_currentHP(hp)// ���݂�HP
	, m_maxHP{}// �ő�HP
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// �U���̃N�[���_�E���^�C��
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// ����U���̃N�[���_�E���^�C��
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// ����U���̃N�[���_�E���^�C��(�������p)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// �e�̃N�[���_�E���^�C��
	, m_bulletSize(EnemyParameters::BOSS_BULLET_SIZE)// �e�̃T�C�Y
	, m_bossDamage(EnemyParameters::BOSS_DAMAGE)// �{�X���v���C���[�ɗ^����_���[�W
	, m_SEVolume(0.0f)// SE�̉���
	, m_SEVolumeCorrection(0.0f)// SE�̉��ʕ␳
	, m_pBossModel{}// �{�X���f��
	, m_pBossAI{}// AI
	, m_pHPBar{}// HP�o�[
	, m_time(0.0f)// ����
	, m_position{}// �ʒu
	, m_velocity{}// ���x
	, m_rotate{}// ��]
	, m_enemyBSToPlayerArea{}// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	, m_enemyBulletBS{}// �G�̒e�̋��E��
	, m_playerBS{}// �v���C���[�̋��E��
	, m_matrix{}// �}�g���N�X
	, m_isDead(false)// ���S�t���O
	, m_isHitToPlayer(false)// �v���C���[�ɓ���������
	, m_isHitToOtherEnemy(false)// ���̓G�ɓ���������
	, m_isEnemyHitByPlayerBullet(false)// �G���v���C���[�̒e�ɓ���������
	, m_isPlayerHitByEnemyBullet(false)// �v���C���[���G�̒e�ɓ���������
	, m_canAttack(true)// �U���\��
	, m_isAttack(false)// �U������
	, m_bossType(BossType::NORMAL_BOSS)// �{�X�̎��
	, m_bossBulletType(BossBulletType::STAGE_4)// �{�X�̒e�̎��
	, m_bulletType(BulletType::NORMAL)// �e�̎��
	, m_defaultHitRadius(EnemyParameters::NORMAL_BOSS_RADIUS)// �f�t�H���g�̓����蔻�蔼�a
	, m_defensiveHitRadius(EnemyParameters::BOSS_SHIELD_RADIUS)// �e�̔��ˈʒu
	, m_barrierBreakSize(EnemyParameters::BOSS_BARRIERBREAK_SIZE)// �o���A�j��p�[�e�B�N���̃T�C�Y
	, m_deadEffectSize(EnemyParameters::BOSS_DEADEFFECT_SCALE)// �{�X�̎��S�G�t�F�N�g�̃T�C�Y
{
}
/*
*	@brief	�f�X�g���N�^
*	@return	�Ȃ�
*/
BossBase::~BossBase() { m_pBulletManager->RemoveBulletsByShooter(this); }// �e���폜
/*
*	@brief	����������
*	@return	�Ȃ�
*/
void BossBase::Initialize()
{
	m_pBoss = EnemyFactory::CreateBoss(m_bossType, this, m_commonResources);// �{�X����
	DrawCollision::Initialize(m_commonResources);// �����蔻��`��̏�����
	m_maxHP = m_currentHP;// �ő�HP��ݒ�
	m_pHPBar = std::make_unique<EnemyHPBar>();	// HP�o�[����
	m_pHPBar->SetEnemyMaxHP(m_currentHP);// HP�o�[��HP��ݒ�
	m_pHPBar->Initialize(m_commonResources);// HP�o�[������
	m_pBoss->Initialize();// �{�X������
	m_pBossAI = std::make_unique<BossAI>(this);// �{�XAI����
	m_pBossAI->Initialize();// �{�XAI������
	m_pBossAI->SetPosition(m_position);// AI�ɍ��W��ݒ�
	m_bossBS.Center = m_position;// ���E���̒��S���W��ݒ�
	m_SEVolume = m_pPlayer->GetVolume();// SE�̉��ʂ�ݒ�
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();// SE�̉��ʕ␳��ݒ�
	m_pBossSheild = std::make_unique<BossSheild>();// �V�[���h���� ����̓^�C�v�ɂ���ĕ���\��
	m_pBossSheild->SetUp(m_maxHP, this);// �V�[���h�̏�����
	m_pBossSheild->Initialize(m_commonResources);// �V�[���h������ ����̓^�C�v�ɂ���ĕ���\��
}
/*
*	@brief	�X�V����
*	@param[in] elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void BossBase::Update(float elapsedTime)
{
	m_cameraEye = m_pCamera->GetEyePosition();// �J�����̈ʒu���擾
	m_cameraTarget = m_pCamera->GetTargetPosition();// �J�����̒����_���擾
	m_cameraUp = m_pCamera->GetUpVector();// �J�����̏�������擾
	m_pBoss->ChangeState();// �{�X�̍X�V
	m_pBossAI->Update(elapsedTime);// AI�̍X�V
	m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	m_commonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// �U���̃N�[���_�E���^�C�����擾
	m_specialAttackCooldown -= elapsedTime;// ����U���̃N�[���_�E���^�C�������炷
	m_pBoss->BulletPositioning();// �e�̈ʒu�ݒ�
	this->ShootBullet();// �e����
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// ���E���̒��S���W���X�V
	m_bossBS.Radius = m_defaultHitRadius;// �ʏ펞�̃{�X�̓����蔻���ݒ�
	m_pHPBar->SetCurrentHP(m_currentHP);// HP�o�[��HP���X�V
	m_pHPBar->Update(elapsedTime);// HP�o�[�̍X�V
	if (m_currentHP <= m_maxHP / 2)	m_pBossSheild->SetSheild(true);// HP�������ȉ��ɂȂ�����V�[���h���o��
	m_pBossSheild->Update(elapsedTime);// �V�[���h�̍X�V
	m_isDead = m_pHPBar->GetIsDead();// ���񂾂��ǂ������󂯎��
}
/*
*	@brief	�`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossBase::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	m_quaternion = m_pBossAI->GetRotation();// �G�̉�]���X�V
	m_scale = m_pBossAI->GetScale();// �G�̃X�P�[�����X�V
	m_pBoss->Draw(view, proj);// �{�X�`��
}
/*
*	@brief	�����蔻��`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossBase::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	UNREFERENCED_PARAMETER(view);// ���g�p�̌x�����o���Ȃ�
	UNREFERENCED_PARAMETER(proj);// ���g�p�̌x�����o���Ȃ�
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);// �`��J�n
	DirectX::XMVECTOR color = Colors::Black;// �F
	if (m_isHitToPlayer)color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;// ��������
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;// �������Ă��Ȃ�
	DrawCollision::DrawBoundingSphere(m_bossBS, color);// ���E���̕`��
	DrawCollision::DrawEnd();// �`��I��
#endif
}
/*
*	@brief	�V�[���h�W�J���Đ�
*	@return	�Ȃ�
*/
void BossBase::PlayBarrierSE()
{
	m_commonResources->GetAudioManager()->PlaySound("Barrier", this->GetSheildSEVolume());// �T�E���h�Đ� 
}
/*
*	@brief	�e����
*	@return	�Ȃ�
*/
void BossBase::ShootBullet()
{
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)	// �U���̃N�[���_�E���^�C�����Ǘ�
	{
		m_commonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());// �N�H�[�^�j�I����������x�N�g�����v�Z
		m_pBoss->CreateBullet();// �e�𔭎�
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);// �N�[���_�E���^�C����ݒ�
	}
	if (m_specialAttackCooldown <= 0.0f)// ����U���̃N�[���_�E���^�C�����Ǘ�
	{
		m_commonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		m_pBulletManager->SetEnemyBulletSize(m_bulletSize);// �e�̃T�C�Y��ݒ�
		m_pBulletManager->SetShooter(this);// �e�𔭎˂����I�u�W�F�N�g��ݒ�
		this->CreateSpiralBullet();// 		// ����U��
		m_specialAttackCooldown = m_initSpecialAttackCooldown;// �N�[���_�E���^�C����ݒ�
	}
}

/*
*	@brief	�^���ɗ����ĉ�������e�𔭎�
*	@return	�Ȃ�
*/
void BossBase::CreateVerticalBullet()
{
	m_pBulletManager->SetEnemyBulletType(BulletType::SPEED);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�
}

/*
*	@brief	����U��������
*	@return	�Ȃ�
*/
void BossBase::CreateSpiralBullet()
{
	m_pBulletManager->SetEnemyBulletType(BulletType::SPECIAL);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�
}
/*
*	@brief	�G��HP��ݒ�
*	@param[in] hp �G��HP
*	@return	�Ȃ�
*/
void BossBase::SetEnemyMaxHP(int hp)
{
	if (m_pBossSheild->GetSheildHP() > 0 &&// �V�[���h������ꍇ
		m_pBossSheild->GetSheild() == true)// �V�[���h���W�J����Ă���ꍇ
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);// �V�[���h��HP�����炷
		if (m_pBossSheild->GetSheildHP() <= 0)// �V�[���h����ꂽ��T�E���h�Đ�
			m_commonResources->GetAudioManager()->PlaySound("BarrierBreak", m_pPlayer->GetVolume());
	}
	else m_currentHP -= hp;// �V�[���h���Ȃ��ꍇ�͓G��HP�����炷
}