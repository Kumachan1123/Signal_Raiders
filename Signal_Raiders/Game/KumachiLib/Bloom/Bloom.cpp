#include <pch.h>
#include "Bloom.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<Bloom> Bloom::m_instance = nullptr;
Bloom* const Bloom::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new Bloom());
	}
	return m_instance.get();
}
Bloom::Bloom()
	: m_context{}
	, m_pDR{}
	, m_device{}
	, m_commonResources{}
	, m_blur1{}
	, m_blur2{}
	, m_basicPostProcess{}
	, m_dualPostProcess{}
	, m_screenSize{}
	, m_states{}
{
}
Bloom::~Bloom()
{
	// �|�C���^�[�̉��
	m_commonResources = nullptr;
	m_context = nullptr;
	m_pDR = nullptr;
	m_device = nullptr;
}
// -----------------------------------------------------------
// �|�X�g�v���Z�X�ƃf���A���|�X�g�v���Z�X�I�u�W�F�N�g�����
// -----------------------------------------------------------
void Bloom::CreatePostProcess(CommonResources* resources)
{
	m_commonResources = resources;
	m_pDR = m_commonResources->GetDeviceResources();
	m_device = m_pDR->GetD3DDevice();
	m_context = m_pDR->GetD3DDeviceContext();
	this->CreateRenderTexture();
	m_basicPostProcess = std::make_unique<BasicPostProcess>(m_device);
	m_dualPostProcess = std::make_unique<DualPostProcess>(m_device);
}

void Bloom::ChangeOffScreenRT()
{
	// �I�t�X�N���[���p��RTV���擾
	m_offScreenRTV = m_offScreenRT->GetRenderTargetView();
	m_defaultDSV = m_commonResources->GetDeviceResources()->GetDepthStencilView();

	// RTV��DSV���o�C���h
	m_context->OMSetRenderTargets(1, m_offScreenRTV.GetAddressOf(), m_defaultDSV.Get());

	// �^�[�Q�b�g�r���[���N���A
	m_context->ClearRenderTargetView(m_offScreenRTV.Get(), Colors::Black);
	m_context->ClearDepthStencilView(m_defaultDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// -----------------------------------------------------------
//	�|�X�g�v���Z�X�ɕK�v�Ȑݒ����������
// -----------------------------------------------------------
void Bloom::PostProcess()
{
	m_blur1.RTV = m_blur1RT->GetRenderTargetView();
	m_blur1.SRV = m_blur1RT->GetShaderResourceView();
	m_blur2.RTV = m_blur2RT->GetRenderTargetView();
	m_blur2.SRV = m_blur2RT->GetShaderResourceView();
	m_offScreenSRV = m_offScreenRT->GetShaderResourceView();
	// �V�F�[�_�[���\�[�X�r���[�̉���
	ID3D11ShaderResourceView* nullsrv[] = { nullptr,nullptr };
	D3D11_VIEWPORT vp = { 0.0f,0.0f,m_screenSize.right / 2.0f,m_screenSize.bottom / 2.0f,0.0f,1.0f };
	m_context->RSSetViewports(1, &vp);
	// -----------------------------------------------------
	//	Pass1::offscreen -> blur1 �� ���邢�����𒊏o����
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
	m_basicPostProcess->SetBloomExtractParameter(0.35f);
	m_basicPostProcess->SetSourceTexture(m_offScreenSRV.Get());
	m_basicPostProcess->Process(m_context);
	// -----------------------------------------------------
	//	Pass2::blur1 -> blur2 �� ���ɂڂ���
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur2.RTV, nullptr);
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	m_basicPostProcess->SetBloomBlurParameters(true, 1.50f, 1.10f);
	m_basicPostProcess->SetSourceTexture(m_blur1.SRV);
	m_basicPostProcess->Process(m_context);
	m_context->PSSetShaderResources(0, 1, nullsrv);
	// -----------------------------------------------------
	//	Pass3::blur2 -> blur1 �� �c�ɂڂ���
	// -----------------------------------------------------
	m_context->OMSetRenderTargets(1, &m_blur1.RTV, nullptr);
	m_basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	m_basicPostProcess->SetBloomBlurParameters(false, 1.50f, 1.10f);
	m_basicPostProcess->SetSourceTexture(m_blur2.SRV);
	m_basicPostProcess->Process(m_context);
	// -----------------------------------------------------
	// Pass4::offscreen -> blur1 �� �t���[���o�b�t�@�ɕ`�悷��
	// -----------------------------------------------------
	m_defaultRTV = m_commonResources->GetDeviceResources()->GetRenderTargetView();
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), nullptr);
	const auto& defaultVP = m_commonResources->GetDeviceResources()->GetScreenViewport();
	m_context->RSSetViewports(1, &defaultVP);
	m_dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
	m_dualPostProcess->SetBloomCombineParameters(1.125f, 1.0f, 1.0f, 1.0f);
	m_dualPostProcess->SetSourceTexture(m_offScreenSRV.Get());
	m_dualPostProcess->SetSourceTexture2(m_blur1.SRV);
	m_dualPostProcess->Process(m_context);
	// �V�F�[�_�[���\�[�X�r���[������
	m_context->PSSetShaderResources(0, 2, nullsrv);
}
// -----------------------------------------------------------
// �����_�[�^�[�Q�b�g�����ɖ߂�
// -----------------------------------------------------------
void Bloom::ChangeDefaultRT()
{
	m_context->OMSetRenderTargets(1, m_defaultRTV.GetAddressOf(), m_defaultDSV.Get());
}

// -----------------------------------------------------------
// �����_�[�e�N�X�`�����쐬����
// -----------------------------------------------------------
void Bloom::CreateRenderTexture()
{
	m_screenSize = m_commonResources->GetDeviceResources()->GetOutputSize();

	RECT halfSize{ 0,0, m_screenSize.right / 2, m_screenSize.bottom / 2 };

	m_offScreenRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_offScreenRT->SetDevice(m_device);
	m_offScreenRT->SetWindow(m_screenSize);

	m_blur1RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_blur1RT->SetDevice(m_device);
	m_blur1RT->SetWindow(halfSize);

	m_blur2RT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_B8G8R8A8_UNORM);
	m_blur2RT->SetDevice(m_device);
	m_blur2RT->SetWindow(halfSize);
}