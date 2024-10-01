/*
	@file	Boss.cpp
	@brief	�{�X�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Boss.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/Microsoft/ReadData.h"
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Boss::Boss(Player* pPlayer)
	: m_commonResources{}
	, m_currentHP{ 500 }
	, m_attackCooldown{ 3.0f }
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_bossBullets{}
	, m_bossBSToPlayerArea{}
	, m_bossBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherBoss{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
	, m_audioManager{ AudioManager::GetInstance() }
	, m_pPlayer{ pPlayer }
{
}

// �f�X�g���N�^
Boss::~Boss() {}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Boss::Initialize(CommonResources* resources, int hp)
{
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
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// �e�S�̐���
	/*m_bossBullets = std::make_unique<EnemyBullets>(this);
	m_bossBullets->Initialize(resources);*/
	// ���f������
	m_enemyModel = std::make_unique<EnemyModel>();
	m_enemyModel->Initialize(resources);
	// ���W��ݒ�
	m_position = Vector3(0.0f, 8.0f, 0.0f);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");
}
// �X�V
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	//m_enemyModel->Update(elapsedTime, m_enemyAI->GetState());// ���f���̃A�j���[�V�����X�V
	//m_enemyAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V
	//if (m_enemyAI->GetNowState() == m_enemyAI->GetEnemyAttack())// �U���Ԑ��Ȃ�
	//{
	//	m_attackCooldown = m_enemyAI->GetEnemyAttack()->GetCoolTime();
	//	// �U���̃N�[���_�E���^�C�����Ǘ�
	//	if (m_attackCooldown <= 0.1f)
	//	{
	//		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
	//		// �N�H�[�^�j�I����������x�N�g�����v�Z
	//		DirectX::SimpleMath::Vector3 direction = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_enemyAI->GetRotation());
	//		// �e�𔭎�
	//		m_enemyBullets->CreateBullet(GetPosition(), direction, playerPos);
	//		// �N�[���_�E���^�C�������Z�b�g
	//		m_enemyAI->GetEnemyAttack()->SetCoolTime(3.0f);
	//	}
	//}
	//m_bossBullets->Update(elapsedTime, GetPosition());// �G�̒e�̍X�V
	m_bossBS.Center = m_position;
	m_bossBS.Center.y -= 2.0f;
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}
// �`��
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world =/* Matrix::CreateFromQuaternion(m_enemyAI->GetRotation())
		* */Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// �G�̃T�C�Y��ݒ�
	//Matrix enemyWorld = Matrix::CreateScale(m_enemyAI->GetScale());
	// �G�̍��W��ݒ�
	//enemyWorld *= world;
	// HPBar�`��
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// �G�`��	
	m_enemyModel->Render(context, states, world, view, proj);
	// ���C�g�̕���
	Vector3 lightDir = Vector3::UnitY;
	lightDir.Normalize();
	//// �e�s��̌������
	//Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, -0.01f));
	//enemyWorld *= shadowMatrix;
	//// �e�`��
	//m_model->Draw(context, *states, enemyWorld * Matrix::Identity, view, proj, true, [&]()
	//			  {
	//				  context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xffffffff);
	//				  context->OMSetDepthStencilState(m_depthStencilState_Shadow.Get(), 0);
	//				  context->RSSetState(states->CullClockwise());
	//				  context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	//			  });
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
	//m_bossBullets->Render(view, proj);
#ifdef _DEBUG
	m_primitiveBatch->Begin();
	if (!m_isHit)
	{
		if (!m_isHitToOtherBoss)DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Black);
		else					DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::White);
	}
	else
	{
		if (!m_isHitToOtherBoss)DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Blue);
		else					 DX::Draw(m_primitiveBatch.get(), m_bossBS, Colors::Tomato);

	}
	m_primitiveBatch->End();
#endif

}
