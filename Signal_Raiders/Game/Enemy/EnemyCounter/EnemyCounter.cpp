/*
	@file	EnemyCounter.cpp
	@brief	�G�J�E���g�N���X(���݃V�F�[�_�[���g�p)
*/
#include "pch.h"
#include "EnemyCounter.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
EnemyCounter::EnemyCounter()
	:
	m_commonResources{},
	m_enemyIndex{ 0 },
	m_nowEnemy{ 0 },
	m_texture{},
	m_enemyIndex10{ 0 },
	m_enemyIndex1{ 0 },
	m_nowEnemy10{ 0 },
	m_nowEnemy1{ 0 },
	m_frameRows{ 1 },
	m_frameCols{ 10 },
	m_inputLayout{},
	m_states{},
	m_batchEffect{},
	m_verticesEnemyIndex10{},
	m_verticesEnemyIndex1{},
	m_verticesNowEnemy10{},
	m_verticesNowEnemy1{},
	m_verticesRemaining{},
	m_verticesSlash{}

{}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// �摜��ǂݍ���
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& tex)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	tex = texture;
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void EnemyCounter::Initialize(CommonResources* commonResources)
{
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �e�N�X�`���̓ǂݍ���
	LoadTexture(L"Resources/Textures/number.png", m_texture);//	����
	LoadTexture(L"Resources/Textures/remaining.png", m_remaining);//	�u�c��F�v
	LoadTexture(L"Resources/Textures/slash.png", m_slash);//	�u/�v
	// �v���~�e�B�u�o�b�`�쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	// �G�t�F�N�g�̍쐬 
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_batchEffect->SetReferenceAlpha(1); // 0�ł͂Ȃ�1�ɐݒ肵�Ċ��S�ȍ������O����

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	// ���ʃX�e�[�g����
	m_states = std::make_unique<CommonStates>(device);

	// �e���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		m_verticesEnemyIndex10[i] = {};
		m_verticesEnemyIndex1[i] = {};
		m_verticesNowEnemy10[i] = {};
		m_verticesNowEnemy1[i] = {};
		m_verticesRemaining[i] = {};
		m_verticesSlash[i] = {};
	}
}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
void EnemyCounter::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// �󂯎�����l����10�̈ʂ�1�̈ʂ��v�Z
	m_enemyIndex10 = m_enemyIndex / 10;
	m_enemyIndex1 = m_enemyIndex % 10;
	m_nowEnemy10 = m_nowEnemy / 10;
	m_nowEnemy1 = m_nowEnemy % 10;
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void EnemyCounter::Render()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();


	// �[�x�X�e���V����Ԃ�ݒ�i�[�x�o�b�t�@��L���ɂ���j

	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	DrawQuad(m_remaining, m_verticesRemaining, 0.4f, 1.0f, 0.25f, 0.18f, 0, 1, 1); // �u�c��F�v
	DrawQuad(m_slash, m_verticesSlash, 0.8f, 1.0f, 0.05f, 0.18f, 0, 1, 1);       // �u/�v
	DrawEnemyIndex1();// ������1�̈ʂ�`��
	DrawEnemyIndex10();// ������10�̈ʂ�`��
	DrawNowEnemy1();// ���݂̓G�̐���1�̈ʂ�`��
	DrawNowEnemy10();// ���݂̓G�̐���10�̈ʂ�`��
}



//---------------------------------------------------------
// ���݂̓G�̐���10�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy10()
{
	DrawQuad(
		m_texture, m_verticesNowEnemy10,
		0.66f, 1.0f, 0.08f, 0.18f, // �ʒu�ƃT�C�Y
		m_nowEnemy10, m_frameCols, m_frameRows // �t���[�����
	);
}

//---------------------------------------------------------
// ���݂̓G�̐���1�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy1()
{
	DrawQuad(
		m_texture, m_verticesNowEnemy1,
		0.72f, 1.0f, 0.08f, 0.18f, // �ʒu�ƃT�C�Y
		m_nowEnemy1, m_frameCols, m_frameRows // �t���[�����
	);
}

//---------------------------------------------------------
// ������10�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex10()
{
	DrawQuad(
		m_texture, m_verticesEnemyIndex10,
		0.86f, 1.0f, 0.08f, 0.18f, // �ʒu�ƃT�C�Y
		m_enemyIndex10, m_frameCols, m_frameRows // �t���[�����
	);

}

//---------------------------------------------------------
// ������1�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex1()
{
	DrawQuad(
		m_texture, m_verticesEnemyIndex1,
		0.92f, 1.0f, 0.08f, 0.18f, // �ʒu�ƃT�C�Y
		m_enemyIndex1, m_frameCols, m_frameRows // �t���[�����
	);
}

//---------------------------------------------------------
// �e�N�X�`����`��
//---------------------------------------------------------
void EnemyCounter::DrawQuad(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture, DirectX::VertexPositionTexture* vertices, float startX, float startY, float width, float height, int frameIndex, int frameCols, int frameRows)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �t���[���ʒu�̌v�Z
	int currentRow = frameIndex / frameCols;
	int currentCol = frameIndex % frameCols;

	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / frameCols;
	float vMin = static_cast<float>(currentRow) / frameRows;
	float uMax = static_cast<float>(currentCol + 1) / frameCols;
	float vMax = static_cast<float>(currentRow + 1) / frameRows;

	// ���_���W�̐ݒ�
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(uMin, vMin)) };
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(uMax, vMin)) };
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(uMax, vMax)) };
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(uMin, vMax)) };

	// �e�N�X�`���K�p�ƕ`��
	m_batchEffect->SetTexture(texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	m_primitiveBatch->End();
}
