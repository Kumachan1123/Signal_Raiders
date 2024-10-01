/*
	@file	Enemy.cpp
	@brief	�G�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/EnemyModel/EnemyModel.h"
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
Enemy::Enemy(Player* pPlayer)
	: IEnemy(pPlayer)  // BaseEnemy�̃R���X�g���N�^���Ăяo��
	, m_enemyAI{}
	, m_enemyBullets{}

{}
// �f�X�g���N�^
Enemy::~Enemy() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Enemy::Initialize(CommonResources* resources, int hp)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	IEnemy::Initialize(resources, hp);  // BaseEnemy�̏���������

	// AI����
	m_enemyAI = std::make_unique<EnemyAI>();
	m_enemyAI->Initialize();

	// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);

	// ���������i�ʒu�̏������j
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f);
	m_position.x = dist(gen);
	m_position.y = 6.0f;
	m_position.z = dist(gen);

	// AI�ɍ��W��ݒ�
	m_enemyAI->SetPosition(m_position);

	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 1.5f;
}
// �`��
void Enemy::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
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
	// HPBar�`��
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// �G�`��	
	m_enemyModel->Render(context, states, enemyWorld, view, proj);
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	// �e�s��̌������
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	enemyWorld *= shadowMatrix;
	// �e�`��
	m_model->Draw(context, *states, enemyWorld * Matrix::Identity, view, proj, true, [&]()
				  {
					  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
					  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 0);
					  context->RSSetState(states->CullClockwise());
					  context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
				  });
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
	// �G�̒e�`��
	m_enemyBullets->Render(view, proj);
#ifdef _DEBUG
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
// �X�V
void Enemy::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	// BaseEnemy�̍X�V�������Ăяo��
	IEnemy::Update(elapsedTime, playerPos);

	// AI�̍X�V
	m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);

	// �U������
	if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())
	{
		m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
		if (m_attackCooldown <= 0.1f)
		{
			m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());
			DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
			m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
			m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
		}
	}

	// �e�̍X�V
	m_enemyBullets->Update(elapsedTime, GetPosition());

	// ���E���̈ʒu���X�V
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 2.0f;

	// HP�o�[�̍X�V
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�
void Enemy::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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




