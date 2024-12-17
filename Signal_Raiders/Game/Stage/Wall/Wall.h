#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <cassert>
#include <SimpleMath.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
class Wall
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;    // ����
	}m_constBuffer;
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �ǃe�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pWallTexture;
	// �ǂ̒��_
	DirectX::VertexPositionTexture m_wall[4][4];
	// �ǂ̓����蔻��
	DirectX::BoundingBox m_wallBox[4];
	// ���[���h�r���[�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;

	//	�v���~�e�B�u�o�b�` 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_pPrimitiveBatch;


	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	���ʃX�e�[�g�I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<DirectX::CommonStates> m_pStates;


	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;


public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �����X�e�[�^�X��ݒ�
	Wall(CommonResources* resources);
	~Wall();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);
	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	DirectX::BoundingBox& GetBoundingBox(int index) { return m_wallBox[index]; }
private:

	void CreateShader();
};