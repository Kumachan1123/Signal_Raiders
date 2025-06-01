/*
*	@file	UI.cpp
*	@brief	UI�N���X(�^�C�g���E���U���g�E�Z�b�e�B���O)
*/
#include <pch.h>
#include "UI.h"

// UI�N���X�̃C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details UI�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
UI::UI(CommonResources* pCommonResources)
	: m_pDR{ nullptr }// �f�o�C�X���\�[�X
	, m_pCommonResources{ pCommonResources }// ���ʃ��\�[�X�ւ̃|�C���^
	, m_time{ 0 }// ����
	, m_windowWidth{ 0 }// �E�B���h�E�̕�
	, m_windowHeight{ 0 }// �E�B���h�E�̍���
	, m_textureWidth{ 0 }// �e�N�X�`���̕�
	, m_textureHeight{ 0 }// �e�N�X�`���̍���
	, m_scale{ DirectX::SimpleMath::Vector2::One }// �X�P�[��
	, m_baseScale{ DirectX::SimpleMath::Vector2::One }// ��X�P�[��
	, m_position{ DirectX::SimpleMath::Vector2::Zero }// �ʒu
	, m_anchor{ KumachiLib::ANCHOR::TOP_LEFT }// �A���J�[
	, m_shaderType{ ShaderType::NORMAL }// �V�F�[�_�[�̎��
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief	�f�X�g���N�^
*	@details �������Ȃ�
*	@param �Ȃ�
*	@return �Ȃ�
*/
UI::~UI() {/*do nothing*/ }
/*
*	@brief	�e�N�X�`���̓ǂݍ���
*	@details �e�N�X�`���̓ǂݍ���
*	@param key �e�N�X�`���̃L�[
*	@return �Ȃ�
*/
void UI::LoadTexture(std::string key)
{
	m_pTexture = m_pCommonResources->GetTextureManager()->GetTexture(key);// �e�N�X�`���}�l�[�W���[����e�N�X�`�����擾
	// �T�C�Y�擾�̂��߂̏���
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;// �ꎞ�ۑ��p���\�[�X�n���h��
	m_pTexture->GetResource(resource.GetAddressOf());// �e�N�X�`�����烊�\�[�X���擾
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;// �ꎞ�ۑ��p�e�N�X�`���n���h��
	DX::ThrowIfFailed(resource.As(&texture2D));// ���\�[�X���e�N�X�`���ɕϊ�
	// �e�N�X�`���̏����擾
	D3D11_TEXTURE2D_DESC desc;// �e�N�X�`���̏����i�[����\����
	texture2D->GetDesc(&desc);// �e�N�X�`���̏����擾
	m_pTextures.push_back(m_pTexture.Get()); // �e�N�X�`����z��ɒǉ�
	m_textureWidth = desc.Width; // �e�N�X�`���̕����擾
	m_textureHeight = desc.Height; // �e�N�X�`���̍������擾
}
/*
*	@brief	������
*	@details UI�̏�����
*	@param pDR �f�o�C�X���\�[�X
*	@param key �e�N�X�`���̃L�[
*	@param position UI�̈ʒu
*	@param scale UI�̃X�P�[��
*	@param anchor UI�̃A���J�[
*	@return �Ȃ�
*/
void UI::Create(DX::DeviceResources* pDR, std::string key
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, KumachiLib::ANCHOR anchor)
{

	m_pDR = pDR;// �f�o�C�X���\�[�X
	m_position = position;// �ʒu
	m_baseScale = m_scale = scale;// �X�P�[��
	m_anchor = anchor;// �A���J�[
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], // �V�F�[�_�[�쐬�N���X�̏�����
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	CreateShaders();// �V�F�[�_�[�̍쐬
	LoadTexture(key);// �e�N�X�`����ǂݍ���
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);	// �|���S���`��p
}
/*
*	@brief	�����蔻��
*	@details �}�E�X�̍��W��UI�͈͓̔��ɂ��邩�𔻒肷��
*	@param pos �}�E�X�̍��W
*	@return true:�����蔻�肠��, false:�����蔻��Ȃ�
*/
bool UI::IsHit(const DirectX::SimpleMath::Vector2& pos) const
{

	DirectX::SimpleMath::Vector2 leftTop = // �摜�̍���̍��W���擾
		m_position - DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.x / 2;
	DirectX::SimpleMath::Vector2 rightBottom =// �摜�̉E���̍��W���擾
		m_position + DirectX::SimpleMath::Vector2(float(m_textureWidth), float(m_textureHeight)) * m_scale.y / 2;

	if (leftTop.x <= pos.x && pos.x <= rightBottom.x	// �摜�̍���̍��W�ƉE���̍��W���r���āA
		&& leftTop.y <= pos.y && pos.y <= rightBottom.y)// 	�}�E�X�̍��W���͈͓��ɂ��邩�𔻒肷��
		return true;// �����蔻�肠��
	return false;// �����蔻��Ȃ�
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@details �e��V�F�[�_�[���쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
void UI::CreateShaders()
{
	// �V�F�[�_�[���쐬����
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Menu/VS_Menu.cso", m_pVertexShader);// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Menu/GS_Menu.cso", m_pGeometryShader);// �W�I���g���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_Menu.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Menu/PS_StageMenu.cso", m_pPixelShaderStageSelect);// �s�N�Z���V�F�[�_�쐬(�X�e�[�W�Z���N�g�p)
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.gs = m_pGeometryShader.Get();// �W�I���g���V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	// �X�e�[�W�Z���N�g�p�̃V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_StageSelectShaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_StageSelectShaders.gs = m_pGeometryShader.Get();// �W�I���g���V�F�[�_�[
	m_StageSelectShaders.ps = m_pPixelShaderStageSelect.Get();// �s�N�Z���V�F�[�_�[
}
/*
*	@brief	�X�V
*	@details UI�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void UI::Update(float elapsedTime) { m_time += elapsedTime; }
/*
*	@brief	�`��
*	@details UI�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void UI::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionColorTexture vertex[1] = {// ���_���
		VertexPositionColorTexture(Vector3(m_scale.x, m_scale.y, static_cast<float>(m_anchor))// �傫���ƃA���J�[
		, Vector4(m_position.x, m_position.y, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))// �ʒu�ƕ��ƍ���
		, Vector2(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight)))// �E�B���h�E�̕��ƍ���
	};
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����
	m_constBuffer.windowSize = Vector4(static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 1, 1);// �E�B���h�E�T�C�Y
	m_constBuffer.time = m_time;// ����
	m_constBuffer.color = Vector3(0.5, 0.5, 0.5);// �F
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �[�x�X�e���V���X�e�[�g
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);// �`��J�n
	if (m_shaderType == ShaderType::NORMAL)	// ���ʂ̃��j���[��������
		m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);			//	���ʂ̃V�F�[�_���Z�b�g����
	else									// �X�e�[�W�Z���N�g��������
		m_pDrawPolygon->SetShader(m_StageSelectShaders, nullptr, 0);// 	�X�e�[�W�Z���N�g�p�̃V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���

}
/*
*	@brief	�E�B���h�E�̃T�C�Y��ݒ�
*	@details �E�B���h�E�̃T�C�Y��ݒ�
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void UI::SetWindowSize(const int& width, const int& height)
{
	m_windowWidth = width;// �E�B���h�E�̕�
	m_windowHeight = height;// �E�B���h�E�̍���
}
