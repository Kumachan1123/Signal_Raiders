//--------------------------------------------------------------------------------------
//@file    PlayerUI.h
//@brief  �v���C���[UI�N���X
//-------------------------------------------------------------------------------------


#pragma once
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/CommonResources.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/Anchor.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>


class PlayerUI
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
		DirectX::SimpleMath::Vector3	nul;
		float							renderRatio;
	};
	ConstBuffer m_constBuffer{};

private:
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;
	// HP�ȊO�̃V�F�[�_�[
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// HP�Ŏg���V�F�[�_�[
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_hpVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_hpPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_hpShaders;


	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;

	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;
	DirectX::SimpleMath::Vector2 m_position;

	KumachiLib::ANCHOR m_anchor;

	float m_renderRatio;
	float m_renderRatioOffset;
	//�֐�
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	PlayerUI();
	~PlayerUI();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);

	void Render();

	void SetWindowSize(const int& width, const int& height);

	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetBaseScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }

	void SetRenderRatio(float ratio) { m_renderRatio = ratio; };
	float GetRenderRatio() const { return m_renderRatio; }

	void SetRenderRatioOffset(float offset) { m_renderRatioOffset = offset; };
	float GetRenderRatioOffset() const { return m_renderRatioOffset; }

private:

	void CreateShader();
};


