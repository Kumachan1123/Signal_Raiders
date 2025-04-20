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
	, m_bossType(BossType::BOSS)// �{�X�̎��
	//, m_bossSheildType(BossSheild::BossShieldType::BOSS)// �{�X�̃V�[���h�̎��
	, m_bossBulletType(BossBulletType::NORMAL)// �{�X�̒e�̎��
	, m_bulletType(EnemyBullet::BulletType::NORMAL)// �e�̎��
	, m_audioManager{ }// �I�[�f�B�I�}�l�[�W���[
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

	if (m_bossType == BossType::BOSS) m_pBoss = std::make_unique< Boss>(this, m_commonResources);// �{�X����
	else   m_pBoss = std::make_unique<LastBoss>(this, m_commonResources);// ���X�{�X����



	DrawCollision::Initialize(m_commonResources);// �����蔻��`��̏�����
	m_maxHP = m_currentHP;// �ő�HP��ݒ�
	m_pHPBar = std::make_unique<EnemyHPBar>();	// HP�o�[����
	m_pHPBar->SetEnemyHP(m_currentHP);// HP�o�[��HP��ݒ�
	m_pHPBar->Initialize(m_commonResources);// HP�o�[������
	m_pBossAI = std::make_unique<BossAI>(this);// �{�XAI����
	m_pBossAI->Initialize();// �{�XAI������
	m_position = EnemyParameters::INITIAL_BOSS_POSITION;// �G�̍��W��ݒ�
	m_pBossAI->SetPosition(m_position);// AI�ɍ��W��ݒ�
	m_bossBS.Center = m_position;// ���E���̒��S���W��ݒ�
	m_bossBS.Radius = EnemyParameters::NORMAL_BOSS_RADIUS;// ���E���̔��a��ݒ�
	m_SEVolume = m_pPlayer->GetVolume();// SE�̉��ʂ�ݒ�
	m_SEVolumeCorrection = m_pPlayer->GetVolumeCorrection();// SE�̉��ʕ␳��ݒ�
	m_pBoss->Initialize();// �{�X������
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
	m_pBoss->Update(elapsedTime);// �{�X�̍X�V
	//m_pBossModel->SetState(m_pBossAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_pBossAI->Update(elapsedTime);// AI�̍X�V
	m_position = m_pBossAI->GetPosition();// �G�̍��W���X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();// �U���̃N�[���_�E���^�C�����擾
	m_specialAttackCooldown -= elapsedTime;// ����U���̃N�[���_�E���^�C�������炷
	this->ShootBullet();// �e����
	m_bossBS.Center = m_position + EnemyParameters::BOSS_SPHERE_OFFSET;// ���E���̒��S���W���X�V
	this->BulletPositioning();// �e�̈ʒu�ݒ�
	m_pHPBar->SetCurrentHP(m_currentHP);// HP�o�[��HP���X�V
	m_pHPBar->Update(elapsedTime);// HP�o�[�̍X�V
	if (m_currentHP <= m_maxHP / 2)
		m_pBossSheild->SetSheild(true);// HP�������ȉ��ɂȂ�����V�[���h���o��
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
	m_pBoss->Render(view, proj);// �{�X�`��

	//auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g
	//auto states = m_commonResources->GetCommonStates();// �X�e�[�g
	//// ���[���h�s���ݒ�
	//Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale())// �X�P�[��
	//	* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// ��]
	//	* Matrix::CreateTranslation(m_position);// �ʒu
	//// �V�[���h�̃��[���h�s���ݒ�
	//Matrix sheildWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 3)// �X�P�[��(�V�[���h�̑傫��)
	//	* Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// ��]
	//	* Matrix::CreateTranslation(m_position);// �ʒu
	//m_pBossSheild->SetPosition(m_bossBS.Center);// �V�[���h�̍��W��ݒ�
	//m_pBossSheild->SetRotation(m_pBossAI->GetRotation());// �V�[���h�̉�]��ݒ�
	//m_pBossModel->Render(context, states, enemyWorld, view, proj);// ���f���`��
	//m_pBossSheild->Render(context, states, enemyWorld, view, proj);// �V�[���h�`��
	//Vector3 hpBarPos = m_position - EnemyParameters::BOSS_HPBAR_OFFSET;// HP�o�[�̈ʒu��ݒ�
	//m_pHPBar->SetScale(Vector3(EnemyParameters::BOSS_HPBAR_SCALE));// HP�o�[�̃X�P�[����ݒ�
	//m_pHPBar->Render(view, proj, hpBarPos, m_rotate);// HP�o�[�`��
}
/*
*	@brief	�����蔻��`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return	�Ȃ�
*/
void BossBase::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	//m_pBoss->DrawCollision(view, proj);// �{�X�̓����蔻��`��
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
	m_audioManager->PlaySound("Barrier", this->GetSheildSEVolume());// �T�E���h�Đ� 

}
/*
*	@brief	�e����
*	@return	�Ȃ�
*/
void BossBase::ShootBullet()
{
	if (m_attackCooldown <= EnemyParameters::ATTACK_COOLDOWN_THRESHOLD)	// �U���̃N�[���_�E���^�C�����Ǘ�
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		m_bulletDirection = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());// �N�H�[�^�j�I����������x�N�g�����v�Z
		this->CreateBullet();// �e�𔭎�
		m_pBossAI->GetBossAttack()->SetCoolTime(m_bulletCooldown);// �N�[���_�E���^�C����ݒ�
	}
	if (m_specialAttackCooldown <= 0.0f)// ����U���̃N�[���_�E���^�C�����Ǘ�
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE * 2);// �e�̃T�C�Y��ݒ�
		m_pBulletManager->SetShooter(this);// �e�𔭎˂����I�u�W�F�N�g��ݒ�
		this->CreateSpiralBullet();// 		// ����U��
		m_specialAttackCooldown = m_initSpecialAttackCooldown;// �N�[���_�E���^�C����ݒ�
	}
}
/*
*	@brief	�e�̈ʒu�ݒ�
*	@return	�Ȃ�
*/
void BossBase::BulletPositioning()
{
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())// �e�̔��ˈʒu��ݒ�
		* Matrix::CreateTranslation(m_position);
	m_bulletPosCenter = Vector3::Transform(EnemyParameters::BOSS_HEAD_OFFSET, transform);// �����̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(EnemyParameters::BOSS_LEFT_GUN_OFFSET, transform);// ���̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(EnemyParameters::BOSS_RIGHT_GUN_OFFSET, transform);// �E�̍��W�ɉ�]��K�p

}
/*
*	@brief	�e�̐���
*	@return	�Ȃ�
*/
void BossBase::CreateBullet()
{
	float angleOffset = XMConvertToRadians(EnemyParameters::BOSS_BULLET_ANGLE); // 30�x�̊p�x�I�t�Z�b�g
	m_pBulletManager->SetEnemyBulletSize(EnemyParameters::BOSS_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
	m_pBulletManager->SetShooter(this);// �e�𔭎˂����I�u�W�F�N�g��ݒ�
	switch (GetBulletType())	// Enemies�N���X�Őݒ肵���e�̃^�C�v�ɂ���ď����𕪊�
	{
	case BossBulletType::NORMAL:// �ʏ�e
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// �����̒e�𔭎�
		break;
	case BossBulletType::TWIN:// ��

		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// �E�̒e�𔭎�
		break;
	case BossBulletType::THREE:// �O��
		CreateCenterBullet(EnemyBullet::BulletType::NORMAL);// �����̒e�𔭎�
		CreateLeftBullet(-angleOffset, EnemyBullet::BulletType::NORMAL);// ���̒e�𔭎�
		CreateRightBullet(angleOffset, EnemyBullet::BulletType::NORMAL);// �E�̒e�𔭎�
		break;
	}
}
/*
*	@brief	��������e�𔭎�
*	@param[in] type �e�̎��
*/
void BossBase::CreateCenterBullet(EnemyBullet::BulletType type)
{
	m_pBulletManager->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�

}
/*
*	@brief	���̒e�𔭎�
*	@param[in] angleOffset �p�x�I�t�Z�b�g
*	@param[in] type �e�̎��
*/
void BossBase::CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type)
{
	Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);// ������
	Vector3 leftDirection = Vector3::Transform(m_bulletDirection, leftRotation);
	m_pBulletManager->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosLeft, leftDirection);// �e�𐶐�

}

/*
*	@brief �E�̒e�𔭎�
*	@param[in] angleOffset �p�x�I�t�Z�b�g
*	@param[in] type �e�̎��
*/
void BossBase::CreateRightBullet(float angleOffset, EnemyBullet::BulletType type)
{
	Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);// �E����
	Vector3 rightDirection = Vector3::Transform(m_bulletDirection, rightRotation);
	m_pBulletManager->SetEnemyBulletType(type);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosRight, rightDirection);// �e�𐶐�

}
/*
*	@brief	�^���ɗ����ĉ�������e�𔭎�
*	@return	�Ȃ�
*/
void BossBase::CreateVerticalBullet()
{
	m_pBulletManager->SetEnemyBulletType(EnemyBullet::BulletType::SPEED);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�

}

/*
*	@brief	����U��������
*	@return	�Ȃ�
*/
void BossBase::CreateSpiralBullet()
{
	m_pBulletManager->SetEnemyBulletType(EnemyBullet::BulletType::SPECIAL);// �e�̎�ނ�ݒ�
	m_pBulletManager->CreateEnemyBullet(m_bulletPosCenter, m_bulletDirection);// �e�𐶐�

}
/*
*	@brief	�G��HP��ݒ�
*	@param[in] hp �G��HP
*	@return	�Ȃ�
*/
void BossBase::SetEnemyHP(int hp)
{
	if (m_pBossSheild->GetSheildHP() > 0 &&// �V�[���h������ꍇ
		m_pBossSheild->GetSheild() == true)// �V�[���h���W�J����Ă���ꍇ
	{
		m_pBossSheild->SetSheildHP(m_pBossSheild->GetSheildHP() - hp);// �V�[���h��HP�����炷
		if (m_pBossSheild->GetSheildHP() <= 0)m_audioManager->PlaySound("BarrierBreak", m_pPlayer->GetVolume());// �V�[���h����ꂽ��T�E���h�Đ� 
	}
	else m_currentHP -= hp;// �V�[���h���Ȃ��ꍇ�͓G��HP�����炷

}