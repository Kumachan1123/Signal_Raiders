/*
	@file	Radar.cpp
	@brief	���[�_�[�N���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Radar.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
Radar::Radar(CommonResources* commonResources)
	: m_commonResources{ commonResources },
	m_pPlayer{ nullptr },
	m_pEnemies{ nullptr },
	m_radarPos{ 0.745f, -0.55f },
	m_playerSize{ 0.018f, -0.032f },
	m_enemySize{ 0.009f, -0.016f }

{
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
Radar::~Radar()
{
}

//---------------------------------------------------------
// ������
//---------------------------------------------------------
void Radar::Initialize(Player* pPlayer, Enemies* pEnemies)
{
	m_pPlayer = pPlayer;
	m_pEnemies = pEnemies;
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_commonResources->GetDeviceResources()->GetD3DDeviceContext());
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_commonResources->GetDeviceResources()->GetD3DDevice());
	// �G�t�F�N�g�̐ݒ�
	m_basicEffect->SetVertexColorEnabled(true);
	// ���̓��C�A�E�g�̍쐬
	// InputLayout�̍쐬
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_commonResources->GetDeviceResources()->GetD3DDevice()->CreateInputLayout(
		VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.GetAddressOf()
	);

}

//---------------------------------------------------------
// �X�V
//---------------------------------------------------------
void Radar::Update()
{
	// �v���C���[�̈ʒu���擾
	m_playerPos = m_pPlayer->GetPlayerPos();
	// �G�̈ʒu���N���A
	m_enemyPos.clear();
	// �G�̈ʒu���擾
	for (auto& enemy : m_pEnemies->GetEnemy())
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
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �o�b�`�̊J�n
	m_primitiveBatch->Begin();
	m_basicEffect->Apply(context);
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());

	context->IASetInputLayout(m_inputLayout.Get());

	// ���[�_�[�̔w�i��`��
	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(0.5f, -.16f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(1.0f, -.16f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(1.0f, -1.0f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6)),
		VertexPositionColor(Vector3(0.5f, -1.0f, 0.0f), Vector4(0.0, 0.0, 0.0, 0.6))
	);
	// �o�b�`�̏I��
	m_primitiveBatch->End();

	// �o�b�`�̊J�n
	m_primitiveBatch->Begin();


	// �v���C���[�ʒu�����[�_�[���S�ɕ`��
	// ���[�_�[�̔w�i��`�悵�����̒l���璆�S���v�Z���A�v���C���[�̈ʒu��`��
	m_basicEffect->Apply(context);
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthDefault(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());

	m_primitiveBatch->DrawQuad(
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x + m_playerSize.x, m_radarPos.y + m_playerSize.y, 0.0f), Colors::Lime),
		VertexPositionColor(Vector3(m_radarPos.x, m_radarPos.y + m_playerSize.y, 0.0f), Colors::Lime)
	);
	// �o�b�`�̏I��
	m_primitiveBatch->End();




	// �G�̈ʒu��`��	 
	for (const auto& enemyPos : m_enemyPos)
	{
		Vector3 relativePos = enemyPos - m_playerPos;// �v���C���[����G�ւ̑��Έʒu���v�Z


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
			m_basicEffect->Apply(context);
			context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
			context->OMSetDepthStencilState(states->DepthDefault(), 0);
			context->RSSetState(states->CullNone());
			context->IASetInputLayout(m_inputLayout.Get());
			// �o�b�`�̊J�n
			m_primitiveBatch->Begin();
			// �G�̈ʒu��`��
			m_primitiveBatch->DrawQuad(
				VertexPositionColor(Vector3(radarPos.x, radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x + 0.01f, radarPos.y, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x + 0.01f, radarPos.y + 0.02f, 0.0f), Colors::Red),
				VertexPositionColor(Vector3(radarPos.x, radarPos.y + 0.02f, 0.0f), Colors::Red)
			);
			// �o�b�`�̏I��
			m_primitiveBatch->End();
		}


	}



}