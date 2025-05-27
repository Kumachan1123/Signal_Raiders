/*
*	@file	Particle.h
*	@brief	�p�[�e�B�N���N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <algorithm>
#include <random>
#include <list>
// DirectX�̃��C�u����
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// �O�����C�u����
#include <DeviceResources.h>
#include "Game/CommonResources.h"
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include <Game/ParticleUtility/ParticleUtility.h>
#include  "Game/KumachiLib/BinaryFile/BinaryFile.h"
//�O���錾
class CommonResources;
class Particle
{
public:
	// �\����
	struct ConstBuffer//�V�F�[�_�[�ɑ���R���X�^���g�o�b�t�@
	{
		DirectX::SimpleMath::Matrix	 matWorld;	// ���[���h�s��
		DirectX::SimpleMath::Matrix	 matView;	// �r���[�s��
		DirectX::SimpleMath::Matrix	 matProj;	// �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;	// �F
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��
	};
public:
	// �A�N�Z�T
	void SetBulletPosition(DirectX::SimpleMath::Vector3 bulletPos) { m_bulletPosition = bulletPos; }// �e�̍��W��ݒ�
	void SetBossPosition(DirectX::SimpleMath::Vector3 bossPos) { m_bossPosition = bossPos; }// �{�X�̍��W��ݒ�
	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPos) { m_cameraPosition = cameraPos; }// �J�����̈ʒu��ݒ�
	void SetCameraTarget(DirectX::SimpleMath::Vector3 cameraTarget) { m_cameraTarget = cameraTarget; }// �J�����̒����_��ݒ�
	void SetCameraUp(DirectX::SimpleMath::Vector3 cameraUp) { m_cameraUp = cameraUp; }// �J�����̏������ݒ�
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }// �V�[���h�j��̃T�C�Y��ݒ�
public:
	// public�֐�
	Particle(ParticleUtility::Type type, float size);// �R���X�g���N�^
	~Particle();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void LoadTexture(const wchar_t* path);// �e�N�X�`���̓ǂݍ���
	void Update(float elapsedTime);// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �`��
	void CreateBillboard(// �r���{�[�h�s��̍쐬
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);
private:
	void CreateShaders();// �V�F�[�_�[�쐬
	void Trail();// �O��
	void BarrierBreak();// �o���A�j��
public:
	// public�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �o�ߎ���
	float m_timer;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �e�N�X�`���n���h���̔z��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// �R���X�^���g�o�b�t�@�i�\���́j
	ConstBuffer m_constantBuffer;
	// �t���[���̍s���Ɨ�
	int m_frameRows;
	int m_frameCols;
	// �t���[����
	int m_anim;
	// �A�j���[�V�����̑��x
	float m_animSpeed;
	// �A�j���[�V��������
	float m_animTime;
	// �t���[������
	float m_elapsedTime;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	//	�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// �e�̍��W
	DirectX::SimpleMath::Vector3 m_bulletPosition;
	// �{�X�̍��W
	DirectX::SimpleMath::Vector3 m_bossPosition;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraPosition;
	// �J�����̒����_
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �J�����̏����
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// �r���{�[�h�s��
	DirectX::SimpleMath::Matrix m_billboard;
	// ���_
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	// �p�[�e�B�N���̃��[�e�B���e�B
	std::list<ParticleUtility> m_particleUtility;
	// �p�[�e�B�N���^�C�v
	ParticleUtility::Type m_type;
	// �p�[�e�B�N���̃T�C�Y
	float m_size;
	// �V�[���h�j��̃T�C�Y
	float m_barrierBreakSize;
};