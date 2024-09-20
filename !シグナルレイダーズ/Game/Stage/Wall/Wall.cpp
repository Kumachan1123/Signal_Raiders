#include "pch.h"

#include "Game/Stage/Wall/Wall.h"

const std::vector<D3D11_INPUT_ELEMENT_DESC>  Wall::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

Wall::Wall(CommonResources* resources)
	:m_commonResources(resources)
	, m_time(0.0f)
	, m_constBuffer()
	, m_pDR(nullptr)
{
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

	ID3D11Device1* device = pDR->GetD3DDevice();

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ��݁i2���Ƃ��f�t�H���g�͓ǂݍ��ݎ��s��nullptr)
	LoadTexture(L"Resources/Textures/Wall.png");

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_pPrimitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());

	m_pStates = std::make_unique<CommonStates>(device);

}
void Wall::CreateShader()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	ID3D11Device1* device = m_pDR->GetD3DDevice();
	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/VS_Title.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/TitleScene/PS_Title.cso");
	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
							  static_cast<UINT>(INPUT_LAYOUT.size()),
							  VSData.GetData(), VSData.GetSize(),
							  m_pInputLayout.GetAddressOf());

	//	���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//	�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_cBuffer);


}
void Wall::Update(float elapsedTime)
{
	m_time += elapsedTime;
}

void Wall::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	���_���i�ǂ̒��_�j 
	VertexPositionTexture wall[4][4] =
	{//												���W					UV���W�i�ӂ���0�`1�̊ԂŎw��B�������ꍇ�͌J��Ԃ�
		{
			VertexPositionTexture(SimpleMath::Vector3(100.0f,  10.0f,  100.0f),	SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f, 10.0f,  100.0f),	SimpleMath::Vector2(1.0f,0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f,  0.0f,  100.0f),	    SimpleMath::Vector2(1.0f,1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(100.0f,   0.0f,  100.0f),  	SimpleMath::Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(SimpleMath::Vector3(100.0f,  10.0f,  -100.0f),	SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f, 10.0f,  -100.0f),	SimpleMath::Vector2(1.0f,0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f,  0.0f,  -100.0f),	    SimpleMath::Vector2(1.0f,1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(100.0f,   0.0f,  -100.0f),  	SimpleMath::Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(SimpleMath::Vector3(100.0f,10.0f,  100.0f),	SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(100.0f,10.0f ,-100.0f),	SimpleMath::Vector2(1.0f,0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.0f ,-100.0f),	    SimpleMath::Vector2(1.0f,1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(100.0f, 0.0f,  100.0f),  	SimpleMath::Vector2(0.0f, 1.0f))
		},
		{
			VertexPositionTexture(SimpleMath::Vector3(-100.0f,10.0f,  100.0f),	SimpleMath::Vector2(0.0f, 0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f,10.0f ,-100.0f),	SimpleMath::Vector2(1.0f,0.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f ,-100.0f),	    SimpleMath::Vector2(1.0f,1.0f)),
			VertexPositionTexture(SimpleMath::Vector3(-100.0f, 0.0f,  100.0f),  	SimpleMath::Vector2(0.0f, 1.0f))
		}
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = view.Transpose();
	m_constBuffer.matProj = proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(0, 1.0f, 1.0f, 0.0f);
	m_constBuffer.time = m_time;
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &m_constBuffer, 0, 0);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�e�N�X�`����ݒ肷��
	context->PSSetShaderResources(0, 1, m_pWallTexture[0].GetAddressOf());
	context->IASetInputLayout(m_pInputLayout.Get());
	// �ǂ�`��
	m_pPrimitiveBatch->Begin();
	for (int i = 0; i < 4; i++)
	{
		m_pPrimitiveBatch->DrawQuad(wall[i][0], wall[i][1], wall[i][2], wall[i][3]);

	}
	m_pPrimitiveBatch->End();
	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

