/*
	@file	EnemyCounter.cpp
	@brief	�G�J�E���g�N���X
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



	////	�s�����̂ݕ`�悷��ݒ� 
	//m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	//m_batchEffect->SetReferenceAlpha(0);


	DrawRemaining();
	DrawEnemyIndex1();
	DrawEnemyIndex10();
	DrawSlash();
	DrawNowEnemy1();
	DrawNowEnemy10();
}



//---------------------------------------------------------
// ���݂̓G�̐���10�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy10()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	int currentRow = m_nowEnemy10 / m_frameCols;
	int currentCol = m_nowEnemy10 % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesNowEnemy10[0] = { VertexPositionTexture(SimpleMath::Vector3(0.66f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesNowEnemy10[1] = { VertexPositionTexture(SimpleMath::Vector3(0.74f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesNowEnemy10[2] = { VertexPositionTexture(SimpleMath::Vector3(0.74f, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesNowEnemy10[3] = { VertexPositionTexture(SimpleMath::Vector3(0.66f, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesNowEnemy10[0], m_verticesNowEnemy10[1], m_verticesNowEnemy10[2], m_verticesNowEnemy10[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// ���݂̓G�̐���1�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawNowEnemy1()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	int currentRow = m_nowEnemy1 / m_frameCols;
	int currentCol = m_nowEnemy1 % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesNowEnemy1[0] = { VertexPositionTexture(SimpleMath::Vector3(0.72f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesNowEnemy1[1] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesNowEnemy1[2] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesNowEnemy1[3] = { VertexPositionTexture(SimpleMath::Vector3(0.72f, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesNowEnemy1[0], m_verticesNowEnemy1[1], m_verticesNowEnemy1[2], m_verticesNowEnemy1[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// ������10�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex10()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	int currentRow = m_enemyIndex10 / m_frameCols;
	int currentCol = m_enemyIndex10 % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesEnemyIndex10[0] = { VertexPositionTexture(SimpleMath::Vector3(0.86, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesEnemyIndex10[1] = { VertexPositionTexture(SimpleMath::Vector3(0.94, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesEnemyIndex10[2] = { VertexPositionTexture(SimpleMath::Vector3(0.94, 0.82f, 0), SimpleMath::Vector2(uMax, vMax)) };
	m_verticesEnemyIndex10[3] = { VertexPositionTexture(SimpleMath::Vector3(0.86, 0.82f, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesEnemyIndex10[0], m_verticesEnemyIndex10[1], m_verticesEnemyIndex10[2], m_verticesEnemyIndex10[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// ������1�̈ʂ�`��
//---------------------------------------------------------
void EnemyCounter::DrawEnemyIndex1()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	int currentRow = m_enemyIndex1 / m_frameCols;
	int currentCol = m_enemyIndex1 % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesEnemyIndex1[0] = { VertexPositionTexture(SimpleMath::Vector3(0.92f, 1.0f, 0), SimpleMath::Vector2(uMin,vMin)) };
	m_verticesEnemyIndex1[1] = { VertexPositionTexture(SimpleMath::Vector3(1.0f, 1.0f, 0), SimpleMath::Vector2(uMax,vMin)) };
	m_verticesEnemyIndex1[2] = { VertexPositionTexture(SimpleMath::Vector3(1.0f, 0.82f, 0), SimpleMath::Vector2(uMax,vMax)) };
	m_verticesEnemyIndex1[3] = { VertexPositionTexture(SimpleMath::Vector3(0.92f, 0.82f, 0), SimpleMath::Vector2(uMin,vMax)) };
	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesEnemyIndex1[0], m_verticesEnemyIndex1[1], m_verticesEnemyIndex1[2], m_verticesEnemyIndex1[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// �u�̂���F�v��`��
//---------------------------------------------------------
void EnemyCounter::DrawRemaining()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesRemaining[0] = { VertexPositionTexture(SimpleMath::Vector3(0.4f, 1.0f, 0), SimpleMath::Vector2(0,0)) };
	m_verticesRemaining[1] = { VertexPositionTexture(SimpleMath::Vector3(0.65f, 1.0f, 0), SimpleMath::Vector2(1,0)) };
	m_verticesRemaining[2] = { VertexPositionTexture(SimpleMath::Vector3(0.65f, 0.82f, 0), SimpleMath::Vector2(1,1)) };
	m_verticesRemaining[3] = { VertexPositionTexture(SimpleMath::Vector3(0.4f, 0.82f, 0), SimpleMath::Vector2(0,1)) };
	m_batchEffect->SetTexture(m_remaining.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesRemaining[0], m_verticesRemaining[1], m_verticesRemaining[2], m_verticesRemaining[3]);
	m_primitiveBatch->End();
}

//---------------------------------------------------------
// �u/�v��`��
//---------------------------------------------------------
void EnemyCounter::DrawSlash()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_verticesSlash[0] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 1.0f, 0), SimpleMath::Vector2(0,0)) };
	m_verticesSlash[1] = { VertexPositionTexture(SimpleMath::Vector3(0.85f, 1.0f, 0), SimpleMath::Vector2(1,0)) };
	m_verticesSlash[2] = { VertexPositionTexture(SimpleMath::Vector3(0.85f, 0.82f, 0), SimpleMath::Vector2(1,1)) };
	m_verticesSlash[3] = { VertexPositionTexture(SimpleMath::Vector3(0.8f, 0.82f, 0), SimpleMath::Vector2(0,1)) };
	m_batchEffect->SetTexture(m_slash.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_verticesSlash[0], m_verticesSlash[1], m_verticesSlash[2], m_verticesSlash[3]);
	m_primitiveBatch->End();
}