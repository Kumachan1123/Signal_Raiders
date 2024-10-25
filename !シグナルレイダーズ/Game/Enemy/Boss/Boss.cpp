/*
	@file	Boss.cpp
	@brief	�{�X�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Boss/Boss.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/Boss/BossAI/BossAI.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Enemy/EnemyBullets/EnemyBullets.h"
#include "Game/Enemy/Boss/BossModel/BossModel.h"
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
Boss::Boss(Player* pPlayer)
	: IEnemy(pPlayer)
	, m_pPlayer{ pPlayer }
	, m_pCamera{ pPlayer->GetCamera() }
	, m_enemyBS()
	, m_commonResources{}
	, m_currentHP{}
	, m_attackCooldown{ 3.0f }
	, m_bossModel{}
	, m_pBossAI{}
	, m_HPBar{}
	, m_time{ 0.0f }
	, m_depthStencilState_Shadow{}
	, m_pixelShader{}
	, m_depthStencilState{}
	, m_position{}
	, m_velocity{}
	, m_rotate{}
	, m_enemyBSToPlayerArea{}
	, m_enemyBulletBS{}
	, m_playerBS{}
	, m_matrix{}
	, m_isDead{}
	, m_isHit{}
	, m_isHitToOtherEnemy{}
	, m_isHitToPlayerBullet{}
	, m_isBullethit{}
	, m_audioManager{ AudioManager::GetInstance() }

{}
// �f�X�g���N�^
Boss::~Boss() {}
//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void Boss::Initialize(CommonResources* resources, int hp)
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
	std::vector<uint8_t> ps = DX::ReadData(L"Resources/Shaders/PS_EnemyShadow.cso");
	DX::ThrowIfFailed(device->CreatePixelShader(ps.data(), ps.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models/Boss");
	// �e�p�̃��f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Boss/Boss.cmo", *fx);
	m_bossModel = std::make_unique<BossModel>();
	m_bossModel->Initialize(m_commonResources);
	// �G�̗̑͂�ݒ�
	m_currentHP = hp;
	// HPBar����
	m_HPBar = std::make_unique<EnemyHPBar>();
	m_HPBar->SetEnemyHP(m_currentHP);
	m_HPBar->Initialize(resources);
	// AI����
	m_pBossAI = std::make_unique<BossAI>();
	m_pBossAI->Initialize();
	//// �e�S�̐���
	m_enemyBullets = std::make_unique<EnemyBullets>(this);
	m_enemyBullets->Initialize(resources);
	// ��������
	std::random_device rd;  // �V�[�h������
	std::mt19937 gen(rd()); // �����Z���k�E�c�C�X�^�̗���������
	std::uniform_real_distribution<float> dist(-50.0f, 50.0f); // ��l���z
	m_position.x = dist(gen);
	m_position.y = 10.0f;
	m_position.z = dist(gen);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	// �G�̍��W��ݒ�
	m_pBossAI->SetPosition(m_position);


	// ���E���̏�����
	m_enemyBS.Center = m_position;
	m_enemyBS.Radius = 2.5f;
	// �I�[�f�B�I�}�l�[�W���[
	m_audioManager->LoadSound("Resources/Sounds/enemybullet.mp3", "EnemyBullet");

}
// �`��
void Boss::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// ��ƂȂ���W����]���
	Matrix world = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position)
		* Matrix::CreateTranslation(Vector3{ 0,-2,0 });
	// �G�̃T�C�Y��ݒ�
	Matrix enemyWorld = Matrix::CreateScale(m_pBossAI->GetScale() * 2);
	// �G�̍��W��ݒ�
	enemyWorld *= world;
	// HPBar�`��
	m_HPBar->Render(view, proj, m_position, m_rotate);
	// �G�`��	
	m_bossModel->Render(context, states, enemyWorld, view, proj);
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
	//// �G�̒e�`��
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
void Boss::Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_bossModel->Update(elapsedTime, m_pBossAI->GetState());// ���f���̃A�j���[�V�����X�V
	m_pBossAI->Update(elapsedTime, m_position, playerPos, m_isHit, m_isHitToPlayerBullet);// AI�̍X�V
	m_audioManager->Update();// �I�[�f�B�I�}�l�[�W���[�̍X�V

	m_attackCooldown = m_pBossAI->GetBossAttack()->GetCoolTime();
	ShootBullet();// �e����

	m_enemyBullets->Update(elapsedTime, GetPosition());// �G�̒e�̍X�V
	m_enemyBS.Center = m_position;
	m_enemyBS.Center.y -= 1.0f;
	// �e�̈ʒu�ݒ�
	BulletPotsitioning(elapsedTime);
	// HPBar�X�V
	m_HPBar->Update(elapsedTime, m_currentHP);
	m_isDead = m_HPBar->GetIsDead();
}

// �I�u�W�F�N�g���m���Փ˂����牟���߂�
void Boss::CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B)
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
}

// �e����
void Boss::ShootBullet()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �U���̃N�[���_�E���^�C�����Ǘ�
	if (m_attackCooldown <= 0.1f)
	{
		m_audioManager->PlaySound("EnemyBullet", m_pPlayer->GetVolume());// �T�E���h�Đ� 
		// �N�H�[�^�j�I����������x�N�g�����v�Z
		Vector3 direction = Vector3::Transform(Vector3::Backward, m_pBossAI->GetRotation());

		// �e�𔭎�
		// �����̒e�𔭎�
		m_enemyBullets->CreateBullet(m_bulletPosCenter, direction, m_pPlayer->GetPlayerPos(),
			BULLET_SIZE, EnemyBullet::BulletType::SPIRAL);
		m_enemyBullets->SetRotateDirection(-1);// �����e�̉�]������ݒ�i�����

		// �p�x�����炵�č��E�̒e�𔭎�
		constexpr float angleOffset = XMConvertToRadians(30.0f); // 15�x�̊p�x�I�t�Z�b�g

		// ������
		Quaternion leftRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, angleOffset);
		Vector3 leftDirection = Vector3::Transform(direction, leftRotation);
		m_enemyBullets->CreateBullet(m_bulletPosLeft, direction, m_pPlayer->GetPlayerPos(),
			BULLET_SIZE, EnemyBullet::BulletType::SPIRAL);

		// �E����
		Quaternion rightRotation = Quaternion::CreateFromAxisAngle(Vector3::Up, -angleOffset);
		Vector3 rightDirection = Vector3::Transform(direction, rightRotation);
		m_enemyBullets->CreateBullet(m_bulletPosRight, direction, m_pPlayer->GetPlayerPos(),
			BULLET_SIZE, EnemyBullet::BulletType::SPIRAL);
		// �N�[���_�E���^�C�������Z�b�g
		m_pBossAI->GetBossAttack()->SetCoolTime(1.5f);
	}
}

// �e�̈ʒu�ݒ�
void Boss::BulletPotsitioning(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;


	// �e�̔��ˈʒu��ݒ�
	Matrix transform = Matrix::CreateFromQuaternion(m_pBossAI->GetRotation())
		* Matrix::CreateTranslation(m_position);
	// �����̍��W�ɉ�]��K�p
	m_bulletPosCenter = Vector3::Transform(Vector3(0, 2.5f, 3), transform);
	// ���̍��W�ɉ�]��K�p
	m_bulletPosLeft = Vector3::Transform(Vector3(-2.5f, 0, 3), transform);
	// �E�̍��W�ɉ�]��K�p
	m_bulletPosRight = Vector3::Transform(Vector3(2.5f, 0, 3), transform);
}
