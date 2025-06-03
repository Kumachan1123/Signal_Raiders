/*
*	@file	DrawCollision.cpp
*	@brief	�����蔻��`��p�N���X
*/
#include <pch.h>
#include "DrawCollision.h"
// �ÓI�����o�[�̏�����
// �x�[�V�b�N�G�t�F�N�g
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_pBasicEffect = nullptr;
// ���̓��C�A�E�g
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;
// �v���~�e�B�u�o�b�`
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_pPrimitiveBatch = nullptr;
// ���ʃ��\�[�X
CommonResources* DrawCollision::m_pCommonResources = nullptr;
/*
*	@brief ������
*	@details �����蔻��`��p�N���X�̏��������s��
*	@param commonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void DrawCollision::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	m_pCommonResources = commonResources;
	// �f�o�C�X���擾
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_pBasicEffect = std::make_unique<BasicEffect>(device);
	// ���_�J���[��L���ɂ���
	m_pBasicEffect->SetVertexColorEnabled(true);
	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColor>(device, m_pBasicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	// �v���~�e�B�u�o�b�`���쐬����
	m_pPrimitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
}
/*
*	@brief �`��J�n
*	@details �`��J�n���ɌĂяo���֐�
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void DrawCollision::DrawStart(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// �e�p�����[�^��ݒ肷��
	// �u�����h�X�e�[�g��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	// �[�x�X�e���V���X�e�[�g��ݒ肷��
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_pInputLayout.Get());
	// ���̓��C�A�E�g��ݒ肷��
	// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
	// �r���[�s���ݒ肷��
	m_pBasicEffect->SetView(view);
	// �v���W�F�N�V�����s���ݒ肷��
	m_pBasicEffect->SetProjection(proj);
	// �f�o�C�X�R���e�L�X�g�ɓK�p����
	m_pBasicEffect->Apply(context);
	// �`��J�n
	m_pPrimitiveBatch->Begin();
}
/*
*	@brief ���E���`��
*	@details ���E����`�悷��
*	@param bs ���E��
*	@param color ���E���̐F
*	@return �Ȃ�
*/
void DrawCollision::DrawBoundingSphere(const DirectX::BoundingSphere& bs, const DirectX::XMVECTOR& color)
{
	// ���E����`��
	DX::Draw(m_pPrimitiveBatch.get(), bs, color);
}
/*
*	@brief �`��I��
*	@details �`��I�����ɌĂяo���֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawCollision::DrawEnd()
{
	// �`��I��
	m_pPrimitiveBatch->End();
}
/*
*	@brief �I������
*	@details �ÓI�����o�[���������
*	@param �Ȃ�
*	@return �Ȃ�
*/
void DrawCollision::Finalize()
{
	// �ÓI�����o�[�����
	// �x�[�V�b�N�G�t�F�N�g�����
	m_pBasicEffect.reset();
	// ���̓��C�A�E�g�����
	m_pInputLayout.Reset();
	// �v���~�e�B�u�o�b�`�����
	m_pPrimitiveBatch.reset();
	// ���ʃ��\�[�X�����
	m_pCommonResources = nullptr;
}
