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
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include  "Game/KumachiLib/BinaryFile/BinaryFile.h"
//�O���錾
class CommonResources;
class Particle
{
public:// �\����
	//�V�F�[�_�[�ɑ���R���X�^���g�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix	 matWorld;	// ���[���h�s��
		DirectX::SimpleMath::Matrix	 matView;	// �r���[�s��
		DirectX::SimpleMath::Matrix	 matProj;	// �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;	// �F
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��
	};
public:	// �A�N�Z�T
	// �e�̍��W��ݒ�
	void SetBulletPosition(const DirectX::SimpleMath::Vector3& bulletPos) { m_bulletPosition = bulletPos; }
	// �{�X�̍��W��ݒ�
	void SetBossPosition(const DirectX::SimpleMath::Vector3& bossPos) { m_bossPosition = bossPos; }
	// �J�����̈ʒu��ݒ�
	void SetCameraPosition(const DirectX::SimpleMath::Vector3& cameraPos) { m_cameraPosition = cameraPos; }
	// �J�����̒����_��ݒ�
	void SetCameraTarget(const DirectX::SimpleMath::Vector3& cameraTarget) { m_cameraTarget = cameraTarget; }
	// �J�����̏������ݒ�
	void SetCameraUp(const DirectX::SimpleMath::Vector3& cameraUp) { m_cameraUp = cameraUp; }
	// �V�[���h�j��̃T�C�Y��ݒ�
	void SetBarrierBreakSize(float size) { m_barrierBreakSize = size; }
public:	// public�֐�
	// �R���X�g���N�^
	Particle(ParticleUtility::Type type, float size);
	// �f�X�g���N�^
	~Particle();
	// ������
	void Initialize(CommonResources* resources);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �r���{�[�h�s��̍쐬
	void CreateBillboard(
		const DirectX::SimpleMath::Vector3& target,
		const DirectX::SimpleMath::Vector3& eye,
		const DirectX::SimpleMath::Vector3& up);
private:// private�֐�
	// �V�F�[�_�[�쐬
	void CreateShaders();
	// �O��
	void Trail();
	// �o���A�j��
	void BarrierBreak();
private:// �萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:// public�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
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
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
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