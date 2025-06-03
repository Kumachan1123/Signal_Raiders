/*
*	@file	BossBase.cpp
*	@brief	�{�X�x�[�X�N���X
*/
#include <pch.h>
#include "BossBase.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"


/*
*	@brief	�R���X�g���N�^
*	@details �{�X�x�[�X�N���X�̃R���X�g���N�^
*	@param pPlayer		�v���C���[�̃|�C���^
*	@param resources	���ʃ��\�[�X
*	@param hp			�{�X�̗̑�
*	@return	�Ȃ�
*/
BossBase::BossBase(Player* pPlayer, CommonResources* resources, int hp)
	: m_pPlayer(pPlayer)// �v���C���[�̃|�C���^��ݒ�
	, m_pCamera(pPlayer->GetCamera())// �J�����̃|�C���^��ݒ�
	, m_bossBS{}// ���E���̏�����
	, m_pCommonResources(resources)// ���ʃ��\�[�X
	, m_currentHP(hp)// ���݂�HP
	, m_pBulletManager(nullptr)// �e�Ǘ��N���X�̃|�C���^��ݒ�
	, m_maxHP{}// �ő�HP
	, m_attackCooldown(EnemyParameters::ATTACK_COOLDOWN)// �U���̃N�[���_�E���^�C��
	, m_specialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)// ����U���̃N�[���_�E���^�C��
	, m_initSpecialAttackCooldown(EnemyParameters::SPECIAL_ATTACK_COOLDOWN)	// ����U���̃N�[���_�E���^�C��(�������p)
	, m_bulletCooldown(EnemyParameters::ATTACK_INTERVAL)// �e�̃N�[���_�E���^�C��
	, m_bulletSize(EnemyParameters::BOSS_BULLET_SIZE)// �e�̃T�C�Y
	, m_bossDamage(EnemyParameters::BOSS_DAMAGE)// �{�X���v���C���[�ɗ^����_���[�W
	, m_SEVolume(0.0f)// SE�̉���
	, m_SEVolumeCorrection(0.0f)// SE�̉��ʕ␳
	, m_isBarrierSEPlay(false)// �o���ASE�̍Đ��t���O
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
*	@details �|���ꂽ��e���폜����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
BossBase::~BossBase() { m_pBulletManager->RemoveBulletsByShooter(this); }// �e���폜
/*
*	@brief	����������
*	@details �{�X�x�[�X�N���X�̏�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossBase::Initialize()
{
	// �{�X����
	m_pBoss = EnemyFactory::CreateBoss(m_bossType, this, m_pCommonResources);
	// �����蔻��`��̏�����
	DrawCollision::Initialize(m_pCommonResources);
	// �ő�HP��ݒ�
	m_maxHP = m_currentHP;
	// HP�o�[����
	m_pHPBar = std::make_unique<EnemyHPBar>();
	// HP�o�[��HP��ݒ�
	m_pHPBar->SetEnemyMaxHP(m_currentHP);
	// HP�o�[������
	m_pHPBar->Initialize(m_pCommonResources);
	// �{�X������
	m_pBoss->Initialize();
	// �{�XAI����
	m_pBossAI = std::make_unique<BossAI>(this);
	// �{�XAI������
	m_pBossAI->Initialize();
	// AI�ɍ��W��ݒ�
	m_pBossAI->SetPosition(m_position);
	// ���E���̒��S���W��ݒ�
	m_bossBS.Center = m_position;
	// SE�̉��ʂ�ݒ�
	m_SEVolume = m_pPlayer->GetVolume();
	// SE�̉��ʕ␳��ݒ�
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();
	// �V�[���h����
	m_pBossSheild = std::make_unique<BossSheild>();
	// �V�[���h�̏�����
	m_pBossSheild->SetUp(m_maxHP, this);
	// �V�[���h������
	m_pBossSheild->Initialize(m_pCommonResources);

}
/*
*	@brief	�X�V����
*	@details �{�X�x�[�X�N���X�̍X�V����
*	@param elapsedTime �o�ߎ���
*	@return	�Ȃ�
*/
void BossBase::Update(float elapsedTime)
{
	// �J�����̈ʒu���擾
	m_cameraEye = m_pCamera->GetEyePosition();
	// �J�����̒����_���擾
	m_cameraTarget = m_pCamera->GetTargetPosition();
	// �J�����̏�������擾
	m_cameraUp = m_pCamera->GetUpVector();
	// �{�X�̍X�V
	m_pBoss->ChangeState();
	// AI�̍X�V
	m_pBossAI->Update(elapsedTime);
	// �G�̍��W���X�V
	m_position = m_pBossAI->GetPosition();
	// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pCommonResources->GetAudioManager()->Update();
	// �U���̃N�[���_�E���^�C�����擾
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	// ����U���̃N�[���_�E���^�C�������炷
	m_specialAttackCooldown -= elapsedTime;
	// �e�̈ʒu�ݒ�
	m_pBoss->BulletPositioning();
	// �e����
	ShootBullet();
	// ���E���̒��S���W���X�V
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;
	// �ʏ펞�̃{�X�̓����蔻���ݒ�
	m_bossBS.Radius = m_defaultHitRadius;
	// HP�o�[��HP���X�V
	m_pHPBar->SetCurrentHP(m_currentHP);
	// HP�o�[�̍X�V
	m_pHPBar->Update(elapsedTime);
	// HP�������ȉ��ɂȂ�����V�[���h���o��
	if (m_currentHP <= m_maxHP / 2)	m_pBossSheild->SetSheild(true);
	// �V�[���h�̍X�V
	m_pBossSheild->Update(elapsedTime);
	// ���񂾂��ǂ������󂯎��
	m_isDead = m_pHPBar->GetIsDead();
}
/*
*	@brief	�`�揈��
*	@details �{�X�x�[�X�N���X�̕`�揈��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �G�̍��W���X�V
	m_position = m_pBossAI->GetPosition();
	// �G�̉�]���X�V
	m_quaternion = m_pBossAI->GetRotation();
	// �G�̃X�P�[�����X�V
	m_scale = m_pBossAI->GetScale();
	// �{�X�`��
	m_pBoss->Draw(view, proj);
}
/*
*	@brief	�����蔻��`�揈��
*	@details �{�X�x�[�X�N���X�̓����蔻��`�揈���i�f�o�b�O�p�j
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossBase::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	// ���g�p�̌x�����o���Ȃ�
	UNREFERENCED_PARAMETER(view);
	// ���g�p�̌x�����o���Ȃ�
	UNREFERENCED_PARAMETER(proj);
#ifdef _DEBUG
	// �`��J�n
	DrawCollision::DrawStart(view, proj);
	// �F��ݒ�
	DirectX::XMVECTOR color = Colors::Black;
	// �v���C���[�ɓ������Ă���ꍇ
	if (m_isHitToPlayer)
	{
		// ���̓G�ɂ��������Ă���Ȃ�g�}�g�F�A�����łȂ���ΐ�
		color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;
	}
	// �v���C���[�ɓ������Ă��Ȃ��ꍇ
	else
	{
		// ���̓G�ɓ������Ă���Ȃ甒�A�����łȂ���΍�
		color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;
	}
	// ���E���̕`��
	DrawCollision::DrawBoundingSphere(m_bossBS, color);
	// �`��I��
	DrawCollision::DrawEnd();
#endif
}
/*
*	@brief	�V�[���h�W�J���Đ�
*	@details �V�[���h�W�J�����Đ�����
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossBase::PlayBarrierSE()
{
	// ���ɍĐ����Ȃ牽�����Ȃ�
	if (m_isBarrierSEPlay) return;
	// �V�[���h�W�J���Đ� 
	m_pCommonResources->GetAudioManager()->PlaySound("Barrier", m_pPlayer->GetVolume());
	// �Đ��t���O�𗧂Ă�
	m_isBarrierSEPlay = true;
}
/*
*	@brief	�e����
*	@details �{�X���e�𔭎˂��鏈��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossBase::ShootBullet()
{
	using namespace DirectX::SimpleMath;
	// �ʏ�U���̃N�[���_�E���^�C�����`�F�b�N
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)
	{
		// �ʏ�e�̃T�E���h�Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// �e�̔�ԕ������A���݂̃{�X�̉�]����v�Z
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());
		// �ʏ�e�̐���
		m_pBoss->CreateBullet();
		// �U���N�[���_�E���^�C�������Z�b�g
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);
	}
	// ����U���̃N�[���_�E���^�C�����`�F�b�N
	if (m_specialAttackCooldown <= 0.0f)
	{
		// ����e�̃T�E���h�Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
		// �e�̃T�C�Y��ݒ�
		m_pBulletManager->SetEnemyBulletSize(m_bulletSize);
		// ���˂����I�u�W�F�N�g���w��
		m_pBulletManager->SetShooter(this);
		// ����U���i�X�p�C�����e�j�̐���
		CreateSpiralBullet();
		// ����U���̃N�[���_�E�������Z�b�g
		m_specialAttackCooldown = m_initSpecialAttackCooldown;
	}
}

/*
*	@brief	�^���ɗ����ĉ�������e�𔭎�
*	@details �{�X���^���ɗ����ĉ�������e�𔭎˂��鏈��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossBase::CreateVerticalBullet()
{
	// �e�̎�ނ�ݒ�
	m_pBulletManager->SetEnemyBulletType(BulletType::SPEED);
	// �e�𐶐�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}

/*
*	@brief	����U��������
*	@details �{�X������U�������鏈��
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void BossBase::CreateSpiralBullet()
{
	// �e�̎�ނ�ݒ�
	m_pBulletManager->SetEnemyBulletType(BulletType::SPECIAL);
	// �e�𐶐�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);
}
/*
*	@brief	�G��HP��ݒ�
*	@details �G��HP��ݒ肷��
*	@param hp �G��HP
*	@return	�Ȃ�
*/
void BossBase::ApplyDamageToEnemy(int hp)
{
	// �V�[���h������A�V�[���h���W�J����Ă���ꍇ
	if (m_pBossSheild->GetSheildHP() > 0 && m_pBossSheild->GetSheild() == true)
	{
		// �V�[���h��HP�����炷
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);
		// �V�[���h����ꂽ��T�E���h�Đ�
		if (m_pBossSheild->GetSheildHP() <= 0)m_pCommonResources->GetAudioManager()->PlaySound("BarrierBreak", m_pPlayer->GetVolume());
	}
	else // �V�[���h���Ȃ��ꍇ�͓G��HP�����炷
		m_currentHP -= hp;
}