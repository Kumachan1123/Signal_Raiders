/*
	@file	Effect.cpp
	@brief	�G�t�F�N�g�N���X
*/
#include "pch.h"
#include "Effect.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;


/*
*	@brief	���̓��C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const 	float Effect::m_vertexMinX = -1.0f;// �ŏ�X���W
const 	float Effect::m_vertexMaxX = 1.0f;// �ő�X���W
const 	float Effect::m_vertexMinY = -1.0f;// �ŏ�Y���W
const 	float Effect::m_vertexMaxY = 1.0f;// �ő�Y���W

/*
*	@brief	�R���X�g���N�^
*	@param[in] resources ���ʃ��\�[�X
*	@param[in] type �G�t�F�N�g�̎��
*	@param[in] playPos �Đ�������W
*	@param[in] scale �X�P�[��
*	@param[in] world ���[���h�s��
*	@return	�Ȃ�
*/
Effect::Effect(CommonResources* resources, EffectType type, DirectX::SimpleMath::Vector3 playPos, float scale, DirectX::SimpleMath::Matrix world)
	: m_position(playPos)// �G�t�F�N�g���Đ�������W
	, m_type(type)// �G�t�F�N�g�̎��
	, m_scale(scale)// �G�t�F�N�g�̃X�P�[��
	, m_commonResources(resources)// ���ʃ��\�[�X
	, m_world(world)// ���[���h�s��
	, m_isPlaying(false)// �Đ������̃t���O
	, m_anim(0)// �A�j���[�V�����̃R�}
	, m_animSpeed(30.0f)// �A�j���[�V�����̃X�s�[�h
	, m_animTime(0.0f)// �A�j���[�V�����̎���
	, m_frameRows{}// �摜�̍s��
	, m_frameCols{}// �摜�̗�
	, m_vertices{}// ���_���
	, m_offSetY(0.0f)// ����
	, m_pDrawPolygon(DrawPolygon::GetInstance())// �|���S���`��N���X
	, m_pCreateShader(CreateShader::GetInstance())// �V�F�[�_�[�쐬�N���X
{
	switch (m_type)// �G�t�F�N�g�̎�ނɂ���ĉ摜��ǂݍ���
	{
	case EffectType::ENEMY_DEAD:// �G���S�G�t�F�N�g
		LoadTexture(L"Resources/Textures/effect.png");// �e�N�X�`���̓ǂݍ���
		m_frameRows = 4; // �摜�̍s��
		m_frameCols = 5; // �摜�̗�
		break;
	case EffectType::ENEMY_HIT:// �G�q�b�g�G�t�F�N�g
		LoadTexture(L"Resources/Textures/hit.png");// �e�N�X�`���̓ǂݍ���
		m_frameRows = 2; // �摜�̍s��
		m_frameCols = 4; // �摜�̗�
		break;
	}
	m_pDrawPolygon->InitializePositionTexture(m_commonResources->GetDeviceResources());// ���_���̏�����
	m_pCreateShader->Initialize(m_commonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_vertexShader);// ���_�V�F�[�_�[
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pixelShader);	// �s�N�Z���V�F�[�_�[
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g�̎擾
	m_pCreateShader->CreateConstantBuffer(m_cBuffer, sizeof(ConstBuffer));// �R���X�^���g�o�b�t�@����
	m_shaders.vs = m_vertexShader.Get();// ���_�V�F�[�_
	m_shaders.ps = m_pixelShader.Get();// �s�N�Z���V�F�[�_
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_
	m_isPlaying = true;	// �A�j���[�V�������Đ����ɂ���
}
/*
*	@brief	�f�X�g���N�^
*	@return �Ȃ�
*/
Effect::~Effect() {}
/*
*	@brief	�摜��ǂݍ���
*	@param[in] path �摜�̃p�X
*	@return �Ȃ�
*/
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// �摜�̓ǂݍ���
	m_textures.push_back(texture);// �e�N�X�`���̒ǉ�
}
/*
*	@brief	�X�V����
*	@param[in] elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Effect::Update(float elapsedTime)
{
	m_animTime += elapsedTime * m_animSpeed;// �o�ߎ��Ԃ����Z
	if (m_animTime >= 1.0f)// 1�b�o��
	{
		m_anim++;// �A�j���̃R�}��E�߂�
		m_animTime = 0.0f;// �^�C�}�[�����Z�b�g
	}
	if (m_anim == m_frameRows * m_frameCols)m_isPlaying = false;// �Đ��I��
}

/*
*	@brief	�`�揈��
*	@param[in] view �r���[�s��
*	@param[in] proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Effect::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���_���
	m_vertices[0] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX, m_vertexMaxY, 0), SimpleMath::Vector2(0, 1)) };// ����
	m_vertices[1] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX, m_vertexMaxY, 0), SimpleMath::Vector2(1, 1)) };// �E��
	m_vertices[2] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMaxX,m_vertexMinY, 0), SimpleMath::Vector2(1, 0)) };// �E��
	m_vertices[3] = { VertexPositionTexture(SimpleMath::Vector3(m_vertexMinX,m_vertexMinY, 0), SimpleMath::Vector2(0, 0)) }; // ����
	VertexPositionTexture billboardVertex[4]{};// �r���{�[�h�p�̒��_���
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];		// ���_�����R�s�[
		billboardVertex[i].position.x *= m_scale;// �X�P�[��
		billboardVertex[i].position.y *= m_scale;// �X�P�[��
		billboardVertex[i].position.y -= m_offSetY;// ����
	}
	// �r���{�[�h�s����쐬
	Matrix billboardMatrix = view.Invert();// �r���[�s��̋t�s��
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	Matrix worldBillboard = m_world * billboardMatrix;	// �r���{�[�h���A�t�B���ϊ�
	worldBillboard *= Matrix::CreateTranslation(m_position);// �ʒu
	m_constBuffer.matWorld = worldBillboard.Transpose();// ���[���h�s��
	m_constBuffer.matView = view.Transpose();// �r���[�s��
	m_constBuffer.matProj = proj.Transpose();// �v���W�F�N�V�����s��
	m_constBuffer.count = Vector4((float)(m_anim));// �A�j���[�V�����J�E���g
	m_constBuffer.height = Vector4((float)(m_frameRows));// ����
	m_constBuffer.width = Vector4((float)(m_frameCols));// ��
	m_pDrawPolygon->UpdateSubResources(m_cBuffer.Get(), &m_constBuffer);// �R���X�^���g�o�b�t�@�̍X�V
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �e�N�X�`���̃T���v���[�X�e�[�g(���j�A�A���b�v)
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g(���Z)
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g(�J�����O�Ȃ�)
		DrawPolygon::DepthStencilStates::DEPTH_READ);// �[�x�X�e���V���X�e�[�g(�ǂݎ��̂�)
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_textures);	// �`�揀��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_�[�̓o�^
	m_pDrawPolygon->DrawTexture(billboardVertex);// �e�N�X�`���̕`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�[�̉��
}

/*
*	@brief	�I������
*	@return �Ȃ�
*/
void Effect::Finalize()
{
	m_vertexShader.Reset();// ���_�V�F�[�_�̉��
	m_pixelShader.Reset();// �s�N�Z���V�F�[�_�̉��
	m_pInputLayout.Reset();	// �C���v�b�g���C�A�E�g�̉��
	m_cBuffer.Reset();// �R���X�^���g�o�b�t�@�̉��
	m_textures.clear();// �e�N�X�`���̉��
}
