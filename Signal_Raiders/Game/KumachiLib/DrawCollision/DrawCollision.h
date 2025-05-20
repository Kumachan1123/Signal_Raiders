/*
	@file	DrawCollision.h
	@brief	�����蔻��`��p�N���X
*/
#pragma once
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <Libraries/Microsoft/DebugDraw.h>
//�O���錾
class CommonResources;

class DrawCollision
{
public:
	// public�֐�
	static void Initialize(CommonResources* commonResources);// ������
	static void DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �`��J�n
	static void DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color);// ���E���`��
	static void DrawEnd();// �`��I��
	static void Finalize();// ���
private:
	// private�ϐ�
	// �x�[�V�b�N�G�t�F�N�g
	static std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���ʃ��\�[�X
	static CommonResources* m_commonResources;
};