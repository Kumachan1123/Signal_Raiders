/*
	@file	Radar.cpp
	@brief	���[�_�[�N���X
*/
#include "pch.h"
#include "Radar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
const std::vector<D3D11_INPUT_ELEMENT_DESC>  Radar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },

};

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Radar::Radar(CommonResources* commonResources)
	: m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_radarPos{ 0.745f, -0.55f },
	m_playerSize{ 0.018f, -0.032f },
	m_enemySize{ 0.027f, -0.048f },
	m_time{ 0.0f }

{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Radar::~Radar()
{
	DrawPolygon::ReleasePositionTexture();
}
void Radar::LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(device, path, nullptr, texture.ReleaseAndGetAddressOf());
	textures.push_back(texture);
}

//---------------------------------------------------------
// ������
//---------------------------------------------------------
void Radar::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	m_pPlayer = pPlayer;
	m_pEnemies = pEnemies;
	// �|���S���`�揀��
	DrawPolygon::InitializePositionTexture(m_commonResources->GetDeviceResources());

	// �摜�̓ǂݍ��� 
	LoadTexture(L"Resources/Textures/RadarBack.png", m_backTextures);
	LoadTexture(L"Resources/Textures/PlayerPin.png", m_playerTextures);
	LoadTexture(L"Resources/Textures/EnemyPin.png", m_enemyTextures);
	//	�R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	kumachi::BinaryFile VSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Radar/VS_Radar.cso");
	kumachi::BinaryFile PSData = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Radar/PS_Radar.cso");
	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), VSData.GetData(), VSData.GetSize(), m_inputLayout.GetAddressOf());
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

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void Radar::Update(float elapsedTime)
{
	// ����
	m_time += elapsedTime;
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayer->GetPlayerPos();
	// �G�̈ʒu���N���A
	m_enemyPos.clear();
	// �G�̈ʒu���擾
	for (auto& enemy : m_pEnemies->GetEnemies())
	{
		Vector3 enemyPos = enemy->GetPosition();
		float distance = Vector3::Distance(m_playerPos, enemyPos);

		// ���[�_�[�͈͓��ɂ���G�����ۑ�
		if (distance <= m_range)
		{
			m_enemyPos.push_back(enemyPos);
		}
	}

}

//---------------------------------------------------------
// �`��
//---------------------------------------------------------
void Radar::Render()
{
	// ���[�_�[�`��
	DrawBackground();// �w�i�`��
	DrawPlayer();// �v���C���[�`��
	DrawEnemy();// �G�`��
}


//---------------------------------------------------------
// �w�i�`��
// ---------------------------------------------------------
void Radar::DrawBackground()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	���_���(�|���S���̂S���_�̍��W���j
	VertexPositionTexture vertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(0.5f, -.16f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f, -.16f, 0.0f),SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(1.0f, -1.0f, 0.0f),SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(0.5f, -1.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f)),

	};
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer(context);
	// �`�揀��
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_backTextures);
	// �|���S����`��
	DrawPolygon::DrawTexture(vertex);
	// �V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------
// �v���C���[�`��
//---------------------------------------------------------
void Radar::DrawPlayer()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	VertexPositionTexture playerVertex[4] =
	{
		//	���_���													UV���
		VertexPositionTexture(Vector3(m_radarPos.x, m_radarPos.y, 0.0f)										, SimpleMath::Vector2(0.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y, 0.0f)					, SimpleMath::Vector2(1.0f, 0.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y + m_playerSize.y, 0.0f)	, SimpleMath::Vector2(1.0f, 1.0f)),
		VertexPositionTexture(Vector3(m_radarPos.x, m_radarPos.y + m_playerSize.y, 0.0f)					, SimpleMath::Vector2(0.0f, 1.0f)),

	};
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.5f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer(context);
	// �`�揀��
	DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_playerTextures);
	// �|���S����`��
	DrawPolygon::DrawTexture(playerVertex);
	// �V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//---------------------------------------------------------
// �G�`��
//---------------------------------------------------------
void Radar::DrawEnemy()
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �O���f�[�V�����G�t�F�N�g�̐F�ݒ� 
	m_constBuffer.colors = SimpleMath::Vector4(0.9f, 0.0f, 0.0f, 0);
	// �o�b�t�@���쐬
	CreateBuffer(context);
	// �G�̈ʒu��`��	 
	for (const auto& enemyPos : m_enemyPos)
	{
		// �v���C���[����G�ւ̑��Έʒu���v�Z
		Vector3 relativePos = enemyPos - m_playerPos;
		// �J�����̌������擾
		Vector3 cameraDirection = m_pPlayer->GetPlayerDir(); // �J�����̌����i�O���x�N�g���j

		// �J�����̌����Ɋ�Â��ĉ�]�s����쐬
		float playerRotation = atan2(cameraDirection.x, cameraDirection.z); // �J�����̑O���x�N�g�������]�p���v�Z
		Matrix rotationMatrix = Matrix::CreateRotationZ(XMConvertToRadians(180));// ��]�𒲐�
		rotationMatrix *= Matrix::CreateRotationY(playerRotation); // Y����]

		// �G�̈ʒu���v���C���[�̈ʒu�����ɂ��ĉ�]
		Vector3 rotatedPos = Vector3::Transform(relativePos, rotationMatrix);
		// ���[�_�[�̒��S����̑��Έʒu���v�Z
		Vector2 radarPos = Vector2(
			rotatedPos.x / m_range * (1.0f - 0.5f), // �~�j�}�b�v�̉���
			rotatedPos.z / m_range * (1.0f - 0.16f) // �~�j�}�b�v�̍���
		);
		radarPos += m_radarPos;

		// �~�j�}�b�v�͈͓̔��ɂ��邩���m�F
		if (radarPos.x > 0.5f + m_enemySize.x && radarPos.x < 1.0f + m_enemySize.x &&
			radarPos.y > -1.0f + m_enemySize.y && radarPos.y < -0.16f + m_enemySize.y)
		{
			// �`�揀��
			DrawPolygon::DrawStartTexture(context, m_inputLayout.Get(), m_enemyTextures);
			VertexPositionTexture enemyVertex[4] =
			{
				//	���_���													UV���
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y, 0.0f), SimpleMath::Vector2(0.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y, 0.0f), SimpleMath::Vector2(1.0f, 0.0f)),
				VertexPositionTexture(Vector3(radarPos.x + 0.01f, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f)),
				VertexPositionTexture(Vector3(radarPos.x, radarPos.y + 0.02f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f))
			};
			// �|���S����`��
			DrawPolygon::DrawTexture(enemyVertex);
		}
	}
	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}


//---------------------------------------------------------
// �o�b�t�@���쐬
//---------------------------------------------------------
void Radar::CreateBuffer(ID3D11DeviceContext1* context)
{
	// ���Ԑݒ�
	m_constBuffer.time = SimpleMath::Vector4(m_time);
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	DrawPolygon::UpdateSubResources(context, m_cBuffer.Get(), &m_constBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_cBuffer.Get() };
	// ���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	// �V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}