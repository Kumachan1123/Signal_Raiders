/*
	@file	Particle.h
	@brief	�p�[�e�B�N���N���X
*/
#pragma once
//�O���錾
class CommonResources;
#include "Game/CommonResources.h"
#include <vector>
#include <list>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include <Game/ParticleUtility/ParticleUtility.h>
class Particle
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	colors;

	};

	//�ϐ�
public:

	DirectX::SimpleMath::Vector3 m_bulletPosition;// �e�̍��W
	DirectX::SimpleMath::Vector3 m_bossPosition;// �{�X�̍��W
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	�ϐ�
	DX::DeviceResources* m_pDR;
	float m_timer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constantBuffer;


	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	DirectX::SimpleMath::Matrix m_world;// ���[���h�s��
	DirectX::SimpleMath::Matrix m_view;// �r���[�s��
	DirectX::SimpleMath::Matrix m_proj;	// �v���W�F�N�V�����s��

	DirectX::SimpleMath::Vector3 m_cameraPosition;// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraTarget;// �J�����̒����_
	DirectX::SimpleMath::Vector3 m_cameraUp;// �J�����̏����
	DirectX::SimpleMath::Matrix m_billboard;// �r���{�[�h�s��

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ParticleUtility> m_particleUtility;
	// �p�[�e�B�N���^�C�v
	ParticleUtility::Type m_type;
	// �p�[�e�B�N���̃T�C�Y
	float m_size;
	//	�֐�
public:
	Particle(ParticleUtility::Type type, float size);
	~Particle();
	void Initialize(CommonResources* resources);
	void LoadTexture(const wchar_t* path);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);
	//	Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 bulletPos) { m_bulletPosition = bulletPos; }
	void SetBossPosition(DirectX::SimpleMath::Vector3 bossPos) { m_bossPosition = bossPos; }

	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPos) { m_cameraPosition = cameraPos; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 cameraTarget) { m_cameraTarget = cameraTarget; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 cameraUp) { m_cameraUp = cameraUp; }
private:
	void CreateShader();
	void Trail();
	void BarrierBreak();
};