//--------------------------------------------------------------------------------------
// File: Crisis.cpp
//
// �댯��ԃN���X
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Crisis.h"

using namespace DirectX;

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Crisis::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

/// <summary>
/// �R���X�g���N�^
/// </summary>
Crisis::Crisis(CommonResources* resources)
	:m_pDR(nullptr)
	, m_time(0.0f)
	, m_constBuffer()
{
	m_commonResources = resources;
	// �F�̏�����
	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Crisis::~Crisis()
{
	DrawPolygon::ReleasePositionTexture();
}

/// <summary>
/// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
/// </summary>
/// <param name="path">���΃p�X(Resources/Textures/�E�E�E.png�Ȃǁj</param>
void  Crisis::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.ReleaseAndGetAddressOf());

	m_texture.push_back(texture);
}

/// <summary>
/// �����֐�
/// </summary>
/// <param name="pDR">���[�U�[���\�[�X�����玝���Ă���</param>
void  Crisis::Create(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	// �|���S���`��p
	DrawPolygon::InitializePositionTexture(m_pDR);

	//	�V�F�[�_�[�̍쐬
	CreateShader();

	//	�摜�̓ǂݍ���
	LoadTexture(L"Resources/Textures/crisis.png");



}

/// <summary>
/// Shader�쐬�����������������֐�
/// </summary>
void  Crisis::CreateShader()
{
	ID3D11Device1* device = m_pDR->GetD3DDevice();

	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/CRT/VS_CRT.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/CRT/PS_CRT.cso");

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

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

	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[���Z�b�g
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = nullptr;

}

//�X�V
void  Crisis::Update(float elapsedTime)
{
	//	���ԍX�V�im_time��0.1�����₵�A�P�𒴂�����O�����蒼���j
	m_time += elapsedTime;
	// �F�̍X�V
	// alpha�l��0.7f������������܂�0.9f�܂ŏ������グ��
	if (m_constBuffer.colors.w < 0.7f)
	{
		m_constBuffer.colors.w += 0.15f * elapsedTime;
	}
	else
	{
		// alpha�l��0.7f����0.9f�܂ł���������
		m_constBuffer.colors.w = 0.8f + 0.1f * sin(m_time);
	}



	m_constBuffer.colors = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, m_constBuffer.colors.w);

}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void  Crisis::Render()
{

	ID3D11DeviceContext1* context = m_pDR->GetD3DDeviceContext();
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(SimpleMath::Vector3(1.0, -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),
	};
	// �V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����(ConstBuffer)
	m_constBuffer.matView = m_view.Transpose();
	m_constBuffer.matProj = m_proj.Transpose();
	m_constBuffer.matWorld = m_world.Transpose();
	m_constBuffer.time = DirectX::SimpleMath::Vector4(m_time);
	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	//	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	DrawPolygon::SetShaderBuffer(context, 0, 1, cb);
	// �`�揀��
	DrawPolygon::DrawStart(context, m_inputLayout.Get(), m_texture);
	//	�V�F�[�_���Z�b�g����
	DrawPolygon::SetShader(context, m_shaders, nullptr, 0);
	// �|���S����`��
	DrawPolygon::DrawTexture(vertex);
	//	�V�F�[�_�̓o�^���������Ă���
	DrawPolygon::ReleaseShader(context);
}