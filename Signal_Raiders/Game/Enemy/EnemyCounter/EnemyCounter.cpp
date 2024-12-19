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
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
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
	m_pInputLayout{},
	m_states{},
	m_batchEffect{},
	m_verticesEnemyIndex10{},
	m_verticesEnemyIndex1{},
	m_verticesNowEnemy10{},
	m_verticesNowEnemy1{},
	m_verticesRemaining{},
	m_verticesSlash{},
	m_pCreateShader{ CreateShader::GetInstance() },
	m_pDrawPolygon{ DrawPolygon::GetInstance() }
{}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
EnemyCounter::~EnemyCounter() {}

//---------------------------------------------------------
// �摜��ǂݍ���
//---------------------------------------------------------
void EnemyCounter::LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& tex)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	tex.push_back(texture);
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
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃZ�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;
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
	DrawQuad(m_remaining, m_verticesRemaining, 0.4f, 1.0f, 0.25f, 0.18f, 0, 1, 1); // �u�c��F�v
	DrawQuad(m_slash, m_verticesSlash, 0.8f, 1.0f, 0.05f, 0.18f, 0, 1, 1); // �u/�v
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
void EnemyCounter::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���_���W�̐ݒ�
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };
	m_constBuffer.matWorld = Matrix::Identity;
	m_constBuffer.matView = Matrix::Identity;
	m_constBuffer.matProj = Matrix::Identity;

	m_constBuffer.count = Vector4(frameIndex);
	m_constBuffer.height = Vector4(frameRows);
	m_constBuffer.width = Vector4(frameCols);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
