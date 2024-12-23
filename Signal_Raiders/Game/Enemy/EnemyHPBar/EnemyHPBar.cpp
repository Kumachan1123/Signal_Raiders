/*
	@file	EnemyHPBar.cpp
	@brief	�GHPBar�N���X
*/
#include "pch.h"
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/KumachiLib.h"
// �R���X�g���N�^
EnemyHPBar::EnemyHPBar()
	: m_commonResources{}, m_maxHP(100), m_currentHP(100), m_displayedHP(100), m_lerpSpeed(5.0f), m_scale(1.5f), m_isDead(false)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �r���{�[�h�̒��_����ݒ肷��
	m_hpbarVert[0] = { Vector3(-1.5f,3.25f,0.0f),Vector2(0.0f,0.0f) };	// ����
	m_hpbarVert[1] = { Vector3(1.5f,3.25f,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarVert[2] = { Vector3(-1.5f,3.0f,0.0f),Vector2(0.0f,1.0f) };	// ����
	m_hpbarVert[3] = { Vector3(1.5f,3.0f,0.0f),Vector2(1.0f,1.0f) };	// �E��
	// �r���{�[�h�̒��_����ݒ肷��
	m_hpbarBackVert[0] = { Vector3(-1.60f,3.27f,0.0f),Vector2(0.0f,0.0f) };	// ����
	m_hpbarBackVert[1] = { Vector3(1.60f,3.27f,0.0f),Vector2(1.0f,0.0f) };	// �E��
	m_hpbarBackVert[2] = { Vector3(-1.60f,2.98f,0.0f),Vector2(0.0f,1.0f) };	// ����
	m_hpbarBackVert[3] = { Vector3(1.60f,2.98f,0.0f),Vector2(1.0f,1.0f) };	// �E��

}
// �f�X�g���N�^
EnemyHPBar::~EnemyHPBar() {}
//---------------------------------------------------------
//// ����������
////---------------------------------------------------------
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetTextureEnabled(true);	// �e�N�X�`�����g�p����

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_pInputLayout.ReleaseAndGetAddressOf()
		)
	);
	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/EnemyHPBar.png",
			nullptr,
			m_gaugeTexture.ReleaseAndGetAddressOf()
		)
	);
	m_displayedHP = (float)(m_maxHP);
}
// �`��
void EnemyHPBar::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// �r���{�[�h�s����쐬
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;

	// �r���{�[�h���A�t�B���ϊ�
	Matrix worldBillboard{};
	worldBillboard = billboardMatrix;
	worldBillboard *= Matrix::CreateScale(m_scale);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));	// �B
	worldBillboard *= Matrix::CreateTranslation(pos);
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));	// �B


	// �|���S����`�悷�邽�߂̐ݒ�
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullCounterClockwise());
	context->IASetInputLayout(m_pInputLayout.Get());
	// HPbar(�w��)�`��///////////////////////////////////////////////////////////////////////////////
	m_basicEffect->SetWorld(worldBillboard);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->SetTexture(m_gaugeTexture.Get());
	m_basicEffect->SetColorAndAlpha(DirectX::Colors::Gray);
	m_basicEffect->Apply(context);

	// �r���{�[�h��`��
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_hpbarBackVert[0], m_hpbarBackVert[1], m_hpbarBackVert[3], m_hpbarBackVert[2]);
	m_primitiveBatch->End();
	// HPbar(��)�`��///////////////////////////////////////////////////////////////////////////////
	m_basicEffect->SetColorAndAlpha(DirectX::Colors::Lime);
	m_basicEffect->Apply(context);

	// �r���{�[�h��`��
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_hpbarVert[0], m_hpbarVert[1], m_hpbarVert[3], m_hpbarVert[2]);
	m_primitiveBatch->End();
}
// �X�V
void EnemyHPBar::Update(float elapsedTime, int currentHP)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	// ���݂�HP���X�V
	m_currentHP = currentHP;

	// HP�����炩�ɍX�V���邽�߂̐��`���
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);

	// �\��HP�Ɋ�Â���HP�������v�Z
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);

	// HP�o�[�̕����v�Z (��Ƃ��āA�ő啝��2.0f�Ƃ���)
	float maxBarWidth = 3.0f;
	float currentBarWidth = maxBarWidth * hpPercentage;

	// HP�o�[�̒��_��ݒ� (�o�[�̍��[�� -1.50f �Ƃ��āA�E�[�� 1.50f �ɂ���)
	m_hpbarVert[0].position.x = -1.50f;                    // ����
	m_hpbarVert[1].position.x = -1.50f + currentBarWidth;  // �E��
	m_hpbarVert[2].position.x = -1.50f;                    // ����
	m_hpbarVert[3].position.x = -1.50f + currentBarWidth;  // �E��

	if (m_hpbarVert[1].position.x <= -1.50f)
	{
		m_isDead = true;
	}

}
