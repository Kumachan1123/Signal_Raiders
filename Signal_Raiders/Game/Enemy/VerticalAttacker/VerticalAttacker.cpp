/*
	@file	VerticalAttacker.cpp
	@brief	�����U���G�N���X
*/
#include "pch.h"
#include "VerticalAttacker.h"
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"

/*
*	@brief	�R���X�g���N�^
*	@param	pPlayer	�v���C���[�̃|�C���^�[
*	@param	resources	���ʃ��\�[�X
*	@param	hp	�G��HP
*	@return	�Ȃ�
*/
VerticalAttacker::VerticalAttacker(Player* pPlayer, CommonResources* resources, int hp)
	: IEnemy(pPlayer, resources, hp)// �e�N���X�̃R���X�g���N�^���Ă�
	, m_pPlayer{ pPlayer }// �v���C���[�̃|�C���^�[
	, m_pCamera{ pPlayer->GetCamera() }// �J�����̃|�C���^�[
	, m_enemyBS{}// �G�̋��E��
	, m_commonResources{ resources }// ���ʃ��\�[�X
	, m_currentHP{ hp }// �G��HP
	, m_attackCooldown{ EnemyParameters::ATTACK_COOLDOWN }// �U���̃N�[���_�E���^�C��
	, m_pVerticalAttackerModel{}// �G�̃��f��
	, m_enemyAI{}// �G��AI
	, m_pHPBar{}// �G��HP�o�[
	, m_position{}// �G�̍��W
	, m_velocity{}// �G�̑��x
	, m_rotate{}// �G�̉�]
	, m_enemyBSToPlayerArea{}// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	, m_enemyBulletBS{}// �G�̒e�̋��E��
	, m_playerBS{}// �v���C���[�̋��E��
	, m_matrix{}// �}�g���N�X
	, m_isDead{ false }// �G��HP��0�ɂȂ�����True
	, m_isHitToPlayer{ false }// �v���C���[�Ƃ̔���
	, m_isHitToOtherEnemy{ false }// ���̑��̓G�Ƃ̔���
	, m_isEnemyHitByPlayerBullet{ false }// �G���v���C���[�̒e�ɓ���������
	, m_isPlayerHitByEnemyBullet{ false }// �G�̒e���v���C���[�ɓ���������
	, m_canAttack{ true }// �U���\��
	, m_isAttack{ false }// �U������
{}
/*
*	@brief	�f�X�g���N�^
*	@param	�Ȃ�
*/
VerticalAttacker::~VerticalAttacker()
{
	m_pBulletManager->RemoveBulletsByShooter(this);// �e���폜����
}

/*
*	@brief	����������
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void VerticalAttacker::Initialize()
{
	using namespace DirectX::SimpleMath;
	DrawCollision::Initialize(m_commonResources);// �����蔻��`��N���X�̏�����
	m_pVerticalAttackerModel = std::make_unique<VerticalAttackerModel>();	// ���f����ǂݍ���
	m_pVerticalAttackerModel->Initialize(m_commonResources);// ���f���̏�����
	m_pHPBar = std::make_unique<EnemyHPBar>();	// HPBar����
	m_pHPBar->SetEnemyMaxHP(m_currentHP);// �ő�HP�ݒ�
	m_pHPBar->Initialize(m_commonResources);// ������
	m_enemyAI = std::make_unique<EnemyAI>(this);	// AI����
	m_enemyAI->Initialize();// AI������
	Vector3 position = 	// ��������
		Vector3(GenerateRandomMultiplier(-EnemyParameters::ENEMY_SPAWN_RADIUS, EnemyParameters::ENEMY_SPAWN_RADIUS));
	m_position = Vector3{ position.x, 0.0f,position.z };// �G�̏����ʒu��ݒ�
	m_enemyAI->SetPosition(m_position);	// �G�̍��W��ݒ�
	m_enemyBS.Center = m_position;	// ���E���̍��W�ݒ�
	m_enemyBS.Radius = EnemyParameters::NORMAL_ENEMY_RADIUS;// ���E���̔��a�ݒ�
}

/*
*	@brief	�X�V
*	@param	elapsedTime	�o�ߎ���
*	@return	�Ȃ�
*/
void VerticalAttacker::Update(float elapsedTime)
{
	m_pVerticalAttackerModel->SetState(m_enemyAI->GetState());// ���f���̍X�V
	m_pHPBar->SetCurrentHP(m_currentHP);// HP�̍X�V
	m_pHPBar->Update(elapsedTime);// HP�o�[�̍X�V
	m_enemyAI->Update(elapsedTime);// AI�̍X�V
	m_commonResources->GetAudioManager()->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
		ShootBullet();// �e�𔭎�
	m_enemyBS.Center = m_position;	// �G�̓����蔻��̍��W���X�V
	m_isDead = m_pHPBar->GetIsDead();// �G��HP��0�ɂȂ����玀�S
}

/*
*	@brief	�`��
*	@param	view	�r���[�s��
*	@param	proj	�ˉe�s��
*	@return	�Ȃ�
*/
void VerticalAttacker::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g�擾
	auto states = m_commonResources->GetCommonStates();// ���ʃX�e�[�g�擾
	Matrix world = Matrix::CreateScale(m_enemyAI->GetScale())// �X�P�[���s��
		* Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())// ��]�s��
		* Matrix::CreateTranslation(m_position);	// ���s�ړ��s��
	Vector3 hpBarPos = m_position + EnemyParameters::ENEMY_HPBAR_OFFSET;	// HPBar�̍��W��ݒ�
	m_pHPBar->Render(view, proj, hpBarPos, m_rotate);	// HPBar�`��
	m_pVerticalAttackerModel->Render(context, states, world, view, proj);	// �G�`��	
}
/*
*	@brief	�G�̓����蔻���`��
*	@param	view	�r���[�s��
*	@param	proj	�ˉe�s��
*	@return	�Ȃ�
*/
void VerticalAttacker::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(view);// ���g�p�ϐ��x���𖳎�
	UNREFERENCED_PARAMETER(proj);// ���g�p�ϐ��x���𖳎�
#ifdef _DEBUG// �f�o�b�O�r���h���̂ݎ��s
	DrawCollision::DrawStart(view, proj);	// �`��J�n
	DirectX::XMVECTOR color = Colors::Black;	// �F�ݒ�
	if (m_isHitToPlayer) color = m_isHitToOtherEnemy ? Colors::Tomato : Colors::Blue;	// ��������
	else color = m_isHitToOtherEnemy ? Colors::White : Colors::Black;	// �������Ă��Ȃ�
	DrawCollision::DrawBoundingSphere(m_enemyBS, color);	// ���E���`��
	DrawCollision::DrawEnd();	// �`��I��
#endif
}
/*
*	@brief	�G�̒e�𔭎�
*	@param	�Ȃ�
*	@return	�Ȃ�
*/
void VerticalAttacker::ShootBullet()
{
	m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();// �N�[���_�E���^�C�����擾
	if (m_attackCooldown <= EnemyParameters::ATTACK_INTERVAL)	// �U���̃N�[���_�E���^�C�����Ǘ�
	{
		m_commonResources->GetAudioManager()->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		DirectX::SimpleMath::Vector3 direction =
			DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());// �N�H�[�^�j�I����������x�N�g�����v�Z
		m_pBulletManager->SetEnemyBulletType(BulletType::SPEED);// �e�̎�ނ�ݒ�
		m_pBulletManager->SetEnemyBulletSize(EnemyParameters::ENEMY_BULLET_SIZE);// �e�̃T�C�Y��ݒ�
		m_pBulletManager->SetShooter(this);// �e�̔��ˎ҂�ݒ�
		m_pBulletManager->CreateEnemyBullet(m_position, direction);// �e�𐶐�
		m_enemyAI->GetEnemyAttack()->SetCoolTime(EnemyParameters::ATTACK_COOLDOWN);		// �N�[���_�E���^�C�������Z�b�g
	}
}
