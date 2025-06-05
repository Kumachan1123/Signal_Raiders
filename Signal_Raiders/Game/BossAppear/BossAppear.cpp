/*
*	@file BossAppear.cpp
*	@brief �{�X�o�����o�̃N���X�̃\�[�X�t�@�C��
*/
#include <pch.h>
#include "BossAppear.h"



/*
*	@brief �C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> BossAppear::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*
*  @brief �R���X�g���N�^
*  @param �Ȃ�
*  @return �Ȃ�
*/
BossAppear::BossAppear()
	: m_world(DirectX::SimpleMath::Matrix::Identity)// ���[���h�s��
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
	, m_timer(0.0f)// �^�C�}�[
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_constantBuffer{}// �R���X�^���g�o�b�t�@
	, m_pInputLayout{}// �C���v�b�g���C�A�E�g
	, m_pTextures{}// �e�N�X�`��
	, m_pCommonResources{}// �R�������\�[�X
	, m_seVolume(0.0f)// SE����
	, m_isPlaying(false)// �Đ��t���O
{
}

/*
*  @brief �f�X�g���N�^
*  @param �Ȃ�
*  @return �Ȃ�
*/
BossAppear::~BossAppear()
{
	// �I������
	Finalize();
}

/*
*  @brief �������֐�
*  @param resources �R�������\�[�X
*  @return �Ȃ�
*/
void BossAppear::Initialize(CommonResources* resources)
{
	// �R�������\�[�X�̎擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�̎擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	SettingShader();
	// �e�N�X�`���̎擾
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("BossAppear"));
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionTexture(m_pDR);

}


/*
*  @brief �V�F�[�_�[�̐ݒ�
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::SettingShader()
{
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/BossAppear/VS_BossAppear.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppear.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���擾
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �s�N�Z���V�F�[�_�[�쐬�i�w�i�p�j
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/BossAppear/PS_BossAppearBack.cso", m_pPixelShaderBack);
	// �萔�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	// ���_�V�F�[�_�[
	m_shaders.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
	m_shaders.gs = nullptr;
	// �w�i�p�V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	// ���_�V�F�[�_�[
	m_shadersBack.vs = m_pVertexShader.Get();
	// �s�N�Z���V�F�[�_�[
	m_shadersBack.ps = m_pPixelShaderBack.Get();
	// �W�I���g���V�F�[�_�[�͎g�p���Ȃ�
	m_shadersBack.gs = nullptr;
}

/*
*  @brief �X�V�֐�
*  @param elapsedTime �t���[������
*  @return �Ȃ�
*/
void BossAppear::Update(float elapsedTime)
{
	// ���ԉ��Z
	m_timer += elapsedTime;
	// �I�[�f�B�I�}�l�[�W���[�̍X�V
	m_pCommonResources->GetAudioManager()->Update();
	// �Đ��t���O�������Ă��Ȃ��ꍇ�A�{�X�o�ꉉ�o�����Đ�����
	if (!m_isPlaying)
	{
		// �Đ��t���O�𗧂Ă�
		m_isPlaying = !m_isPlaying;
		// �{�X�o�ꉉ�o�����Đ�
		m_pCommonResources->GetAudioManager()->PlaySound("BossAppear", m_seVolume);
	}

}

/*
*  @brief �`��֐�
*  @param view �r���[�s��
*  @param proj �v���W�F�N�V�����s��
*  @return �Ȃ�
*/
void BossAppear::Render()
{
	// �w�i�`��
	DrawBack();
	// ���C���e�N�X�`���`��
	DrawMain();
}
/*
*  @brief �I���֐�
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::Finalize()
{
	//�@do nothing
}

/*
*  @brief �`��֐�(���C���e�N�X�`��)
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::DrawMain()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  0.5f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  0.7f, 0.0f) , SimpleMath::Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(SimpleMath::Vector3(1.0, -0.5f, 0.0f)  , SimpleMath::Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(SimpleMath::Vector3(-1.0, -0.7f, 0.0f) , SimpleMath::Vector2(0.0f, 1.0f)),// ����
	};
	// 2�b�o�ߌ�Ɋg����J�n
	const float expandStartTime = 2.0f;  // �g��J�n����
	const float expandDuration = 0.5f;    // �g��ɂ����鎞�ԁi0.5�b�Ŋ����j
	// �g�嗦���v�Z(�����l��1.0f)
	float scaleFactor = 1.0f;
	// �g��J�n���Ԃ��߂�����g����J�n
	if (m_timer > expandStartTime)
	{
		// �g�厞�Ԃ��v�Z
		float t = (m_timer - expandStartTime) / expandDuration;
		// 1.0�𒴂��Ȃ��悤�ɃN�����v
		t = std::min(t, 1.0f);
		// 1.0 ���� 2.0 �փX���[�Y�Ɋg��
		scaleFactor = Lerp(1.0f, 5.0f, t * 10);
	}
	// ���_�̊g��
	vertex[0].position = SimpleMath::Vector3(-1.0f, 0.5f, 0.0f) * scaleFactor;// ����
	vertex[1].position = SimpleMath::Vector3(1.0f, 0.7f, 0.0f) * scaleFactor; // �E��
	vertex[2].position = SimpleMath::Vector3(1.0f, -0.5f, 0.0f) * scaleFactor;// �E��
	vertex[3].position = SimpleMath::Vector3(-1.0f, -0.7f, 0.0f) * scaleFactor;// ����
	// �R���X�^���g�o�b�t�@�̐ݒ�
	// �F�̐ݒ�
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// ���Ԃ̐ݒ�
	m_constantBuffer.time = Vector4(m_timer);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�o�b�t�@���Z�b�g����
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	m_pDrawPolygon->ReleaseShader();
}

/*
*  @brief �`��֐�(�w�i)
*  @param �Ȃ�
*  @return �Ȃ�
*/
void BossAppear::DrawBack()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(SimpleMath::Vector3(-1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(SimpleMath::Vector3(1.0f,  -1.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)), // �E��
		VertexPositionTexture(SimpleMath::Vector3(1.0f,1.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),	 // �E��
		VertexPositionTexture(SimpleMath::Vector3(-1.0f, 1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),	 // ����
	};
	// �R���X�^���g�o�b�t�@�̐ݒ�
	// �F�̐ݒ�
	m_constantBuffer.colors = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// ���Ԃ̐ݒ�
	m_constantBuffer.time = Vector4(m_timer);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�o�b�t�@���Z�b�g����
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_DEFAULT);// �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTextures);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shadersBack, nullptr, 0);
	//	�|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �`��I��
	m_pDrawPolygon->ReleaseShader();
}