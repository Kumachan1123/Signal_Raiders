/*
*	@file	DrawCollision.h
*	@brief	�����蔻��`��p�N���X
*/
#pragma once
#ifndef DRAWCOLLISION_DEFINED
#define DRAWCOLLISION_DEFINED
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"

//�O���錾
class CommonResources;

// �����蔻��`��p�N���X
class DrawCollision
{
public:
	// public�֐�
	// ������
	static void Initialize(CommonResources* commonResources);
	// �`��J�n
	static void DrawStart(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// ���E���`��
	static void DrawBoundingSphere(const DirectX::BoundingSphere& bs, const DirectX::XMVECTOR& color);
	// �`��I��
	static void DrawEnd();
	// ���
	static void Finalize();
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
#endif // DRAWCOLLISION_DEFINED