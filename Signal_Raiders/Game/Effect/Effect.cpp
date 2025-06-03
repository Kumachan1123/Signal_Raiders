/*
	@file	Effect.cpp
	@brief	�G�t�F�N�g�N���X
*/
#include <pch.h>
#include "Effect.h"
/*
*	@brief	���̓��C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Effect::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const 	float Effect::m_vertexMinX = -1.0f;// �ŏ�X���W
const 	float Effect::m_vertexMaxX = 1.0f;// �ő�X���W
const 	float Effect::m_vertexMinY = -1.0f;// �ŏ�Y���W
const 	float Effect::m_vertexMaxY = 1.0f;// �ő�Y���W

/*
*	@brief	�R���X�g���N�^
*	@details �G�t�F�N�g�̏��������s��
*	@param resources ���ʃ��\�[�X
*	@param type �G�t�F�N�g�̎��
*	@param playPos �Đ�������W
*	@param scale �X�P�[��
*	@param world ���[���h�s��
*	@return	�Ȃ�
*/
Effect::Effect(CommonResources* resources, EffectType type, const DirectX::SimpleMath::Vector3& playPos, float scale, const DirectX::SimpleMath::Matrix& world)
	: m_position(playPos)// �G�t�F�N�g���Đ�������W
	, m_type(type)// �G�t�F�N�g�̎��
	, m_scale(scale)// �G�t�F�N�g�̃X�P�[��
	, m_pCommonResources(resources)// ���ʃ��\�[�X
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
		m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Effect")); // �e�N�X�`���̎擾
		m_frameRows = 4; // �摜�̍s��
		m_frameCols = 5; // �摜�̗�
		break;
	case EffectType::ENEMY_HIT:// �G�q�b�g�G�t�F�N�g
		m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Hit")); // �e�N�X�`���̎擾
		m_frameRows = 2; // �摜�̍s��
		m_frameCols = 4; // �摜�̗�
		break;
	}
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());// ���_���̏�����
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);// ���_�V�F�[�_�[
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);	// �s�N�Z���V�F�[�_�[
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g�̎擾
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �R���X�^���g�o�b�t�@����
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_
	m_isPlaying = true;	// �A�j���[�V�������Đ����ɂ���
}
/*
*	@brief	�f�X�g���N�^
*	@details �G�t�F�N�g�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Effect::~Effect() {}
/*
*	@brief	�摜��ǂݍ���
*	@details �w�肳�ꂽ�p�X����摜��ǂݍ��݁A�e�N�X�`���Ƃ��ĕۑ�����
*	@param path �摜�̃p�X
*	@return �Ȃ�
*/
void Effect::LoadTexture(const wchar_t* path)
{
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`��
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// �摜�̓ǂݍ���
	m_pTextures.push_back(texture);// �e�N�X�`���̒ǉ�
}
/*
*	@brief	�X�V����
*	@details �G�t�F�N�g�̃A�j���[�V�������X�V����
*	@param elapsedTime �o�ߎ���
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
*	@details �G�t�F�N�g��`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Effect::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���																	    
	m_vertices[0] = { VertexPositionTexture(Vector3(m_vertexMinX, m_vertexMaxY, 0), Vector2(0, 1)) };// ����
	m_vertices[1] = { VertexPositionTexture(Vector3(m_vertexMaxX, m_vertexMaxY, 0), Vector2(1, 1)) };// �E��
	m_vertices[2] = { VertexPositionTexture(Vector3(m_vertexMaxX,m_vertexMinY, 0),  Vector2(1, 0)) };// �E��
	m_vertices[3] = { VertexPositionTexture(Vector3(m_vertexMinX,m_vertexMinY, 0),  Vector2(0, 0)) }; // ����
	// �r���{�[�h�p�̒��_���
	VertexPositionTexture billboardVertex[4]{};
	// �r���{�[�h�̍����𒲐�
	for (int i = 0; i < 4; i++)
	{
		billboardVertex[i] = m_vertices[i];		// ���_�����R�s�[
		billboardVertex[i].position.x *= m_scale;// �X�P�[��
		billboardVertex[i].position.y *= m_scale;// �X�P�[��
		billboardVertex[i].position.y -= m_offSetY;// ����
	}
	// �r���{�[�h�s����쐬
	// �r���[�s��̋t�s��
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	// �r���{�[�h���A�t�B���ϊ�
	Matrix worldBillboard = m_world * billboardMatrix;
	// �r���{�[�h�̈ʒu��ݒ�
	worldBillboard *= Matrix::CreateTranslation(m_position);
	// �R���X�^���g�o�b�t�@�̍X�V
	// ���[���h�s���]�u���Đݒ�
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// �r���[�s���]�u���Đݒ�
	m_constBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����s���]�u���Đݒ�
	m_constBuffer.matProj = proj.Transpose();
	// �A�j���[�V�����̐ݒ�
	// �A�j���[�V�����̃R�}����ݒ�
	m_constBuffer.count = Vector4((float)(m_anim));
	// �e�N�X�`���̗��ݒ�
	m_constBuffer.height = Vector4((float)(m_frameRows));
	// �e�N�X�`���̍s��ݒ�
	m_constBuffer.width = Vector4((float)(m_frameCols));
	// �R���X�^���g�o�b�t�@�̍X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�ɃR���X�^���g�o�b�t�@��ݒ�
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �e�N�X�`���̃T���v���[�X�e�[�g(���j�A�A���b�v)
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g(���Z)
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g(�J�����O�Ȃ�)
		DrawPolygon::DepthStencilStates::DEPTH_READ);// �[�x�X�e���V���X�e�[�g(�ǂݎ��̂�)
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_�[�̓o�^
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �e�N�X�`���̕`��
	m_pDrawPolygon->DrawTexture(billboardVertex);
	// �V�F�[�_�[�̉��
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief	�I������
*	@details �G�t�F�N�g�̃��\�[�X���������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Effect::Finalize()
{
	// ���_�V�F�[�_�̉��
	m_pVertexShader.Reset();
	// �s�N�Z���V�F�[�_�̉��
	m_pPixelShader.Reset();
	// �C���v�b�g���C�A�E�g�̉��
	m_pInputLayout.Reset();
	// �R���X�^���g�o�b�t�@�̉��
	m_pCBuffer.Reset();
	// �e�N�X�`���̉��
	m_pTextures.clear();
}
