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
	m_pCommonResources = resources;// ���ʃ��\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);// �V�F�[�_�[�쐬�N���X�̏�����
	CreateShaders();// �V�F�[�_�[�̍쐬
	switch (m_type)// �摜�̓ǂݍ���
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		m_animSpeed = 1;//	�A�j���[�V�����̑��x
		m_frameCols = 1;//	�t���[���̗�
		m_frameRows = 1;//	�t���[���̍s��
		LoadTexture(L"Resources/Textures/Trail.png");// �e�N�X�`���̓ǂݍ���
		break;
	case ParticleUtility::Type::BARRIERBREAK:// �o���A�j��
		m_animSpeed = 17.0f;// �A�j���[�V�����̑��x
		m_frameCols = 5;// 	�t���[���̗�
		m_frameRows = 4;// 	�t���[���̍s��
		LoadTexture(L"Resources/Textures/break.png");// �e�N�X�`���̓ǂݍ���
		break;
	default:// ����ȊO�̃p�[�e�B�N��
		break;
	}
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);// �|���S���`��p
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@detail �V�F�[�_�[�̍쐬�������s��
*	@param �Ȃ�
*/
void Particle::CreateShaders()
{
	// �V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_pVertexShader);// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Particle.cso", m_pPixelShader);// �s�N�Z���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_pGeometryShader);// �W�I���g���V�F�[�_�[�̍쐬
	m_pInputLayout = m_pCreateShader->GetInputLayout();// �C���v�b�g���C�A�E�g���󂯎��
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));// �萔�o�b�t�@�쐬
	// �V�F�[�_�[�̍\���̂ɃV�F�[�_�[��n��
	m_shaders.vs = m_pVertexShader.Get();// ���_�V�F�[�_�[
	m_shaders.ps = m_pPixelShader.Get();// �s�N�Z���V�F�[�_�[
	m_shaders.gs = m_pGeometryShader.Get();// �W�I���g���V�F�[�_�[
}

/*
*	@brief �e�N�X�`���̓ǂݍ���
*	@detail �e�N�X�`���̓ǂݍ��ݏ������s��
*	@param const wchar_t* path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void Particle::LoadTexture(const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �ꎞ�ۑ��p�e�N�X�`���n���h��
	DirectX::CreateWICTextureFromFile(m_pDR->GetD3DDevice(), path, nullptr, texture.GetAddressOf());// �e�N�X�`���̓ǂݍ���
	m_pTexture.push_back(texture);// �z��ɓo�^
}
/*
*	@brief �X�V
*	@detail �N���X�̍X�V�������s��
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Particle::Update(float elapsedTime)
{
	m_elapsedTime = elapsedTime;// �t���[�����Ԃ��Z�b�g
	m_timer += elapsedTime;//	�^�C�}�[���X�V
	m_animTime += elapsedTime * m_animSpeed;//	�A�j���[�V�����̍X�V
	if (m_animTime >= 2.0f)//	�^�C�}�[����莞�Ԃ𒴂����烊�Z�b�g
	{
		m_anim++;// �t���[�������X�V
		m_animTime = 0.0f;// �^�C�}�[�����Z�b�g
	}
	//	�O�Ղ̍X�V
	switch (m_type)// �p�[�e�B�N���̃^�C�v�ɂ���ď����𕪂���
	{
	case ParticleUtility::Type::ENEMYTRAIL:// �G�̒e�̋O��
	case ParticleUtility::Type::PLAYERTRAIL:// �v���C���[�̒e�̋O��
		Trail();// �O�Ղ̍X�V
		break;
	case ParticleUtility::Type::BARRIERBREAK:// �o���A�j��
		if (m_timer <= 0.50f)BarrierBreak();// �o���A�j��̍X�V
		break;
	default:// ����ȊO�̃p�[�e�B�N��
		break;
	}
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)//	timer��n���čX�V�������s��
	{
		if (!(ite)->Update(elapsedTime))// �p�[�e�B�N���̍X�V
		{
			ite = m_particleUtility.erase(ite);// �p�[�e�B�N���̍폜
			if (ite == m_particleUtility.end()) break;// ���[�v�𔲂���
		}
	}
}
/*
*	@brief �`��
*	@detail �N���X�̕`�揈�����s��
*	@param DirectX::SimpleMath::Matrix view �r���[�s��
*	@param DirectX::SimpleMath::Matrix proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Particle::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	if (m_timer >= 1.9f)return;// �����Ԃ��߂�����`�悵�Ȃ�
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;// �J�����̕������擾
	cameraDir.Normalize();// ���K��
	m_particleUtility.sort(								// �\�[�g
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);// ���ςŃ\�[�g
		});
	m_vertices.clear();// ���_���N���A
	for (ParticleUtility& li : m_particleUtility)// ���X�g�̃p�[�e�B�N����S�ĕ`�悷��
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) // ���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
			continue;
		VertexPositionColorTexture vPCT{};// ���_�̍\���̂��쐬
		vPCT.position = XMFLOAT3(li.GetPosition());// ���W
		vPCT.color = XMFLOAT4(li.GetNowColor());// �e�N�X�`���̐F
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);// ���݂̃e�N�X�`���̃X�P�[�����uXMFLOAT2�v��X�ɓ����
		m_vertices.push_back(vPCT);// ���_��ǉ�
	}
	if (m_vertices.empty())return;//	�\������_���Ȃ��ꍇ�͕`����I���
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	m_constantBuffer.matView = view.Transpose();// �r���[�s��
	m_constantBuffer.matProj = proj.Transpose();// �v���W�F�N�V�����s��
	m_constantBuffer.matWorld = m_billboard.Transpose();// ���[���h�s��
	m_constantBuffer.colors = Vector4(1, 1, 1, 0);// �F
	m_constantBuffer.count = Vector4((float)(m_anim));// �t���[����
	m_constantBuffer.height = Vector4((float)(m_frameRows));// �s��
	m_constantBuffer.width = Vector4((float)(m_frameCols));// ��
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);// �V�F�[�_�[�Ƀo�b�t�@��n��
	if (m_type == ParticleUtility::Type::BARRIERBREAK)// �o���A����ꂽ�Ƃ��̔j�Ђ͐[�x�o�b�t�@���g��Ȃ�
	{

		m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
			DrawPolygon::SamplerStates::ANISOTROPIC_WRAP,//�T���v���[�X�e�[�g
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
			DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
			DrawPolygon::DepthStencilStates::DEPTH_READ);// �[�x�X�e���V���X�e�[�g
	}
	else// ����ȊO�̃p�[�e�B�N���͐[�x�o�b�t�@���g��
	{

		m_pDrawPolygon->DrawSetting(// �`��O�ݒ�
			DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
			DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
			DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
			DrawPolygon::DepthStencilStates::DEPTH_READ); // �[�x�X�e���V���X�e�[�g
	}
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);	// �`�揀��
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());// �w�肵�����W�𒆐S�ɁA�V�F�[�_���Ŕ|���S���𐶐��E�`�悳����
	m_pDrawPolygon->ReleaseShader();// �V�F�[�_�̓o�^���������Ă���
}
/*
*	@brief �r���{�[�h�s��̍쐬
*	@detail �r���{�[�h�s��̍쐬�������s��
*	@param target �����_
*	@param eye �J�����̈ʒu
*	@param up �J�����̏����
*	@return �Ȃ�
*/
void Particle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	using namespace DirectX::SimpleMath;
	m_billboard = Matrix::CreateBillboard(Vector3::Zero, eye - target, up);// �r���{�[�h�s��̍쐬
	Matrix rot = Matrix::Identity;// �P�ʍs��
	rot._11 = -1;// X�����]
	rot._33 = -1;// Z�����]
	m_cameraPosition = eye;// �J�����̈ʒu
	m_cameraTarget = target;// �J�����̒����_
	m_billboard = rot * m_billboard;// �r���{�[�h�s��̔��]
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
	if (m_timer >= m_elapsedTime)// �^�C�}�[����莞�Ԃ𒴂�����V�����p�[�e�B�N���𐶐�
	{
		std::random_device seed;// �����̐ݒ�
		std::default_random_engine engine(seed());// �����Z���k�E�c�C�X�^�@
		std::uniform_real_distribution<> angleDist(0, XM_2PI);// �����_���Ȋp�x�i0 ���� 2�� �܂Łj
		std::uniform_real_distribution<> speedDist(0.5f, 2.0f);// �����_���ȑ��x�͈̔͂�ݒ�
		// �����_���Ȋp�x
		float randAngleXY = static_cast<float>(angleDist(engine));// XY���ʏ�̃����_���Ȋp�x
		float randAngleXZ = static_cast<float>(angleDist(engine)); // XZ���ʏ�̃����_���Ȋp�x
		float speed = static_cast<float>(speedDist(engine));// �����_���ȑ��x
		Vector3 randomVelocity = speed * Vector3(	// �����_���ȕ����̑��x�x�N�g��
			cosf(randAngleXY) * sinf(randAngleXZ),// X����
			cosf(randAngleXZ),					 // Y����
			sinf(randAngleXY) * sinf(randAngleXZ)// Z����
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
			m_particleUtility.push_back(pU);// ���������p�[�e�B�N�������X�g�ɒǉ�
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
			m_particleUtility.push_back(pU);// ���������p�[�e�B�N�������X�g�ɒǉ�
		}
		m_timer = 0.0f;// �^�C�}�[�����Z�b�g
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
	if (m_timer >= m_elapsedTime)  // �o���A����ꂽ�Ƃ��̔j�Ђ͏����Ԋu�����߂ł��悢
	{
		std::random_device seed;// �����̐ݒ�
		std::default_random_engine engine(seed()); // �����Z���k�E�c�C�X�^�@
		std::uniform_real_distribution<> angleDist(0, XM_2PI); // �S���������_���Ȋp�x
		std::uniform_real_distribution<> speedDist(2.5f, 3.0f); // ���q�̑��x�͈�
		std::uniform_real_distribution<> sizeDist(1.5f, 5.0f);  // �j�Ђ̃T�C�Y�������_���ɐݒ�
		std::uniform_real_distribution<> heightDist(-1.0f, 1.0f); // ���������i-1�`1�j�̃����_���͈�
		for (int i = 0; i < 20; ++i)  // �j�Ђ𕡐�����
		{
			// �����_���ȕ����Ŕ�΂����߂̋��ʍ��W
			float theta = static_cast<float>(angleDist(engine)); // ���������̊p�x (0�`2��)
			float phi = acosf(static_cast<float>(heightDist(engine))); // ���������̊p�x (0�`��)
			float speed = static_cast<float>(speedDist(engine)) * 5.0f;// �����_���ȑ��x
			Vector3 randomVelocity = speed * Vector3(// ���ʍ��W�n���f�J���g���W�n�ɕϊ����đ��x�x�N�g�����v�Z
				sinf(phi) * cosf(theta), // X����
				cosf(phi),              // Y����
				sinf(phi) * sinf(theta) // Z����
			);
			float randomSize = static_cast<float>(sizeDist(engine)) * m_barrierBreakSize;// �����_���ȃT�C�Y
			ParticleUtility pU(// �p�[�e�B�N���̐���
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
			m_particleUtility.push_back(pU);// ���������p�[�e�B�N�������X�g�ɒǉ�
		}
	}
}
