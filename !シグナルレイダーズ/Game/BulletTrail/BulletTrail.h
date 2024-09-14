/*
	@file	BulletTrail.h
	@brief	�e�̋O�ՃN���X
	�쐬�ҁF���܂�
*/
#pragma once
//�O���錾
class CommonResources;
#include "Game/CommonResources.h"
#include <vector>
#include <list>

#include <Game/ParticleUtility/ParticleUtility.h>
class BulletTrail
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};
	//�ϐ�
public:

	DirectX::SimpleMath::Vector3 m_bulletPos;// �e�̍��W

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//	�ϐ�
	DX::DeviceResources* m_pDR;
	float m_timer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::SimpleMath::Matrix m_world;// ���[���h�s��
	DirectX::SimpleMath::Matrix m_view;// �r���[�s��
	DirectX::SimpleMath::Matrix m_proj;	// �v���W�F�N�V�����s��

	DirectX::SimpleMath::Vector3 m_cameraPosition;// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraTarget;// �J�����̒����_
	DirectX::SimpleMath::Vector3 m_cameraUp;// �J�����̏����
	DirectX::SimpleMath::Matrix m_billboard;// �r���{�[�h�s��

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ParticleUtility> m_particleUtility;
	//	�֐�
public:
	BulletTrail();
	~BulletTrail();
	void Initialize(CommonResources* resources);
	void LoadTexture(const wchar_t* path);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);
	//	Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 bulletPos) { m_bulletPos = bulletPos; }
	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPos) { m_cameraPosition = cameraPos; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 cameraTarget) { m_cameraTarget = cameraTarget; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 cameraUp) { m_cameraUp = cameraUp; }
private:
	void CreateShader();
	void Trail();

};