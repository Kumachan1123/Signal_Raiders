/*
	@file	AreaAttacker.cpp
	@brief	�͈͍U���G�N���X
*/
#include "pch.h"
#include "AreaAttacker.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/AreaAttackerModel/AreaAttackerModel.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/Microsoft/ReadData.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>

// �R���X�g���N�^
AreaAttacker::AreaAttacker(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS{}
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_pAreaAttackerModel{}
	, m_enemyAI{}
	, m_HPBar{}
	, m_bullets{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{ false }
	, m_isHit{ false }
	, m_isHitToOtherEnemy{ false }
	, m_isHitToPlayerBullet{ false }
	, m_isBullethit{ false }
	, m_audioManager{ AudioManager::GetInstance() }
{}

// �f�X�g���N�^
AreaAttacker::~AreaAttacker() {}

// ����������
void AreaAttacker::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_commonResources = resources;
	// �f�o�C�X�ƃR���e�L�X�g���擾
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);
	// �e�p�̃��f����ǂݍ���
	m_pAreaAttackerModel = std::make_unique<AreaAttackerModel>();
	m_pAreaAttackerModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI����
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();
	// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);
	// ��������
	std::random_device rd;  // �V�[�h������
	std::mt19937 gen(rd()); // �����Z���k�E�c�C�X�^�̗���������
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f); // ��l���z
	m_position = Vector3{ dist(gen), 0.0f, dist(gen) };// �G�̏����ʒu��ݒ�

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// �G�̍��W��ݒ�
	m_enemyAI->SetPosition(m_position);
	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}

// �X�V����
void AreaAttacker::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	m_pAreaAttackerModel->Update(elapsedTime, m_enemyAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		// �U���̃N�[���_�E���^�C�����Ǘ�
		if (m_attackCooldown <= 0.1f)
		{
			m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
			// �N�H�[�^�j�I����������x�N�g�����v�Z
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
			// �e�𔭎�
			m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos, 0.15f, EnemyBullet::BulletType::VERTICAL);
			// �N�[���_�E���^�C�������Z�b�g
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}
	m_enemyBullets->Update(elapsedTime, GetPosition());// �G�̒e�̍X�V
	// �G�̓����蔻��̍��W���X�V
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);// HPBar�̍X�V
	m_isDead = m_HPBar->GetIsDead();// �G��HP��0�ɂȂ����玀�S
}

// �`��
void AreaAttacker::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// �G�̃T�C�Y��ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// �G�̍��W��ݒ�
	enemyWorld *= world;
	// HPBar�̍��W��ݒ�
	Vector3 hpBarPos = Vector3(m_position.x, m_position.y - 3.0f, m_position.z);
	// HPBar�`��
	m_HPBar->Render(view, proj, hpBarPos, m_rotate);
	// �G�`��	
	m_pAreaAttackerModel->Render(context, states, enemyWorld, view, proj);


	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);
#ifdef _DEBUG
	// �`�悷��
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherEnemy)DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_enemyBS, Colors::Tomato);

	}
	m_primitiveBatch->End();
#endif

}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�(���E�����m�̏ꍇ�j
void AreaAttacker::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
{
	using namespace DirectX::SimpleMath;
	// �����߂�����
	// �`�̒��S�Ƃa�̒��S�Ƃ̍����x�N�g���i�a����`�Ɍ������x�N�g���j�c�@
	Vector3 diffVector = A.Center - B.Center;
	// �`�̒��S�Ƃa�̒��S�Ƃ̋����i�@�̒����j�c�A
	float distance = diffVector.Length();
	// �`�̔��a�Ƃa�̔��a�̍��v�c�B
	float sumRadius = A.Radius + B.Radius;
	// �i�`���a�Ɂj�߂荞�񂾋����i�B�|�A�j�c�C
	float penetrationDistance = sumRadius - distance;
	// �@�𐳋K������c�D
	diffVector.Normalize();
	// �����߂��x�N�g�����v�Z����i�D�ƇC�ŕ\������j�c�E
	Vector3 pushBackVec = diffVector * penetrationDistance;
	// �E���g�p���āA�`�̍��W�Ƃ`�̃R���C�_�[���W���X�V����i���ۂɉ����߂��j
	m_position += pushBackVec;
	A.Center = m_position;
	A.Center.y -= 2.0f;
}
