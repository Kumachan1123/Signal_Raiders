/*
*	@file Bloom.h
*	@brief �u���[���G�t�F�N�g�̃w�b�_�t�@�C��
*	@details �֐����ĂԂ����Ńu���[���G�t�F�N�g�������ł���
*/
#pragma once
// DirectX�̃w�b�_�t�@�C��
#include <PostProcess.h>
#include <CommonStates.h>
// �O�����C�u����
#include "DeviceResources.h"
#include "Game/CommonResources.h"
#include "Libraries/Microsoft/RenderTexture/RenderTexture.h"
//�O���錾
class CommonResources;

class Bloom
{
public:
	// �V���O���g���C���X�^���X���擾
	static Bloom* const GetInstance();
public:
	// �\����
	struct Blur// �u���[
	{
		ID3D11RenderTargetView* RTV;// �����_�[�^�[�Q�b�g�r���[
		ID3D11ShaderResourceView* SRV;// �V�F�[�_�[���\�[�X�r���[
	};

public:
	// public�֐�
	~Bloom();// �f�X�g���N�^
	void CreatePostProcess(CommonResources* resources);// �|�X�g�v���Z�X�𐶐�
	void ChangeOffScreenRT();// �I�t�X�N���[���`��p��RTV��؂�ւ���
	void PostProcess();// �|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
private:
	// private�֐�
	Bloom();// �R���X�g���N�^
	void CreateRenderTexture();// �����_�[�e�N�X�`�����쐬����
	Bloom(const Bloom&) = delete;// �R�s�[�R���X�g���N�^
	Bloom& operator=(const Bloom&) = delete;// ������Z�q�̋֎~
private:
	//private�ϐ�
	// �V���O���g���C���X�^���X
	static std::unique_ptr<Bloom> m_instance;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���ʃ��\�[�X
	ID3D11DeviceContext1* m_context;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �f�o�C�X
	ID3D11Device1* m_device;
	// �u���[���G�t�F�N�g�̃R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �u���[
	Blur m_blur1;
	Blur m_blur2;
	// �|�X�g�v���Z�X
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;
	// �f���A���|�X�g�v���Z�X
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;
	// �X�N���[���T�C�Y
	RECT m_screenSize;
	// �����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_offScreenRT;// �I�t�X�N���[���p��RT
	std::unique_ptr<DX::RenderTexture> m_blur1RT;// �u���[1�p��RT
	std::unique_ptr<DX::RenderTexture> m_blur2RT;// �u���[2�p��RT
	std::unique_ptr<DX::RenderTexture> m_savedOffScreenRT;// �ۑ����ꂽ�I�t�X�N���[���p��RT
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

