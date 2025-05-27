/*
*	@file Bloom.cpp
*	@brief �u���[���G�t�F�N�g�̎����t�@�C��
*	@details �֐����ĂԂ����Ńu���[���G�t�F�N�g�������ł���
*/
#include <pch.h>
#include "Bloom.h"

// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<Bloom> Bloom::m_instance = nullptr;
/*
*	@brief �V���O���g���C���X�^���X���擾
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
Bloom* const Bloom::GetInstance()
{
	if (m_instance == nullptr)// �C���X�^���X���Ȃ��ꍇ
	{
		m_instance.reset(new Bloom());// �C���X�^���X�𐶐�
	}
	return m_instance.get();// �C���X�^���X��Ԃ�
}
/*
*	@brief �R���X�g���N�^
*	@param �Ȃ�
*/
Bloom::Bloom()
	: m_context{}// �f�o�C�X�R���e�L�X�g
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_device{}// �f�o�C�X
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_blur1{}// �u���[1
	, m_blur2{}// �u���[2
	, m_basicPostProcess{}// �|�X�g�v���Z�X
	, m_dualPostProcess{}// �f���A���|�X�g�v���Z�X
	, m_screenSize{}// �X�N���[���T�C�Y
	, m_states{}// �R�����X�e�[�g
{
}
/*
*	@brief �f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Bloom::~Bloom()
{
	// �e��|�C���^�[�̉��
	m_pCommonResources = nullptr;// ���ʃ��\�[�X
	m_context = nullptr;// �f�o�C�X�R���e�L�X�g
	m_pDR = nullptr;// �f�o�C�X���\�[�X
	m_device = nullptr;// �f�o�C�X
}
/*
*	@brief �|�X�g�v���Z�X�𐶐�
*	@details �u���[���G�t�F�N�g���������邽�߂̃|�X�g�v���Z�X�𐶐�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Bloom::CreatePostProcess(CommonResources* resources)
{
	using namespace DirectX;
	m_pCommonResources = resources;// ���ʃ��\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();// �f�o�C�X���\�[�X���擾
	m_device = m_pDR->GetD3DDevice();// �f�o�C�X���擾
	m_context = m_pDR->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	this->CreateRenderTexture();// �����_�[�e�N�X�`�����쐬
	m_basicPostProcess = std::make_unique<BasicPostProcess>(m_device);// �|�X�g�v���Z�X�𐶐�
	m_dualPostProcess = std::make_unique<DualPostProcess>(m_device);// �f���A���|�X�g�v���Z�X�𐶐�
}
/*
*	@brief �I�t�X�N���[���`��p��RTV��؂�ւ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::ChangeOffScreenRT()
{
	using namespace DirectX;
	m_offScreenRTV = m_offScreenRT->GetRenderTargetView();// �I�t�X�N���[���p��RTV���擾
	m_defaultDSV = m_pCommonResources->GetDeviceResources()->GetDepthStencilView();// �f�t�H���g��DSV���擾
	m_context->OMSetRenderTargets(1, m_offScreenRTV.GetAddressOf(), m_defaultDSV.Get());// RTV��DSV���o�C���h
	m_context->ClearRenderTargetView(m_offScreenRTV.Get(), Colors::Black);// RTV���N���A
	m_context->ClearDepthStencilView(m_defaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);// DSV���N���A
}
/*
*	@brief �|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
*	@details �u���[���G�t�F�N�g���������邽�߂̃|�X�g�v���Z�X�����s����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::PostProcess()
{
	using namespace DirectX;
	m_blur1.RTV = m_blur1RT->GetRenderTargetView();// �u���[1�p��RTV���擾
	m_blur1.SRV = m_blur1RT->GetShaderResourceView();//	�u���[1�p��SRV���擾
	m_blur2.RTV = m_blur2RT->GetRenderTargetView();// �u���[2�p��RTV���擾
	m_blur2.SRV = m_blur2RT->GetShaderResourceView();//	�u���[2�p��SRV���擾
	m_offScreenSRV = m_offScreenRT->GetShaderResourceView();// �I�t�X�N���[���p��SRV���擾
	ID3D11ShaderResourceView* nullsrv[] = { nullptr,nullptr };	// �V�F�[�_�[���\�[�X�r���[�̉���
	D3D11_VIEWPORT vp = { 0.0f,0.0f,m_screenSize.right / 2.0f,m_screenSize.bottom / 2.0f,0.0f,1.0f };// �r���[�|�[�g�̐ݒ�
	m_context->RSSetViewports(1, &vp);// �r���[�|�[�g��ݒ�
	// -----------------------------------------------------
	//	Pass1::offscreen -> blur1 �� ���邢�����𒊏o����
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// �����_�[�^�[�Q�b�g��ݒ�
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);// �G�t�F�N�g��ݒ�
	m_basicPostProcess->SetBloomExtractParameter(0.35f);// 臒l��ݒ�
	m_basicPostProcess->SetSourceTexture(m_offScreenSRV.Get());// �I�t�X�N���[���p��SRV��ݒ�
	m_basicPostProcess->Process(m_context);// �|�X�g�v���Z�X�����s
	// -----------------------------------------------------
	//	Pass2::blur1 -> blur2 �� ���ɂڂ���
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur2.RTV, nullptr);// �����_�[�^�[�Q�b�g��ݒ�
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// �G�t�F�N�g��ݒ�
	m_basicPostProcess->SetBloomBlurParameters(true, 1.250f, 1.025f);// ���ڂ����̃p�����[�^��ݒ�
	m_basicPostProcess->SetSourceTexture(m_blur1.SRV);// �u���[1�p��SRV��ݒ�
	m_basicPostProcess->Process(m_context);// �|�X�g�v���Z�X�����s
	m_context->PSSetShaderResources(0, 1, nullsrv);// �V�F�[�_�[���\�[�X�r���[������
	// -----------------------------------------------------
	//	Pass3::blur2 -> blur1 �� �c�ɂڂ���
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);// �����_�[�^�[�Q�b�g��ݒ�
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);// �G�t�F�N�g��ݒ�
	m_basicPostProcess->SetBloomBlurParameters(false, 1.250f, 1.025f);// �c�ڂ����̃p�����[�^��ݒ�
	m_basicPostProcess->SetSourceTexture(m_blur2.SRV);// �u���[2�p��SRV��ݒ�
	m_basicPostProcess->Process(m_context);// �|�X�g�v���Z�X�����s
	// -----------------------------------------------------
	// Pass4::offscreen -> blur1 �� �t���[���o�b�t�@�ɕ`�悷��
	// -----------------------------------------------------
	m_defaultRTV = m_pCommonResources->GetDeviceResources()->GetRenderTargetView();// �f�t�H���g��RTV���擾
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), nullptr);// �����_�[�^�[�Q�b�g��ݒ�
	const auto& defaultVP = m_pCommonResources->GetDeviceResources()->GetScreenViewport();// �f�t�H���g�̃r���[�|�[�g���擾
	m_context->RSSetViewports(1, &defaultVP);// �r���[�|�[�g��ݒ�
	m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);// �G�t�F�N�g��ݒ�
	m_dualPostProcess->SetBloomCombineParameters(1.125f, 1.0f, 1.0f, 1.0f);// �u���[���̃p�����[�^��ݒ�
	m_dualPostProcess->SetSourceTexture(m_offScreenSRV.Get());// �I�t�X�N���[���p��SRV��ݒ�
	m_dualPostProcess->SetSourceTexture2(m_blur1.SRV); // �u���[1�p��SRV��ݒ�
	m_dualPostProcess->Process(m_context);// �|�X�g�v���Z�X�����s
	m_context->PSSetShaderResources(0, 2, nullsrv);// �V�F�[�_�[���\�[�X�r���[������
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), m_defaultDSV.Get());// �����_�[�^�[�Q�b�g�����ɖ߂�
}
/*
*	@brief �����_�[�e�N�X�`�����쐬����
*	@details �u���[���G�t�F�N�g���������邽�߂̃����_�[�e�N�X�`�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Bloom::CreateRenderTexture()
{
	m_screenSize = m_pCommonResources->GetDeviceResources()->GetOutputSize();// �X�N���[���T�C�Y���擾
	RECT halfSize{ 0,0, m_screenSize.right / 2, m_screenSize.bottom / 2 };// �����̃T�C�Y���擾
	m_offScreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// �I�t�X�N���[���p��RT���쐬
	m_offScreenRT->SetDevice(m_device);// �f�o�C�X��ݒ�
	m_offScreenRT->SetWindow(m_screenSize);// �E�B���h�E�T�C�Y��ݒ�
	m_blur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// �u���[1�p��RT���쐬
	m_blur1RT->SetDevice(m_device);// �f�o�C�X��ݒ�
	m_blur1RT->SetWindow(halfSize);// �E�B���h�E�T�C�Y��ݒ�
	m_blur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);// �u���[2�p��RT���쐬
	m_blur2RT->SetDevice(m_device);// �f�o�C�X��ݒ�
	m_blur2RT->SetWindow(halfSize); // �E�B���h�E�T�C�Y��ݒ�
}