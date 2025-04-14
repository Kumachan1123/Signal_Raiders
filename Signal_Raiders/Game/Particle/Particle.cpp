/*
	@file	Particle.cpp
	@brief	�e�̋O�ՃN���X
*/
#include "pch.h"
#include "Particle.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include  "Game/KumachiLib/BinaryFile/BinaryFile.h"

#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <algorithm>
#include <random>
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

Particle::Particle(ParticleUtility::Type type, float size)
	:m_commonResources{}
	, m_timer(0.0f)
	, m_elapsedTime(0.0f)
	, m_pDR{}
	, m_CBuffer{}
	, m_pInputLayout{}
	, m_batch{}
	, m_states{}
	, m_texture{}
	, m_vertexShader{}
	, m_pixelShader{}
	, m_geometryShader{}
	, m_world{}
	, m_view{}
	, m_proj{}
	, m_billboard{}
	, m_type{ type }
	, m_size{ size * 10 }
	, m_anim{ 0 }
	, m_animTime{ 0.0f }
	, m_animSpeed{ 30.0f }
	, m_frameRows{ 1 }
	, m_frameCols{ 1 }
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }
	, m_pCreateShader{ CreateShader::GetInstance() }
{
}

Particle::~Particle()
{
}

void Particle::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ���
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		m_animSpeed = 1;
		m_frameCols = 1;
		m_frameRows = 1;
		LoadTexture(L"Resources/Textures/Trail.png");
		break;
	case ParticleUtility::Type::BARRIERBREAK:
		m_animSpeed = 17.0f;
		m_frameCols = 5;
		m_frameRows = 4;
		LoadTexture(L"Resources/Textures/break.png");
		break;
	default:
		break;
	}
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}

void Particle::CreateShader()
{
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_vertexShader);
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Particle.cso", m_pixelShader);
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_geometryShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �萔�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_CBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_vertexShader.Get();
	m_shaders.ps = m_pixelShader.Get();
	m_shaders.gs = m_geometryShader.Get();
}



// �摜�̓ǂݍ���
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());
	m_texture.push_back(texture);
}

void Particle::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;
	m_timer += elapsedTime;
	m_animTime += elapsedTime * m_animSpeed;//	�A�j���[�V�����̍X�V
	//	�^�C�}�[����莞�Ԃ𒴂����烊�Z�b�g
	if (m_animTime >= 2.0f)
	{
		m_anim++;
		m_animTime = 0.0f;
	}
	//	�O�Ղ̍X�V
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:
	case ParticleUtility::Type::PLAYERTRAIL:
		Trail();
		break;
	case ParticleUtility::Type::BARRIERBREAK:

		if (m_timer <= 0.50f)BarrierBreak();

		break;
	default:
		break;
	}

	//	timer��n����m_effect�̍X�V�������s��
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		//	�q�N���X����false�ŏ���
		if (!(ite)->Update(elapsedTime))
		{
			ite = m_particleUtility.erase(ite);
			if (ite == m_particleUtility.end()) break;
		}
	}
}

void Particle::Render(Matrix view, Matrix proj)
{
	if (m_timer >= 1.9f)return;
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//	�J�������ʂ̋����Ń\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	m_vertices.clear();
	for (ParticleUtility& li : m_particleUtility)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//	���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
			continue;
		}
		VertexPositionColorTexture vPCT{};

		//	���W
		vPCT.position = XMFLOAT3(li.GetPosition());
		//	�e�N�X�`���̐F
		vPCT.color = XMFLOAT4(li.GetNowColor());

		//	���݂̃e�N�X�`���̃X�P�[�����uXMFLOAT2�v��X�ɓ����B
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//	�\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())
	{
		return;
	}
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	m_constantBuffer.matView = view.Transpose();
	m_constantBuffer.matProj = proj.Transpose();
	m_constantBuffer.matWorld = m_billboard.Transpose();
	m_constantBuffer.colors = Vector4(1, 1, 1, 0);
	m_constantBuffer.count = Vector4((float)(m_anim));
	m_constantBuffer.height = Vector4((float)(m_frameRows));
	m_constantBuffer.width = Vector4((float)(m_frameCols));

	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_CBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);

	if (m_type == ParticleUtility::Type::BARRIERBREAK)// �o���A����ꂽ�Ƃ��̔j�Ђ͐[�x�o�b�t�@���g��Ȃ�
	{
		// �`��O�ݒ�
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,
			DrawPolygon::BlendStates::NONPREMULTIPLIED,
			DrawPolygon::RasterizerStates::CULL_NONE,
			DrawPolygon::DepthStencilStates::DEPTH_READ);
	}
	else// ����ȊO�̃p�[�e�B�N���͐[�x�o�b�t�@���g��
	{
		// �`��O�ݒ�
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::LINEAR_WRAP,
			DrawPolygon::BlendStates::NONPREMULTIPLIED,
			DrawPolygon::RasterizerStates::CULL_NONE,
			DrawPolygon::DepthStencilStates::DEPTH_READ);
	}

	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_texture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �w�肵�����W�𒆐S�ɁA�V�F�[�_���Ŕ|���S���𐶐��E�`�悳����
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}

// �r���{�[�h�̍쐬
void Particle::CreateBillboard(Vector3 target, Vector3 eye, Vector3 up)
{
	m_billboard =
		Matrix::CreateBillboard(Vector3::Zero, eye - target, up);

	Matrix rot = Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}

// �p�[�e�B�N���̐���
void Particle::Trail()
{
	// �^�C�}�[����莞�Ԃ𒴂�����V�����p�[�e�B�N���𐶐�
	if (m_timer >= m_elapsedTime)
	{
		// �����̐ݒ�
		std::random_device seed;
		std::default_random_engine engine(seed());

		// �����_���Ȋp�x�i0 ���� 2�� �܂Łj
		std::uniform_real_distribution<> angleDist(0, XM_2PI);

		// �����_���ȑ��x�͈̔͂�ݒ�
		std::uniform_real_distribution<> speedDist(0.5f, 2.0f);

		// �����_���Ȋp�x
		float randAngleXY = static_cast<float>(angleDist(engine));
		float randAngleXZ = static_cast<float>(angleDist(engine));

		// �����_���ȑ��x
		float speed = static_cast<float>(speedDist(engine));

		// �����_���ȕ����̑��x�x�N�g��
		Vector3 randomVelocity = speed * Vector3(
			cosf(randAngleXY) * sinf(randAngleXZ),
			cosf(randAngleXZ),
			sinf(randAngleXY) * sinf(randAngleXZ)
		);



		if (m_type == ParticleUtility::Type::PLAYERTRAIL)// �v���C���[�̒e�̋O��
		{
			// �p�[�e�B�N���̐���
			ParticleUtility pU(
				1.0f,  // ��������(s)
				m_bulletPosition, // �����ʒu (����W)
				randomVelocity, // �������x�i�����_���ȕ����j
				Vector3::One, // �����x
				Vector3::One, // ��]���x
				Vector3(0, 0, 10), // ������]
				Vector3(m_size, m_size, 0), // �����X�P�[��
				Vector3(m_size / 10, m_size / 10, 0), // �ŏI�X�P�[���i�������Ȃ�j
				Vector4(0, 0.2, 1, .5), // �����J���[�i���j
				Vector4(0, 1, 1, 0.5), // �ŏI�J���[�i���������j
				m_type // �p�[�e�B�N���̃^�C�v

			);
			// ���������p�[�e�B�N�������X�g�ɒǉ�
			m_particleUtility.push_back(pU);
		}

		if (m_type == ParticleUtility::Type::ENEMYTRAIL)// �G�̒e�̋O��
		{
			// �p�[�e�B�N���̐���
			ParticleUtility pU(
				1.0f,  // ��������(s)
				m_bulletPosition, // �����ʒu (����W)
				randomVelocity, // �������x�i�����_���ȕ����j
				Vector3::One, // �����x
				Vector3::One, // ��]���x
				Vector3(0, 0, 10), // ������]
				Vector3(1, 1, 0), // �����X�P�[��
				Vector3(0, 0, 0), // �ŏI�X�P�[���i�������Ȃ�j
				Vector4(1, 0, 1, 1), // �����J���[�i���j
				Vector4(0, 1, 1, 0.25), // �ŏI�J���[�i���������j
				m_type // �p�[�e�B�N���̃^�C�v

			);
			// ���������p�[�e�B�N�������X�g�ɒǉ�
			m_particleUtility.push_back(pU);
		}

		// �^�C�}�[�����Z�b�g
		m_timer = 0.0f;
	}
}

void Particle::BarrierBreak()
{
	if (m_timer >= m_elapsedTime)  // �o���A����ꂽ�Ƃ��̔j�Ђ͏����Ԋu�����߂ł��悢
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<> angleDist(0, XM_2PI); // �S���������_���Ȋp�x
		std::uniform_real_distribution<> speedDist(2.5f, 3.0f); // ���q�̑��x�͈�
		std::uniform_real_distribution<> sizeDist(1.5f, 5.0f);  // �j�Ђ̃T�C�Y�������_���ɐݒ�
		std::uniform_real_distribution<> heightDist(-1.0f, 1.0f); // ���������i-1�`1�j�̃����_���͈�

		for (int i = 0; i < 20; ++i)  // �j�Ђ𕡐�����
		{
			// �����_���ȕ����Ŕ�΂����߂̋��ʍ��W
			float theta = static_cast<float>(angleDist(engine)); // ���������̊p�x (0�`2��)
			float phi = acosf(static_cast<float>(heightDist(engine))); // ���������̊p�x (0�`��)

			// �����_���ȑ��x
			float speed = static_cast<float>(speedDist(engine)) * 5.0f;

			// ���ʍ��W�n���f�J���g���W�n�ɕϊ����đ��x�x�N�g�����v�Z
			Vector3 randomVelocity = speed * Vector3(
				sinf(phi) * cosf(theta), // X����
				cosf(phi),              // Y����
				sinf(phi) * sinf(theta) // Z����
			);

			// �����_���ȃT�C�Y
			float randomSize = static_cast<float>(sizeDist(engine));

			// �p�[�e�B�N���̐���
			ParticleUtility pU(
				1.5f,  // �j�Ђ̐������ԁi�Z�����ď�����悤�Ɂj
				m_bossPosition, // �����ʒu (����W)
				randomVelocity * 10, // �������x�i�����_���ȕ����j
				Vector3::Zero, // �d�͉����x�iY�������ɏ�������������j
				Vector3::Zero, // ��]���x
				Vector3::Zero, // ������]
				Vector3(randomSize, randomSize, randomSize), // �����_���ȃT�C�Y
				Vector3(0.1f, 0.1f, 0.1f), // �ŏI�X�P�[���i�������Ȃ�j
				Vector4(1.0f, 1.0f, 0.5f, 1.0f),  // �����J���[�i�I�����W���ۂ��j
				Vector4(1.0f, 1.0f, 1.0f, 0.0f), // �ŏI�J���[�i���������j
				ParticleUtility::Type::BARRIERBREAK // �^�C�v��ǉ�
			);
			m_particleUtility.push_back(pU);
		}
	}
}
