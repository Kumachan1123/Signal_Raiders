/*
*  @file BossAppear.h
*  @brief �{�X�o�����o�N���X
*/
#pragma once
// �W�����C�u����
#include <Effects.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <DeviceResources.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// �N���X�̑O���錾
class CommonResources;

class BossAppear
{
	// �\����
private:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;		// ����	
	};
public:// �A�N�Z�T
	// SE���ʂ̐ݒ�
	void SetSEVolume(float volume) { m_seVolume = volume; }
public:// public�����o�֐�
	// �R���X�g���N�^
	BossAppear();
	// �f�X�g���N�^
	~BossAppear();
	// ������
	void Initialize(CommonResources* resources);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �I������
	void Finalize();
private:// private�����o�֐�
	// �V�F�[�_�[�̐ݒ�
	void SettingShader();
	// ���C���̉��o��`��
	void DrawMain();
	// �w�i�̉��o��`��
	void DrawBack();
private:// �萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private�����o�ϐ�
	//	���_�V�F�[�_(Main)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	�s�N�Z���V�F�[�_(Main)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	//  ���_�V�F�[�_(Back)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShaderBack;
	// �s�N�Z���V�F�[�_(Back)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderBack;
	// �V�F�[�_�[�̍\����(Main)
	DrawPolygon::Shaders m_shaders;
	// �V�F�[�_�[�̍\����(Back)
	DrawPolygon::Shaders m_shadersBack;
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
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constantBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �^�C�}�[
	float m_timer;
	// �V�F�[�_�[�ɓn���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// SE����
	float m_seVolume;
	// �Đ��t���O
	bool m_isPlaying;
};