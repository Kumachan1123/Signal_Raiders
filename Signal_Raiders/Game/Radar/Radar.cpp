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
// ���[�_�[�̍��[
const float Radar::RADAR_SIZE_L = 0.5f;
// ���[�_�[�̉E�[
const float Radar::RADAR_SIZE_R = 1.0f;
// ���[�_�[�̏�[
const float Radar::RADAR_SIZE_T = 0.16f;
// ���[�_�[�̉��[
const float Radar::RADAR_SIZE_B = 1.0f;
// �G�̓_�̕�
const float Radar::ENEMY_SIZE_W = 0.01f;
// �G�̓_�̍���
const float Radar::ENEMY_SIZE_H = 0.02f;
// ���[�_�[�̈ʒuX
const float Radar::RADAR_POSITION_X = 0.745f;
// ���[�_�[�̈ʒuY
const float Radar::RADAR_POSITION_Y = -0.55f;
// �v���C���[��X�T�C�Y
const float Radar::PLAYER_SIZE_X = 0.018f;
// �v���C���[��Y�T�C�Y
const float Radar::PLAYER_SIZE_Y = -0.032f;
// �G��X�T�C�Y
const float Radar::ENEMY_SIZE_X = 0.027f;
// �G��Y�T�C�Y
const float Radar::ENEMY_SIZE_Y = -0.048f;
// ���[�_�[�͈̔�
const float Radar::RADAR_RANGE = 300.0f;
// �v���C���[�Ƃ̋���
const float Radar::DISTANCE = 125.0f;
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
*	@brief	������
*	@details ���[�_�[�N���X�̏�����
*	@param	pPlayer	�v���C���[
*	@param	pEnemyManager	�G�}�l�[�W���[
*	@return �Ȃ�
*/
void Radar::Initialize(Player* pPlayer, EnemyManager* pEnemyManager)
{
	// �v���C���[�̃|�C���^�[��n��
	m_pPlayer = pPlayer;
	// �G�}�l�[�W���[�̃|�C���^�[��n��
	m_pEnemyManager = pEnemyManager;
	// �|���S���`�揀��
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// �f�o�C�X���擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(device, &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// ���_�V�F�[�_
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Radar/VS_Radar.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Radar/PS_Radar.cso", m_pPixelShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[��n��
	m_shaders.ps = m_pPixelShader.Get();
	// �W�I���g���V�F�[�_�[�͎g��Ȃ��̂�nullptr��ݒ�
	m_shaders.gs = nullptr;
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
	// ����
	m_time += elapsedTime;
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayer->GetPlayerPos();
	// �G�̈ʒu���N���A
	m_enemyPos.clear();
	// �o�^�����G�̕��������[�v
	for (auto& enemy : m_pEnemyManager->GetEnemies())
	{
		// �G�̈ʒu���擾
		Vector3 enemyPos = enemy->GetPosition();
		// �v���C���[�Ƃ̋������v�Z
		float distance = Vector3::Distance(m_playerPos, enemyPos);
		// ���[�_�[�͈͓��ɂ���G�����ۑ�
		if (distance <= m_range)
		{
			// �G�̈ʒu��ۑ�
			m_enemyPos.push_back(enemyPos);
		}
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
	// �w�i�`��
	DrawBackground();
	// �v���C���[�s���`��
	DrawPlayer();
	// �G�s���`��
	DrawEnemy();
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
	// ���[�_�[�̃T�C�Y��ݒ�
	VertexPositionTexture vertex[4] =
	{
		//										���_���								UV���
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),// ����
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_T, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),// �E��
		VertexPositionTexture(Vector3(RADAR_SIZE_R,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),// �E��
		VertexPositionTexture(Vector3(RADAR_SIZE_L,-RADAR_SIZE_B, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),// ����

	};
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(���Â߂̗�)
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer();
	// �`��O�ݒ�
	DrawSetting();
	// �摜�����n��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	// �w�i�摜���擾
	textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("RadarBack"));
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

//---------------------------------------------------------
// �v���C���[�`��
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �v���C���[�s���̕`��ʒu��ݒ�
	VertexPositionTexture playerVertex[4] =
	{
		//														���_���													UV���
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y					, 0.0f)	, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x	, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x					, m_radarPos.y + m_playerSize.y	, 0.0f)	, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(�Â߂̗΁j 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer();
	// �`��O�ݒ�
	DrawSetting();
	// �摜�����n��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	// �v���C���[�s���摜���擾
	textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("PlayerPin"));
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
	// �|���S����`��
	m_pDrawPolygon->DrawTexture(playerVertex);
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
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
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ�(��)
	m_constBuffer.colors = Vector4(0.9f, 0.0f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer();
	// �G�̈ʒu��`��	 
	for (const auto& enemyPos : m_enemyPos)
	{
		// �v���C���[����G�ւ̑��Έʒu���v�Z
		Vector3 relativePos = enemyPos - m_playerPos;
		// �J�����̌������擾
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir();
		// �J�����̌����Ɋ�Â��ĉ�]�s����쐬
		// �J�����̑O���x�N�g�������]�p���v�Z
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z);
		// ��]�𒲐�
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));
		// Y����]��ǉ�
		rotationMatrix *= Matrix::CreateRotationY(playerRotation);
		// �G�̈ʒu���v���C���[�̈ʒu�����ɂ��ĉ�]
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);
		// ���[�_�[�̒��S����̑��Έʒu���v�Z
		Vector2 radarPos = Vector2(
			rotatedPos.x / m_range * (RADAR_SIZE_L - RADAR_SIZE_R), // �~�j�}�b�v�̉���
			rotatedPos.z / m_range * (RADAR_SIZE_B - RADAR_SIZE_T) // �~�j�}�b�v�̍���
		);
		// �~�j�}�b�v�̒��S�ʒu�����Z
		radarPos += m_radarPos;
		// �~�j�}�b�v�͈͓̔��ɂ��邩���m�F���A�͈͓��Ȃ�`��
		if (radarPos.x > RADAR_SIZE_L + m_enemySize.x && radarPos.x < RADAR_SIZE_R + m_enemySize.x &&
			radarPos.y > -RADAR_SIZE_B + m_enemySize.y && radarPos.y < -RADAR_SIZE_T + m_enemySize.y)
		{
			// �`��O�ݒ�
			DrawSetting();
			// �摜�����n��
			std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
			// �G�s���摜���擾
			textures.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyPin"));
			// �`�揀��
			m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), textures);
			// �G�s���̕`��ʒu��ݒ�
			VertexPositionTexture enemyVertex[4] =
			{
				//	 �@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���_���													UV���
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y,					 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y,					 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + ENEMY_SIZE_W,	radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x,					radarPos.y + ENEMY_SIZE_H,	 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			// �|���S����`��
			m_pDrawPolygon->DrawTexture(enemyVertex);
		}
	}
	//	�V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
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
	// ���Ԑݒ�
	m_constBuffer.time = Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}
/*
*	@brief	�`��O�ݒ�
*	@details ���[�_�[�N���X�̕`��O�ݒ�
*	@param	�Ȃ�
*	@return �Ȃ�
*/
void Radar::DrawSetting()
{
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE);// �f�v�X�X�e���V���X�e�[�g
}
