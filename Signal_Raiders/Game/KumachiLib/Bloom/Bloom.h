// �u���[���G�t�F�N�g�̃w�b�_�t�@�C��

#pragma once
#include <PostProcess.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "Game/KumachiLib/RenderTexture/RenderTexture.h"
//�O���錾
class CommonResources;
class Bloom
{
public:
	// �V���O���g���C���X�^���X���擾
	static Bloom* const GetInstance();
public:
	// �\����
	struct Blur
	{
		ID3D11RenderTargetView* RTV;
		ID3D11ShaderResourceView* SRV;
	};

public:
	// �f�X�g���N�^
	~Bloom();

	// �|�X�g�v���Z�X�𐶐�
	void CreatePostProcess(CommonResources* resources);
	// �I�t�X�N���[���`��p��RTV��؂�ւ���
	void ChangeOffScreenRT();
	// �|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
	void PostProcess();
	// �����_�[�^�[�Q�b�g�����ɖ߂�
	void ChangeDefaultRT();
private:
	// �R���X�g���N�^
	Bloom();
	// �����_�[�e�N�X�`�����쐬����
	void CreateRenderTexture();
	// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�̋֎~
	Bloom(const Bloom&) = delete;
	Bloom& operator=(const Bloom&) = delete;;

private:
	// �V���O���g���C���X�^���X
	static std::unique_ptr<Bloom> m_instance;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���ʃ��\�[�X
	ID3D11DeviceContext1* m_context;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	ID3D11Device1* m_device;
	// �u���[���G�t�F�N�g�̃R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �u���[
	Blur m_blur1;
	Blur m_blur2;
	// �|�X�g�v���Z�X
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;
	// �X�N���[���T�C�Y
	RECT m_screenSize;
	// �����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_offScreenRT;
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;
	std::unique_ptr<DX::RenderTexture> m_savedOffScreenRT;
	// �I�t�X�N���[���p�̃����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_offScreenRTV;
	// �I�t�X�N���[���p�̃V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_offScreenSRV;
	// �f�t�H���g�̐[�x�X�e���V���r���[
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_defaultDSV;
	// �f�t�H���g�̃����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_defaultRTV;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

};

