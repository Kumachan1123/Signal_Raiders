/*
*	@file Wall.cpp
*	@brief �ǃN���X
*/
#include <pch.h>
#include "Wall.h"
// �ǂ̕�
const float Wall::WALL_WIDTH = 100.0f;
// �ǂ̍���
const float Wall::WALL_HEIGHT = 5.0f;
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Wall::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details �ǂ̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
Wall::Wall(CommonResources* resources)
	: m_pCommonResources(resources)// ���ʃ��\�[�X
	, m_time(0.0f)// ����
	, m_constBuffer()// �V�F�[�_�[�ɓn�����
	, m_wall()// �ǂ̒��_
	, m_pDR(nullptr)// �f�o�C�X���\�[�X
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �`��N���X
	, m_pWallTexture()// �ǃe�N�X�`��
	, m_world(DirectX::SimpleMath::Matrix::Identity)// ���[���h�s��
	, m_wallBox()// �ǂ̓����蔻��
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
Wall::~Wall() {/*do nothing*/ }
/*
*	@brief	�e�N�X�`���̓ǂݍ���
*	@details �e�N�X�`���̓ǂݍ���
*	@param path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void  Wall::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`���ꎞ�ۑ��p�ϐ�
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_pWallTexture.push_back(texture);// �e�N�X�`����ۑ�
}
/*
*	@brief	������
*	@details �ǂ̏�����
*	@param pDR �f�o�C�X���\�[�X
*	@return �Ȃ�
*/
void  Wall::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	// �e�N�X�`���}�l�[�W���[���擾
	auto pTexture = m_pCommonResources->GetTextureManager();
	// �f�o�C�X���\�[�X��ۑ�
	m_pDR = pDR;
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �ǂ̏�����
	CreateWalls();
	// �ǂ̃e�N�X�`����ǉ� 
	m_pWallTexture.push_back(pTexture->GetTexture("Wall"));
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@details �ǂ̃V�F�[�_�[���쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wall::CreateShaders()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g��Ȃ��̂�nullptr���Z�b�g
	m_shaders.gs = nullptr;
}
/*
*	@brief	�ǂ̏�����
*	@details �ǂ̒��_��������������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Wall::CreateWalls()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	���_���i�ǂ̒��_�j 
	VertexPositionTexture wall[4][4] =
	{	// �v���C���[�̏����ʒu���猩��
		{// ���̕�
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,  Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),	Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),	Vector2(1.0f,0.0f)),// �E��
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,  0.0f,  Wall::WALL_WIDTH), Vector2(1.0f,1.0f)),// �E��
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,   0.0f,  Wall::WALL_WIDTH), Vector2(0.0f, 1.0f))// ����
		},
		{// �O�̕�
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,  Wall::WALL_HEIGHT * 2,  -Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT * 2,  -Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// �E��
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,  0.0f,  -Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// �E��
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,   0.0f,  -Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// ����
		},
		{// �E�̕�
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2 ,-Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// �E��
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH, 0.0f ,-Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// �E��
			VertexPositionTexture(Vector3(Wall::WALL_WIDTH, 0.0f,  Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// ����
		},
		{// ���̕�
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2,  Wall::WALL_WIDTH),Vector2(0.0f, 0.0f)),// ����
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH,Wall::WALL_HEIGHT * 2 ,-Wall::WALL_WIDTH),Vector2(1.0f,0.0f)),// �E��
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, 0.0f ,-Wall::WALL_WIDTH),Vector2(1.0f,1.0f)),// �E��
			VertexPositionTexture(Vector3(-Wall::WALL_WIDTH, 0.0f,  Wall::WALL_WIDTH),Vector2(0.0f, 1.0f))// ����
		}
	};

	// �ǂ̒��_���R�s�[
	for (int i = 0; i < WALL_NUM; i++)for (int j = 0; j < WALL_NUM; j++) m_wall[i][j] = wall[i][j];

	// �ǂ̒��S���W�����߂�
	// ���̕ǂ̒��S���W
	m_wallBox[0].Center = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);
	// ���̕ǂ̑傫��
	m_wallBox[0].Extents = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);
	// �O�̕ǂ̒��S���W
	m_wallBox[1].Extents = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);
	// �O�̕ǂ̑傫��
	m_wallBox[1].Center = Vector3(0.0f, Wall::WALL_HEIGHT, -Wall::WALL_WIDTH);
	// �E�̕ǂ̒��S���W
	m_wallBox[2].Center = Vector3(Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);
	// �E�̕ǂ̑傫��
	m_wallBox[2].Extents = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);
	// ���̕ǂ̒��S���W
	m_wallBox[3].Center = Vector3(-Wall::WALL_WIDTH, Wall::WALL_HEIGHT, 0.0f);
	// ���̕ǂ̑傫��
	m_wallBox[3].Extents = Vector3(0.0f, Wall::WALL_HEIGHT, Wall::WALL_WIDTH);
}
/*
*	@brief	�X�V
*	@details �ǂ̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Wall::Update(float elapsedTime)
{
	// ���Ԃ��J�E���g
	m_time += elapsedTime;
}
/*
*	@brief	�`��
*	@details �ǂ̕`��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Wall::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	// �r���[�s���]�u���ăZ�b�g
	m_constBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����s���]�u���ăZ�b�g
	m_constBuffer.matProj = proj.Transpose();
	// ���[���h�s���]�u���ăZ�b�g
	m_constBuffer.matWorld = m_world.Transpose();
	// �F�̐ݒ�
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);
	// ���Ԃ̐ݒ�
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	// �`��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pWallTexture);
	// �ǂ�`��
	for (int i = 0; i < WALL_NUM; i++)	m_pDrawPolygon->DrawTexture(m_wall[i]);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}