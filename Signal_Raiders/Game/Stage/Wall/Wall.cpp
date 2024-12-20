#include "pch.h"
#include "Wall.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC>  Wall::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

Wall::Wall(CommonResources* resources)
	: m_commonResources(resources)
	, m_time(0.0f)
	, m_constBuffer()
	, m_wall()
	, m_pDR(nullptr)
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pWallTexture()
	, m_world(DirectX::SimpleMath::Matrix::Identity)
	, m_wallBox()
	, m_pCreateShader{ CreateShader::GetInstance() }
{
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);

}

Wall::~Wall() {  }

void  Wall::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_pWallTexture.push_back(texture);
}
void  Wall::Create(DX::DeviceResources* pDR)
{
	using namespace DirectX;
	m_pDR = pDR;
	//	�V�F�[�_�[�̍쐬
	CreateShader();

	// �ǂ̏�����
	Initialize();
	//	�摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/Wall.png");
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);
}
void Wall::CreateShader()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/TitleScene/VS_Title.cso", m_vertexShader);
	// �s�N�Z���V�F�[�_�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/TitleScene/PS_Title.cso", m_pixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;

}
void Wall::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	//	���_���i�ǂ̒��_�j 
	VertexPositionTexture wall[4][4] =
	{
		{
			VertexPositionTexture(Vector3(100.0f,  10.0f,  100.0f),	Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 10.0f,  100.0f),	Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f,  0.0f,  100.0f), Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f,   0.0f,  100.0f), Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(100.0f,  10.0f,  -100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 10.0f,  -100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f,  0.0f,  -100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f,   0.0f,  -100.0f),Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(100.0f,10.0f,  100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(100.0f,10.0f ,-100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(100.0f, 0.0f ,-100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(100.0f, 0.0f,  100.0f),Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(Vector3(-100.0f,10.0f,  100.0f),Vector2(0.0f, 0.0f)),
			VertexPositionTexture(Vector3(-100.0f,10.0f ,-100.0f),Vector2(1.0f,0.0f)),
			VertexPositionTexture(Vector3(-100.0f, 0.0f ,-100.0f),Vector2(1.0f,1.0f)),
			VertexPositionTexture(Vector3(-100.0f, 0.0f,  100.0f),Vector2(0.0f, 1.0f))
		}
	};
	// �ǂ̒��_���R�s�[
	for (int i = 0; i < 4; i++)for (int j = 0; j < 4; j++) m_wall[i][j] = wall[i][j];

	// �ǂ̒��S���W�����߂�
	m_wallBox[0].Center = Vector3(0.0f, 5.0f, 100.0f);
	m_wallBox[0].Extents = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[1].Center = Vector3(0.0f, 5.0f, -100.0f);
	m_wallBox[1].Extents = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[2].Center = Vector3(100.0f, 5.0f, 0.0f);
	m_wallBox[2].Extents = Vector3(0.0f, 5.0f, 100.0f);
	m_wallBox[3].Center = Vector3(-100.0f, 5.0f, 0.0f);
	m_wallBox[3].Extents = Vector3(0.0f, 5.0f, 100.0f);



}
void Wall::Update(float elapsedTime)
{
	m_time += elapsedTime;
}

void Wall::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,
		DrawPolygon::BlendStates::NONPREMULTIPLIED,
		DrawPolygon::RasterizerStates::CULL_NONE,
		DrawPolygon::DepthStencilStates::DEPTH_NONE);
	// �`��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pWallTexture);
	// �ǂ�`��
	for (int i = 0; i < 4; i++)	m_pDrawPolygon->DrawTexture(m_wall[i]);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

