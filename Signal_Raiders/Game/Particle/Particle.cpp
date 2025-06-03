/*
*	@file	Particle.cpp
*	@brief	�p�[�e�B�N���N���X
*/
#include <pch.h>
#include "Particle.h"

/*
*	@brief	�C���v�b�g���C�A�E�g
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@detail �N���X�̐����������s��
*	@param type �p�[�e�B�N���̃^�C�v
*	@param size �p�[�e�B�N���̃T�C�Y
*	@return �Ȃ�
*/
Particle::Particle(ParticleUtility::Type type, float size)
	:m_pCommonResources{}// ���ʃ��\�[�X
	, m_timer(0.0f)// �o�ߎ���
	, m_elapsedTime(0.0f)// �t���[������
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �R���X�^���g�o�b�t�@
	, m_pInputLayout{}// ���̓��C�A�E�g
	, m_pTexture{}// �e�N�X�`��
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_pGeometryShader{}// �W�I���g���V�F�[�_�[
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_billboard{}// �r���{�[�h�s��
	, m_type{ type }// �p�[�e�B�N���̃^�C�v
	, m_size{ size * 10 }// �p�[�e�B�N���̃T�C�Y(�n�����l�̏\�{�j
	, m_barrierBreakSize{ 0 }// �o���A�̔j�Ђ̃T�C�Y
	, m_anim{ 0 }// �t���[����
	, m_animTime{ 0.0f }// �A�j���[�V��������
	, m_animSpeed{ 30.0f }// �A�j���[�V�����̑��x
	, m_frameRows{ 1 }// �t���[���̍s��
	, m_frameCols{ 1 }// �t���[���̗�
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief	�f�X�g���N�^
*	@detail �N���X�̔j���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Particle::~Particle() {/*do nothing.*/ }
/*
*	@brief	������
*	@detail �N���X�̏������������s��
*	@param CommonResources* resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Particle::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �摜�̓ǂݍ���
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		//	�A�j���[�V�����̑��x
		m_animSpeed = 1;
		//	�t���[���̗�
		m_frameCols = 1;
		//	�t���[���̍s��
		m_frameRows = 1;
		// �e�N�X�`���̎擾
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Trail"));
		break;
		// �o���A�j��
	case ParticleUtility::Type::BARRIERBREAK:
		m_animSpeed = 17.0f;// �A�j���[�V�����̑��x
		// 	�t���[���̗�
		m_frameCols = 5;
		// 	�t���[���̍s��
		m_frameRows = 4;
		// �e�N�X�`���̎擾
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Break"));
		break;
	default:// ����ȊO�̃p�[�e�B�N��
		break;
	}
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@detail �V�F�[�_�[�̍쐬�������s��
*	@param �Ȃ�
*/
void Particle::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_pVertexShader);
	// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Particle.cso", m_pPixelShader);
	// �W�I���g���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_pGeometryShader);
	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �萔�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g����
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g����
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[���Z�b�g����
	m_shaders.gs = m_pGeometryShader.Get();
}
/*
*	@brief �X�V
*	@detail �N���X�̍X�V�������s��
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Particle::Update(float elapsedTime)
{
	// �t���[�����Ԃ��Z�b�g
	m_elapsedTime = elapsedTime;
	// �^�C�}�[���X�V
	m_timer += elapsedTime;
	// �A�j���[�V�����̍X�V
	m_animTime += elapsedTime * m_animSpeed;
	// �^�C�}�[����莞�Ԃ𒴂����烊�Z�b�g
	if (m_animTime >= 2.0f)
	{
		// �t���[�������X�V
		m_anim++;
		// �^�C�}�[�����Z�b�g
		m_animTime = 0.0f;
	}
	// �p�[�e�B�N���̃^�C�v�ɂ���ď����𕪂���
	switch (m_type)
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		// �O�Ղ̍X�V
		Trail();
		break;
	case ParticleUtility::Type::BARRIERBREAK:// �o���A�j��
		// �o���A�j��̍X�V
		if (m_timer <= 0.50f)BarrierBreak();
		break;
	default:// ����ȊO�̃p�[�e�B�N��
		break;
	}
	//	timer��n���čX�V�������s��
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// �p�[�e�B�N���̍X�V
		if (!(ite->Update(elapsedTime)))
		{
			// �X�V���s�i�����؂�Ȃǁj�Ȃ�폜���Ď���
			ite = m_particleUtility.erase(ite);
		}
		else
		{
			// �X�V�����Ȃ玟�̗v�f��
			++ite;
		}
	}
}
/*
*	@brief �`��
*	@detail �N���X�̕`�揈�����s��
*	@param const DirectX::SimpleMath::Matrix& view �r���[�s��
*	@param const DirectX::SimpleMath::Matrix& proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Particle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �����Ԃ��߂�����`�悵�Ȃ�
	if (m_timer >= 1.9f)return;
	// �J�����̕������擾
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	// �J�����̕����𐳋K��
	cameraDir.Normalize();
	// �J�����̑O���ɋ߂����Ƀ\�[�g
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			// ���ςŃ\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	// ���_���N���A
	m_vertices.clear();
	// ���X�g�̃p�[�e�B�N����S�ĕ`�悷��
	for (ParticleUtility& li : m_particleUtility)
	{
		// ���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) continue;
		// ���_�̍\���̂��쐬
		VertexPositionColorTexture vPCT{};
		// ���W
		vPCT.position = XMFLOAT3(li.GetPosition());
		// �e�N�X�`���̐F
		vPCT.color = XMFLOAT4(li.GetNowColor());
		// ���݂̃e�N�X�`���̃X�P�[����XMFLOAT2��X�ɓ����
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);
		// ���_��ǉ�
		m_vertices.push_back(vPCT);
	}
	// �\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())return;
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �r���[�s���]�u���ăZ�b�g����
	m_constantBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����s���]�u���ăZ�b�g����
	m_constantBuffer.matProj = proj.Transpose();
	// �r���{�[�h�s������[���h�s��Ƃ��ăZ�b�g����
	m_constantBuffer.matWorld = m_billboard.Transpose();
	// �F���Z�b�g����
	m_constantBuffer.colors = Vector4(1, 1, 1, 0);
	// �t���[�������Z�b�g����
	m_constantBuffer.count = Vector4((float)(m_anim));
	// �s�����Z�b�g����
	m_constantBuffer.height = Vector4((float)(m_frameRows));
	// �񐔂��Z�b�g����
	m_constantBuffer.width = Vector4((float)(m_frameCols));
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �o���A����ꂽ�Ƃ��̔j�Ђ͐[�x�o�b�t�@���g��Ȃ�
	if (m_type == ParticleUtility::Type::BARRIERBREAK)
	{

		// �`��O�ݒ�
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,//�T���v���[�X�e�[�g
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
			DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
			DrawPolygon::DepthStencilStates::DEPTH_READ);// �[�x�X�e���V���X�e�[�g
	}
	else	// ����ȊO�̃p�[�e�B�N���͐[�x�o�b�t�@���g��
	{

		// �`��O�ݒ�
		m_pDrawPolygon->DrawSetting(
			DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
			DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
			DrawPolygon::DepthStencilStates::DEPTH_READ); // �[�x�X�e���V���X�e�[�g
	}
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �w�肵�����W�𒆐S�ɁA�V�F�[�_���Ŕ|���S���𐶐��E�`�悳����
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief �r���{�[�h�s��̍쐬
*	@detail �r���{�[�h�s��̍쐬�������s��
*	@param target �����_
*	@param eye �J�����̈ʒu
*	@param up �J�����̏����
*	@return �Ȃ�
*/
void Particle::CreateBillboard(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& up)
{
	using namespace DirectX::SimpleMath;
	// �r���{�[�h�s��̍쐬
	m_billboard = Matrix::CreateBillboard(Vector3::Zero, eye - target, up);
	// �P�ʍs��
	Matrix rot = Matrix::Identity;
	// X�����]
	rot._11 = -1;
	// Z�����]
	rot._33 = -1;
	// �J�����̈ʒu
	m_cameraPosition = eye;
	// �J�����̒����_
	m_cameraTarget = target;
	// �r���{�[�h�s��̔��]
	m_billboard = rot * m_billboard;
}
/*
*	@brief �O�Ղ̍X�V
*	@detail �O�Ղ̍X�V�������s��
*	@param �Ȃ�
*/
void Particle::Trail()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �^�C�}�[����莞�Ԃ𒴂�����V�����p�[�e�B�N���𐶐�
	if (m_timer >= m_elapsedTime)
	{
		// �����̐ݒ�
		std::random_device seed;
		// �����Z���k�E�c�C�X�^�@
		std::default_random_engine engine(seed());
		// �����_���Ȋp�x�i0 ���� 2�� �܂Łj
		std::uniform_real_distribution<> angleDist(0, XM_2PI);
		// �����_���ȑ��x�͈̔͂�ݒ�
		std::uniform_real_distribution<> speedDist(0.5f, 2.0f);
		// XY���ʏ�̃����_���Ȋp�x
		float randAngleXY = static_cast<float>(angleDist(engine));
		// XZ���ʏ�̃����_���Ȋp�x
		float randAngleXZ = static_cast<float>(angleDist(engine));
		// �����_���ȑ��x
		float speed = static_cast<float>(speedDist(engine));
		// �����_���ȕ����̑��x�x�N�g��
		Vector3 randomVelocity = speed * Vector3(
			cosf(randAngleXY) * sinf(randAngleXZ),// X����
			cosf(randAngleXZ),					 // Y����
			sinf(randAngleXY) * sinf(randAngleXZ)// Z����
		);
		// �v���C���[�̒e�̋O��
		if (m_type == ParticleUtility::Type::PLAYERTRAIL)
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
		// �G�̒e�̋O��
		if (m_type == ParticleUtility::Type::ENEMYTRAIL)
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
/*
*	@brief �o���A�j��̍X�V
*	@detail �o���A�j��̍X�V�������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Particle::BarrierBreak()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �o���A����ꂽ�Ƃ��̔j�Ђ͏����Ԋu�����߂ł��悢
	if (m_timer >= m_elapsedTime)
	{
		// �����̐ݒ�
		std::random_device seed;
		// �����Z���k�E�c�C�X�^�@
		std::default_random_engine engine(seed());
		// �S���������_���Ȋp�x
		std::uniform_real_distribution<> angleDist(0, XM_2PI);
		// ���q�̑��x�͈�
		std::uniform_real_distribution<> speedDist(2.5f, 3.0f);
		// �j�Ђ̃T�C�Y�������_���ɐݒ�
		std::uniform_real_distribution<> sizeDist(1.5f, 5.0f);
		// ���������i-1�`1�j�̃����_���͈�
		std::uniform_real_distribution<> heightDist(-1.0f, 1.0f);
		// �j�Ђ𕡐�����
		for (int i = 0; i < 20; ++i)
		{
			// �����_���ȕ����Ŕ�΂����߂̋��ʍ��W
			// ���������̊p�x (0�`2��)
			float theta = static_cast<float>(angleDist(engine));
			// ���������̊p�x (0�`��)
			float phi = acosf(static_cast<float>(heightDist(engine)));
			// �����_���ȑ��x
			float speed = static_cast<float>(speedDist(engine)) * 5.0f;
			Vector3 randomVelocity = speed * Vector3(// ���ʍ��W�n���f�J���g���W�n�ɕϊ����đ��x�x�N�g�����v�Z
				sinf(phi) * cosf(theta), // X����
				cosf(phi),              // Y����
				sinf(phi) * sinf(theta) // Z����
			);
			// �����_���ȃT�C�Y
			float randomSize = static_cast<float>(sizeDist(engine)) * m_barrierBreakSize;
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
			// ���������p�[�e�B�N�������X�g�ɒǉ�
			m_particleUtility.push_back(pU);
		}
	}
}
