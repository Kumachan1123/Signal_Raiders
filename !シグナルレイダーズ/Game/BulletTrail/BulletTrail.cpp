/*
	@file	BulletTrail.cpp
	@brief	�e�̋O�ՃN���X
	�쐬�ҁF���܂�
*/
#include "pch.h"
#include "Game/BulletTrail/BulletTrail.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include  "Game/KumachiLib/BinaryFile.h"
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

const std::vector<D3D11_INPUT_ELEMENT_DESC> BulletTrail::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

BulletTrail::BulletTrail(ParticleUtility::Type type)
	:m_commonResources{}
	, m_timer(0.0f)
	, m_pDR{}
	, m_CBuffer{}
	, m_inputLayout{}
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
{
}

BulletTrail::~BulletTrail()
{
}

void BulletTrail::Initialize(CommonResources* resources)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();
	//	�V�F�[�_�[�̍쐬
	CreateShader();
	// �摜�̓ǂݍ���
	LoadTexture(L"Resources/Textures/Trail2.png");

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);
}

void BulletTrail::CreateShader()
{
	ID3D11Device* device = m_pDR->GetD3DDevice();
	// �R���p�C�����ꂽ�V�F�[�_�[�̓ǂݍ���
	kumachi::BinaryFile VS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailVS.cso");
	kumachi::BinaryFile PS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailPS.cso");
	kumachi::BinaryFile GS = kumachi::BinaryFile::LoadFile(L"Resources/Shaders/Trail/TrailGS.cso");

	// �C���v�b�g���C�A�E�g�쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
							  static_cast<UINT>(INPUT_LAYOUT.size()),
							  VS.GetData(), VS.GetSize(),
							  m_inputLayout.GetAddressOf());
	// ���_�V�F�[�_�[�쐬
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), nullptr, m_vertexShader.GetAddressOf())))
	{
		throw std::exception("���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���");
	}
	// �s�N�Z���V�F�[�_�[�쐬
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), nullptr, m_pixelShader.GetAddressOf())))
	{
		throw std::exception("�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂���");
	}
	// �W�I���g���V�F�[�_�[�쐬
	if (FAILED(device->CreateGeometryShader(GS.GetData(), GS.GetSize(), nullptr, m_geometryShader.GetAddressOf())))
	{
		throw std::exception("�W�I���g���V�F�[�_�[�̍쐬�Ɏ��s���܂���");
	}
	// �萔�o�b�t�@�쐬
	D3D11_BUFFER_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ConstBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	device->CreateBuffer(&desc, nullptr, &m_CBuffer);
}



// �摜�̓ǂݍ���
void BulletTrail::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());
	m_texture.push_back(texture);
}

void BulletTrail::Update(float elapsedTime)
{
	m_timer += elapsedTime;
	//	�O�Ղ̍X�V
	Trail();
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

void BulletTrail::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_pDR->GetD3DDeviceContext();
	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
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
		VertexPositionColorTexture vPCT;
		//	�\������p�[�e�B�N���̒��S���W�݂̂�����B
		//	���܂�AC++�ŗp�ӂ��Ă���f�[�^�����ł̓e�N�X�`����\���ł��Ȃ��B
		//	�@���W�I���g���V�F�[�_���g�����O��̃f�[�^�A�Ƃ�������
		vPCT.position = XMFLOAT3(li.GetPosition());
		//	�e�N�X�`���̐F
		vPCT.color = XMFLOAT4(li.GetNowColor());


		//	���݂̃e�N�X�`���̃X�P�[�����uXMFLOAT2�v��X�ɓ����B
		//	Y�͎g���Ă��Ȃ����A����������Texture��UV���W�Ƃ͈Ⴄ�g�����ɂȂ��Ă��邱�Ƃɒ���
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//	�\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())
	{
		return;
	}
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//	�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//	�������`��w��		��ԃA���t�@����
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	//	�������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	//	�[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);

	//	�J�����O�͂Ȃ�
	context->RSSetState(m_states->CullCounterClockwise());

	//	�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�w�肵�����W�𒆐S�ɁA�V�F�[�_���Ŕ|���S���𐶐��E�`�悳����
	m_batch->Begin();

	//	�����܂ł̏����̊֌W��A�ȉ��͎g���Ȃ�
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);

	//	�W�I���g���V�F�[�_��Point���󂯎�邱�ƂɂȂ��Ă��邽�߁A
	//	�����ł�D3D11_PRIMITIVE_TOPOLOGY_POINTLIST���g��
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());

	m_batch->End();

	//	�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/// <summary>
/// �r���{�[�h�쐬�֐�
/// </summary>
/// <param name="target">�J�����^�[�Q�b�g�i�����_�j</param>
/// <param name="eye">�J�����A�C�i�J�������W�j</param>
/// <param name="up">������x�N�g���i��{��Y�݂̂P�̃x�N�g���j</param>
void BulletTrail::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard =
		SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}
void BulletTrail::Trail()
{
	// �^�C�}�[����莞�ԁi0.05�b�j�𒴂�����V�����p�[�e�B�N���𐶐�
	if (m_timer >= 0.00025f)
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
		SimpleMath::Vector3 randomVelocity = speed * SimpleMath::Vector3(
			cosf(randAngleXY) * sinf(randAngleXZ),
			cosf(randAngleXZ),
			sinf(randAngleXY) * sinf(randAngleXZ)
		);



		if (m_type == ParticleUtility::Type::PLAYERTRAIL)
		{
			// �p�[�e�B�N���̐���
			ParticleUtility pU(
				1.0f,  // ��������(s)
				m_bulletPos, // �����ʒu (����W)
				randomVelocity, // �������x�i�����_���ȕ����j
				SimpleMath::Vector3::One, // �����x
				SimpleMath::Vector3::One, // ��]���x
				SimpleMath::Vector3(0, 0, 10), // ������]
				SimpleMath::Vector3(1, 1, 0), // �����X�P�[��
				SimpleMath::Vector3(0, 0, 0), // �ŏI�X�P�[���i�������Ȃ�j
				SimpleMath::Vector4(0, 0.2, 1, .5), // �����J���[�i���j
				SimpleMath::Vector4(0.5, 1, 1, 0), // �ŏI�J���[�i���������j
				m_type // �p�[�e�B�N���̃^�C�v

			);
			// ���������p�[�e�B�N�������X�g�ɒǉ�
			m_particleUtility.push_back(pU);
		}

		if (m_type == ParticleUtility::Type::ENEMYTRAIL)
		{
			// �p�[�e�B�N���̐���
			ParticleUtility pU(
				1.0f,  // ��������(s)
				m_bulletPos, // �����ʒu (����W)
				randomVelocity, // �������x�i�����_���ȕ����j
				SimpleMath::Vector3::One, // �����x
				SimpleMath::Vector3::One, // ��]���x
				SimpleMath::Vector3(0, 0, 10), // ������]
				SimpleMath::Vector3(1, 1, 0), // �����X�P�[��
				SimpleMath::Vector3(0, 0, 0), // �ŏI�X�P�[���i�������Ȃ�j
				SimpleMath::Vector4(1, 0, 1, 1), // �����J���[�i���j
				SimpleMath::Vector4(1, 1, 1, 0), // �ŏI�J���[�i���������j
				m_type // �p�[�e�B�N���̃^�C�v

			);
			// ���������p�[�e�B�N�������X�g�ɒǉ�
			m_particleUtility.push_back(pU);
		}

		// �^�C�}�[�����Z�b�g
		m_timer = 0.0f;
	}
}

