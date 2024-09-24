/*
	@file	EnemyCounter.cpp
	@brief	�G�J�E���g�N���X
	�쐬�ҁF���܂�
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
	m_vertices{},
	m_vertexMinX{ 0.5f },
	m_vertexMaxX{ 1.0f },
	m_vertexMinY{ 0.7f },
	m_vertexMaxY{ 1.0f }

{}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// �摜��ǂݍ���
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	m_texture = texture;
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void EnemyCounter::Initialize(CommonResources* commonResources)
{
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	//	assert(commonResources);

	LoadTexture(L"Resources/Textures/number.png");


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
	int currentRow = m_enemyIndex1 / m_frameCols;
	int currentCol = m_enemyIndex1 % m_frameCols;



	// �e�N�X�`�����W�̌v�Z
	float uMin = static_cast<float>(currentCol) / m_frameCols;
	float vMin = static_cast<float>(currentRow) / m_frameRows;
	float uMax = static_cast<float>(currentCol + 1) / m_frameCols;
	float vMax = static_cast<float>(currentRow + 1) / m_frameRows;

	// ���_���
	// ���_���i�|���S���̒��_�j���㉺���]
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMax)) };
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(uMin, vMin)) };
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMin)) };
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMinY, 0), SimpleMath::Vector2(uMax, vMax)) };
	// �v���~�e�B�u�o�b�`�쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
	// �[�x�X�e���V����Ԃ�ݒ�i�[�x�o�b�t�@��L���ɂ���j

	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
	context->OMSetBlendState(m_states->NonPremultiplied(), nullptr, 0xFFFFFFFF);

	// �J�����O�͍�����i�����v���j
	context->RSSetState(m_states->CullCounterClockwise());

	// �e�N�X�`���T���v���[�̐ݒ�i�N�����v�e�N�X�`���A�h���b�V���O���[�h�j
	ID3D11SamplerState* samplers[1] = { m_states->AnisotropicWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	�s�����̂ݕ`�悷��ݒ� 
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	m_batchEffect->SetReferenceAlpha(0);

	m_batchEffect->SetTexture(m_texture.Get());
	m_batchEffect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());

	//	������������`�� 
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[2], m_vertices[3]);
	m_primitiveBatch->End();
}