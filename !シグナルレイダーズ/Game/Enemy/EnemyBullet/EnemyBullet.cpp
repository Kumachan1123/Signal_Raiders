#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::EnemyBullet()
	:m_position{}
	, m_velocity{}
	, m_commonResources{}
	, m_time(0.0f)
{
}
//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
EnemyBullet::~EnemyBullet()
{
}
void EnemyBullet::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
	/*
		�f�o�b�O�h���[�̕\���p�I�u�W�F�N�g�𐶐�����
	*/
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
	// ���f����ǂݍ��ޏ���
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// ���f����ǂݍ���
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Bullet.cmo", *fx);
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
						   {
							   // �x�C�V�b�N�G�t�F�N�g���擾����
							   auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
							   // �f�B�t���[�Y�J���[��ݒ肷��
							   basicEffect->SetDiffuseColor(DirectX::Colors::Tomato);
							   // �X�y�L�����J���[��ݒ肷��
							   basicEffect->SetSpecularColor(DirectX::Colors::Tomato);
							   // �X�y�L�����p���[��ݒ肷��
							   basicEffect->SetSpecularPower(500.0f);
							   // �G�~�b�V�����J���[��ݒ肷��
							   basicEffect->SetEmissiveColor(DirectX::XMVECTOR{ 0.3, 0.3, 0.3, 1 });
						   });
	m_direction = Vector3::Zero;
	m_velocity = Vector3{ 0.0f,0.0f,0.0f };
	m_position = Vector3::Zero;
	m_boundingSphere.Center = m_position;
	m_boundingSphere.Radius = .25;
}
// �e�̏����ʒu��ݒ�
void EnemyBullet::MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target)
{
	using namespace DirectX::SimpleMath;
	m_position = pos;
	m_position.y -= 2.5f;
	m_direction = dir;
	m_target = target;
}

// �X�V
void EnemyBullet::Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime)
{
	
	// �v���C���[�̕����x�N�g�����v�Z
	DirectX::SimpleMath::Vector3 toPlayer = m_target - pos;
	
	if (toPlayer.LengthSquared() > 0)
	{
		toPlayer.Normalize();
	}
	// �e�̑��x��x������
	float bulletSpeed = .25f; // �K���ȑ��x��ݒ肷��i�C�ӂ̒l�A�����\�j
	m_velocity = toPlayer * bulletSpeed;
	// �v���C���[�̕����Ɍ������Ēe���΂�
	//m_velocity = toPlayer;
	m_position += m_velocity;
	m_position.y += 0.01f;
	m_boundingSphere.Center = m_position;
	m_time += elapsedTime;

}
void EnemyBullet::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �e�̃T�C�Y��ݒ�
	Matrix bulletWorld = Matrix::CreateScale(SIZE);
	Matrix boundingbulletWorld = Matrix::CreateScale(Vector3::One);
	// �e�̍��W��ݒ�
	bulletWorld *= Matrix::CreateTranslation(m_position);
	boundingbulletWorld *= Matrix::CreateTranslation(m_position);
	// �e�`��
	m_model->Draw(context, *states, bulletWorld, view, proj);

	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	// ���E���̕ϊ��𓯂����[���h�}�g���b�N�X�Ɋ�Â��čs��
	DirectX::BoundingSphere transformedBoundingSphere = m_boundingSphere;
	m_boundingSphere.Transform(transformedBoundingSphere, boundingbulletWorld);

	// �`�悷��
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_boundingSphere, DirectX::Colors::Red);
	m_primitiveBatch->End();
}
