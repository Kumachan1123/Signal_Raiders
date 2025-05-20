/*
	@file	DrawCollision.cpp
	@brief	�����蔻��`��p�N���X
*/
#include "pch.h"
#include "DrawCollision.h"


// �ÓI�����o�[�̏�����
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_basicEffect = nullptr;// �x�[�V�b�N�G�t�F�N�g
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;// ���̓��C�A�E�g
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_primitiveBatch = nullptr;// �v���~�e�B�u�o�b�`
CommonResources* DrawCollision::m_commonResources = nullptr;// ���ʃ��\�[�X

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
	m_commonResources = commonResources;// ���ʃ��\�[�X��ݒ�
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	m_basicEffect = std::make_unique<BasicEffect>(device);// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect->SetVertexColorEnabled(true);// ���_�J���[��L���ɂ���
	DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColor>	// ���̓��C�A�E�g���쐬����
		(device, m_basicEffect.get(), m_pInputLayout.ReleaseAndGetAddressOf()));
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);	// �v���~�e�B�u�o�b�`���쐬����
}
/*
*	@brief �`��J�n
*	@details �`��J�n���ɌĂяo���֐�
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void DrawCollision::DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();// �f�o�C�X�R���e�L�X�g���擾
	auto states = m_commonResources->GetCommonStates();// �R�����X�e�[�g���擾
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);// �u�����h�X�e�[�g��ݒ肷��
	context->OMSetDepthStencilState(states->DepthRead(), 0);// �[�x�X�e���V���X�e�[�g��ݒ肷��
	context->RSSetState(states->CullNone());// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	context->IASetInputLayout(m_pInputLayout.Get());// ���̓��C�A�E�g��ݒ肷��
	// �x�[�V�b�N�G�t�F�N�g��ݒ肷��
	m_basicEffect->SetView(view);// �r���[�s���ݒ肷��
	m_basicEffect->SetProjection(proj);// �v���W�F�N�V�����s���ݒ肷��
	m_basicEffect->Apply(context);// �f�o�C�X�R���e�L�X�g�ɓK�p����
	m_primitiveBatch->Begin();// �`��J�n

}
/*
*	@brief ���E���`��
*	@details ���E����`�悷��
*	@param bs ���E��
*	@param color ���E���̐F
*/
void DrawCollision::DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color)
{
	DX::Draw(m_primitiveBatch.get(), bs, color);// ���E����`��
}
/*
*	@brief �`��I��
*	@details �`��I�����ɌĂяo���֐�
*	@param �Ȃ�
*/
void DrawCollision::DrawEnd()
{
	m_primitiveBatch->End();// �`��I��
}
/*
*	@brief �I������
*	@details �ÓI�����o�[���������
*	@param �Ȃ�
*/
void DrawCollision::Finalize()
{
	// �ÓI�����o�[�����
	m_basicEffect.reset();// �x�[�V�b�N�G�t�F�N�g�����
	m_pInputLayout.Reset();// ���̓��C�A�E�g�����
	m_primitiveBatch.reset();// �v���~�e�B�u�o�b�`�����
	m_commonResources = nullptr;// ���ʃ��\�[�X�����
}
