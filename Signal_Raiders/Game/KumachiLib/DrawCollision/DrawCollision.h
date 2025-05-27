/*
*	@file	DrawCollision.h
*	@brief	�����蔻��`��p�N���X
*/
#pragma once
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
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
	static std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// ���̓��C�A�E�g
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pPrimitiveBatch;
	// ���ʃ��\�[�X
	static CommonResources* m_pCommonResources;
};