/*
*	@file	Radar.cpp
*	@brief	���[�_�[�N���X
*/
#include <pch.h>
#include "Radar.h"
/*
*	@brief	�C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Radar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const float Radar::RADAR_SIZE_L = 0.5f;	// ���[�_�[�̍��[
const float Radar::RADAR_SIZE_R = 1.0f;	// ���[�_�[�̉E�[
const float Radar::RADAR_SIZE_T = 0.16f;	// ���[�_�[�̏�[
const float Radar::RADAR_SIZE_B = 1.0f;	// ���[�_�[�̉��[
const float Radar::ENEMY_SIZE_W = 0.01f;	// �G�̓_�̕�
const float Radar::ENEMY_SIZE_H = 0.02f;	// �G�̓_�̍���
const float Radar::RADAR_POSITION_X = 0.745f;	// ���[�_�[�̈ʒuX
const float Radar::RADAR_POSITION_Y = -0.55f;	// ���[�_�[�̈ʒuY
const float Radar::PLAYER_SIZE_X = 0.018f;	// �v���C���[��X�T�C�Y
const float Radar::PLAYER_SIZE_Y = -0.032f;	// �v���C���[��Y�T�C�Y
const float Radar::ENEMY_SIZE_X = 0.027f;	// �G��X�T�C�Y
const float Radar::ENEMY_SIZE_Y = -0.048f;	// �G��Y�T�C�Y
const float Radar::RADAR_RANGE = 300.0f;	// ���[�_�[�͈̔�
const float Radar::DISTANCE = 125.0f;	// �v���C���[�Ƃ̋���
/*
*	@brief	�R���X�g���N�^
*	@details ���[�_�[�N���X�̃R���X�g���N�^
*	@param	commonResources	���ʃ��\�[�X
*	@return �Ȃ�
*/
Radar::Radar(CommonResources* commonResources)
	: m_pCommonResources{ commonResources }// ���ʃ��\�[�X
	, m_pPlayer{ nullptr }// �v���C���[
	, m_pEnemyManager{ nullptr }// �G�}�l�[�W���[
	, m_radarPos{ Radar::RADAR_POSITION_X , Radar::RADAR_POSITION_Y }// ���[�_�[�̈ʒu
	, m_playerSize{ Radar::PLAYER_SIZE_X,Radar::PLAYER_SIZE_Y }// �v���C���[�̃T�C�Y
	, m_enemySize{ Radar::ENEMY_SIZE_X,Radar::ENEMY_SIZE_Y }// �G�̃T�C�Y
	, m_range{ Radar::RADAR_RANGE }// ���[�_�[�͈̔�
	, m_distance{ Radar::DISTANCE }// �v���C���[�Ƃ̋���
	, m_time{ 0.0f }// ����							
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), // �V�F�[�_�[�쐬�N���X�̏�����
		&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
}
/*
*	@brief	�f�X�g���N�^
*	@details ���[�_�[�N���X�̃f�X�g���N�^
*	@param	�Ȃ�
*	@return �Ȃ�
*/
Radar::~Radar()
{
	m_pDrawPolygon->ReleasePositionTexture();// �`��N���X�̉��
}
/*
*	@brief	�e�N�X�`���̓ǂݍ���
*	@details �e�N�X�`���̓ǂݍ���
*	@param	path	�e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void Radar::LoadTexture(const wchar_t* path)
{
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`�����i�[����|�C���^
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`����ǂݍ���
	m_pTextures.push_back(texture);// �e�N�X�`�����i�[
}
/*
*	@brief	������
*	@details ���[�_�[�N���X�̏�����
*	@param	pPlayer	�v���C���[
*	@param	pEnemyManager	�G�}�l�[�W���[
*	@return �Ȃ�
*/
void Radar::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;// �v���C���[�̃|�C���^�[��n��
	m_pEnemyManager = pEnemyManager;// �G�}�l�[�W���[�̃|�C���^�[��n��
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());// �|���S���`�揀��
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("RadarBack"));// �w�i
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("PlayerPin"));// �v���C���[�̃s��
	m_pTextures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyPin"));// �G�̃s��
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_pVertexShader); // ���_�V�F�[�_
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pPixelShader); // �s�N�Z���V�F�[�_
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = nullptr;// �W�I���g���V�F�[�_�[(�g��Ȃ��̂�nullptr)
}
/*
*	@brief	�X�V
*	@details ���[�_�[�N���X�̍X�V
*	@param	elapsedTime	�o�ߎ���
*	@return �Ȃ�
*/
void Radar::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_time += elapsedTime;// ����
	m_playerPos = m_pPlayer->GetPlayerPos();// �v���C���[�̈ʒu���擾
	m_enemyPos.clear();// �G�̈ʒu���N���A
	for (auto& enemy : m_pEnemyManager->GetEnemies())// �G�̈ʒu���擾
	{
		Vector3 enemyPos = enemy->GetPosition();// �G�̈ʒu���擾
		float distance = Vector3::Distance(m_playerPos, enemyPos); // �v���C���[�Ƃ̋������v�Z
		if (distance <= m_range)// ���[�_�[�͈͓��ɂ���G�����ۑ�
			m_enemyPos.push_back(enemyPos);// �G�̈ʒu��ۑ�
	}
}
/*
*	@brief	�`��
*	@details ���[�_�[�N���X�̕`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::Render()
{
	// ���[�_�[�`��
	DrawBackground();// �w�i�`��
	DrawPlayer();// �v���C���[�`��
	DrawEnemy();// �G�`��
}
/*
*	@brief	�w�i�`��
*	@details ���[�_�[�N���X�̔w�i�`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::DrawBackground()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture vertex[4] =
	{
		//										���_���								UV���
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),// ����

	};
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(���Â߂̗�)
	CreateBuffer();// �o�b�t�@���쐬
	DrawSetting();// �`��O�ݒ�
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;	// �摜�����n��
	textures.push_back(m_pTextures[(int)(RadarState::Background)].Get());// �w�i�摜
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);// �`�揀��
	m_pDrawPolygon->DrawTexture(vertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}

//---------------------------------------------------------
// �v���C���[�`��
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	VertexPositionTexture playerVertex[4] =
	{
		//														���_���													UV���
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(�Â߂̗΁j 
	CreateBuffer();// �o�b�t�@���쐬
	DrawSetting();// �`��O�ݒ�
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;// �摜�����n��
	textures.push_back(m_pTextures[(int)(RadarState::Player)].Get());// �v���C���[�摜
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);// �`�揀��
	m_pDrawPolygon->DrawTexture(playerVertex);// �|���S����`��
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}
/*
*	@brief	�G�`��
*	@details ���[�_�[�N���X�̓G�`��
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::DrawEnemy()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	m_constBuffer.colors = Vector4(0.9f, 0.0f, 0.0f, 0);// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(��)
	CreateBuffer();// �o�b�t�@���쐬
	for (const auto& enemyPos : m_enemyPos)	// �G�̈ʒu��`��	 
	{
		Vector3 relativePos = enemyPos - m_playerPos;// �v���C���[����G�ւ̑��Έʒu���v�Z
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir();// �J�����̌������擾
		// �J�����̌����Ɋ�Â��ĉ�]�s����쐬
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z); // �J�����̑O���x�N�g�������]�p���v�Z
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));// ��]�𒲐�
		rotationMatrix *= Matrix::CreateRotationY(playerRotation); // Y����]
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);// �G�̈ʒu���v���C���[�̈ʒu�����ɂ��ĉ�]
		Vector2 radarPos = Vector2(// ���[�_�[�̒��S����̑��Έʒu���v�Z
			rotatedPos.x / m_range * (RADAR_SIZE_L - RADAR_SIZE_R), // �~�j�}�b�v�̉���
			rotatedPos.z / m_range * (RADAR_SIZE_B - RADAR_SIZE_T) // �~�j�}�b�v�̍���
		);
		radarPos += m_radarPos; // �~�j�}�b�v�̒��S�ʒu�����Z
		// �~�j�}�b�v�͈͓̔��ɂ��邩���m�F
		if (radarPos.x > RADAR_SIZE_L + m_enemySize.x && radarPos.x < RADAR_SIZE_R + m_enemySize.x &&
			radarPos.y > -RADAR_SIZE_B + m_enemySize.y && radarPos.y < -RADAR_SIZE_T + m_enemySize.y)
		{
			DrawSetting();// �`��O�ݒ�
			std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;// �摜�����n��
			textures.push_back(m_pTextures[(int)(RadarState::Enemy)].Get());// �G�摜
			m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);	// �`�揀��
			VertexPositionTexture enemyVertex[4] =
			{
				//	 �@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���_���													UV���
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y,					 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y,					 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			m_pDrawPolygon->DrawTexture(enemyVertex);// �|���S����`��
		}
	}
	m_pDrawPolygon->ReleaseShader();//	�V�F�[�_�̓o�^���������Ă���
}
/*
*	@brief	�o�b�t�@���쐬����
*	@details ���[�_�[�N���X�̃o�b�t�@���쐬����
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	m_constBuffer.time = Vector4(m_time);// ���Ԑݒ�
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_���Z�b�g����
}
/*
*	@brief	�`��O�ݒ�
*	@details ���[�_�[�N���X�̕`��O�ݒ�
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::DrawSetting()
{
	m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �f�v�X�X�e���V���X�e�[�g
}
