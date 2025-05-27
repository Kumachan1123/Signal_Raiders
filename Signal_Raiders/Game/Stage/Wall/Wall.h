/*
*	@file Wall.h
*	@brief �ǃN���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX
#include <DeviceResources.h>
#include <WICTextureLoader.h> 
#include <SimpleMath.h>
// �O�����C�u����
#include "Libraries/Microsoft/DebugDraw.h"
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
class Wall
{
public:// �\����
	struct ConstBuffer// �V�F�[�_�[�ɓn�����
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;    // ����
	};
public:// public�֐�
	Wall(CommonResources* resources);// �R���X�g���N�^
	~Wall();// �f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`���̓ǂݍ���
	void Create(DX::DeviceResources* pDR);// ������
	void Update(float elapsedTime);// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �`��
	DirectX::BoundingBox& GetBoundingBox(int index) { return m_wallBox[index]; }// �����蔻��̎擾
	int GetWallNum() { return WALL_NUM; }// �ǂ̐��̎擾
private:
	void CreateWalls();// �ǂ̏�����
	void CreateShaders();// �V�F�[�_�[�̍쐬
public:// �萔
	// �ǂ̐�
	static const int WALL_NUM = 4;
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// ����
	static const float WALL_HEIGHT;
	// ��
	static const float WALL_WIDTH;
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �ǃe�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pWallTexture;
	// �ǂ̒��_
	DirectX::VertexPositionTexture m_wall[Wall::WALL_NUM][WALL_NUM];
	// �ǂ̓����蔻��
	DirectX::BoundingBox m_wallBox[WALL_NUM];
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �R���X�^���g�o�b�t�@(�V�F�[�_�[�ɓn�����)
	ConstBuffer m_constBuffer;
	// ���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;

};