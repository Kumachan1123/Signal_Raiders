/*
*	@file	EnemyCounter.cpp
*	@brief	�G�J�E���^�\�N���X
*/
#include <pch.h>
#include "EnemyCounter.h"
/*
*	@brief	�C���v�b�g���C�A�E�g
*	@return �Ȃ�
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyCounter::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@details �G�J�E���^�\�N���X�̃R���X�g���N�^
*	@param CommonResources* pCommonResources	���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
EnemyCounter::EnemyCounter()
	: m_pCommonResources{}// ���ʃ��\�[�X�ւ̃|�C���^
	, m_enemyIndex{ 0 }// �G�̑���(�ۑ��p�j
	, m_nowEnemy{ 0 }// ���݂̓G�̐�(�ۑ��p�j
	, m_pTexture{}//	�����摜
	, m_enemyIndex10{ 0 }// �G�̑���(10�̈�)
	, m_enemyIndex1{ 0 }// �G�̑���(1�̈�)
	, m_nowEnemy10{ 0 }// ���݂̓G�̐�(10�̈�)
	, m_nowEnemy1{ 0 }// ���݂̓G�̐�(1�̈�)
	, m_frameRows{ 1 }//	�摜�̍s��
	, m_frameCols{ 10 }//	�摜�̗�
	, m_pInputLayout{}// �C���v�b�g���C�A�E�g
	, m_verticesEnemyIndex10{}// ������10�̈�
	, m_verticesEnemyIndex1{}// ������1�̈�
	, m_verticesNowEnemy10{}// ���݂̓G�̐���10�̈�
	, m_verticesNowEnemy1{}// ���݂̓G�̐���1�̈�
	, m_verticesRemaining{}// �u�c��F�v
	, m_verticesSlash{}// �u/�v
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
{}
/*
*	@brief	�f�X�g���N�^
*	@details �G�J�E���^�\�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
EnemyCounter::~EnemyCounter() {/*do nothing*/ }
/*
*	@brief ������
*	@details �G�J�E���^�\�N���X�̏�����
*	@param CommonResources* pCommonResources	���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void EnemyCounter::Initialize(CommonResources* pCommonResources)
{
	// ���ʃ��\�[�X�ւ̃|�C���^�擾
	m_pCommonResources = pCommonResources;
	//	�����e�N�X�`���̎擾
	m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Number"));
	// �u�c��F�v�e�N�X�`���̎擾
	m_pRemaining.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Remaining"));
	// �u/�v�e�N�X�`���̎擾
	m_pSlash.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Slash"));
	// �|���S���`��N���X������
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_�[�쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Counter/VS_Counter.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Counter/PS_Counter.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[���Z�b�g�i�g��Ȃ��̂�nullptr�j
	m_shaders.gs = nullptr;
	// �e���_�̏�����
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		// ������10�̈�
		m_verticesEnemyIndex10[i] = {};
		// ������1�̈�
		m_verticesEnemyIndex1[i] = {};
		// ���݂̓G�̐���10�̈�
		m_verticesNowEnemy10[i] = {};
		// ���݂̓G�̐���1�̈�
		m_verticesNowEnemy1[i] = {};
		// �u�c��F�v
		m_verticesRemaining[i] = {};
		// �u/�v
		m_verticesSlash[i] = {};
	}
}
/*
*	@brief �X�V
*	@details �G�J�E���^�\�N���X�̍X�V����
*	@param float elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void EnemyCounter::Update(float elapsedTime)
{
	// ���g�p�ϐ��̌x���𖳎�
	UNREFERENCED_PARAMETER(elapsedTime);
	// �󂯎�����l����10�̈ʂ�1�̈ʂ��v�Z
	// �G�̑�����10�̈�
	m_enemyIndex10 = m_enemyIndex / 10;
	// �G�̑�����1�̈�
	m_enemyIndex1 = m_enemyIndex % 10;
	// ���݂̓G�̐���10�̈�
	m_nowEnemy10 = m_nowEnemy / 10;
	// ���݂̓G�̐���1�̈�
	m_nowEnemy1 = m_nowEnemy % 10;
}
/*
*	@brief �`��
*	@details �G�J�E���^�\�N���X�̕`�揈��
*	@param float elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void EnemyCounter::Render()
{
	// �u�c��F�v
	DrawQuad(m_pRemaining, m_verticesRemaining, REMAINING_POS_X, REMAINING_POS_Y, REMAINING_SIZE_X, REMAINING_SIZE_Y, 0, 1, 1);
	// �u/�v
	DrawQuad(m_pSlash, m_verticesSlash, SLASH_POS_X, SLASH_POS_Y, SLASH_SIZE_X, SLASH_SIZE_Y, 0, 1, 1);
	// ������1�̈ʂ�`��
	DrawEnemyIndex1();
	// ������10�̈ʂ�`��
	DrawEnemyIndex10();
	// ���݂̓G�̐���1�̈ʂ�`��
	DrawNowEnemy1();
	// ���݂̓G�̐���10�̈ʂ�`��
	DrawNowEnemy10();
}
/*
*	@brief	���݂̓G�̐���10�̈ʂ�`��
*	@details ���݂̓G�̐���10�̈ʂ�`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void EnemyCounter::DrawNowEnemy10()
{
	// ���݂̓G�̐���10�̈ʂ�`��
	DrawQuad(m_pTexture, m_verticesNowEnemy10,// �摜�ƒ��_���
		NOW_ENEMY_10_POS_X, NOW_ENEMY_10_POS_Y, NOW_ENEMY_SIZE_X, NOW_ENEMY_SIZE_Y, // �ʒu�ƃT�C�Y
		m_nowEnemy10, m_frameCols, m_frameRows // ���̓G�̐���10�̈ʂƃt���[�����
	);
}
/*
*	@brief	���݂̓G�̐���1�̈ʂ�`��
*	@details ���݂̓G�̐���1�̈ʂ�`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void EnemyCounter::DrawNowEnemy1()
{
	// ���݂̓G�̐���1�̈ʂ�`��
	DrawQuad(m_pTexture, m_verticesNowEnemy1,// �摜�ƒ��_���
		NOW_ENEMY_1_POS_X, NOW_ENEMY_1_POS_Y, NOW_ENEMY_SIZE_X, NOW_ENEMY_SIZE_Y, // �ʒu�ƃT�C�Y
		m_nowEnemy1, m_frameCols, m_frameRows  // ���̓G�̐���10�̈ʂƃt���[�����
	);
}
/*
*	@brief	�G�̑�����10�̈ʂ�`��
*	@details �G�̑�����10�̈ʂ�`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void EnemyCounter::DrawEnemyIndex10()
{
	// �G�̑�����10�̈ʂ�`��
	DrawQuad(m_pTexture, m_verticesEnemyIndex10,// �摜�ƒ��_���
		ENEMY_INDEX_10_POS_X, ENEMY_INDEX_10_POS_Y, ENEMY_INDEX_SIZE_X, ENEMY_INDEX_SIZE_Y, // �ʒu�ƃT�C�Y
		m_enemyIndex10, m_frameCols, m_frameRows // �G�̑�����10�̈ʂƃt���[�����
	);

}
/*
*	@brief	�G�̑�����1�̈ʂ�`��
*	@details �G�̑�����1�̈ʂ�`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void EnemyCounter::DrawEnemyIndex1()
{
	// �G�̑�����1�̈ʂ�`��
	DrawQuad(m_pTexture, m_verticesEnemyIndex1,// �摜�ƒ��_���
		ENEMY_INDEX_1_POS_X, ENEMY_INDEX_1_POS_Y, ENEMY_INDEX_SIZE_X, ENEMY_INDEX_SIZE_Y,// �ʒu�ƃT�C�Y
		m_enemyIndex1, m_frameCols, m_frameRows // �G�̑�����1�̈ʂƃt���[�����
	);
}
/*
*	@brief	Quad�`��
*	@details �|���S����`�悷��
*	@param std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture	�e�N�X�`��
*	@param DirectX::VertexPositionTexture* vertices	���_���
*	@param float startX	�J�n�ʒuX
*	@param float startY	�J�n�ʒuY
*	@param float width	��
*	@param float height	����
*	@param int frameIndex	�t���[���C���f�b�N�X
*	@param int frameCols	�t���[���̗�
*	@param int frameRows	�t���[���̍s��
*/
void EnemyCounter::DrawQuad(
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& texture,
	DirectX::VertexPositionTexture* vertices,
	float startX, float startY, float width, float height,
	int frameIndex, int frameCols, int frameRows)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���_���W�̐ݒ�
	vertices[0] = { VertexPositionTexture(Vector3(startX, startY, 0), Vector2(0, 0)) };// ����
	vertices[1] = { VertexPositionTexture(Vector3(startX + width, startY, 0), Vector2(1, 0)) };// �E��
	vertices[2] = { VertexPositionTexture(Vector3(startX + width, startY - height, 0), Vector2(1, 1)) };// �E��
	vertices[3] = { VertexPositionTexture(Vector3(startX, startY - height, 0), Vector2(0, 1)) };// ����
	// �R���X�^���g�o�b�t�@�ɓn���f�[�^��ݒ�
	// ���[���h�s���P�ʍs��ɐݒ�
	m_constBuffer.matWorld = Matrix::Identity;
	// �r���[�s���P�ʍs��ɐݒ�
	m_constBuffer.matView = Matrix::Identity;
	// �v���W�F�N�V�����s���P�ʍs��ɐݒ�
	m_constBuffer.matProj = Matrix::Identity;
	// �A�j���[�V�����̃R�}��ݒ�
	m_constBuffer.count = Vector4((float)(frameIndex));
	// ������ݒ�
	m_constBuffer.height = Vector4((float)(frameRows));
	// ����ݒ�
	m_constBuffer.width = Vector4((float)(frameCols));
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// ConstBuffer����ID3D11Buffer�ւ̕ϊ�
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �f�v�X�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertices);
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}