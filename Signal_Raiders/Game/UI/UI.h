/*
*	@file		UI.h
*	@brief		UI�N���X(�^�C�g���E���U���g�E�Z�b�e�B���O)
*/
#pragma once
// �W�����C�u����
#include <vector>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/Anchor.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

class  UI
{
public:// �\����
	struct ConstBuffer// �V�F�[�_�[�ɓn����
	{
		DirectX::SimpleMath::Vector4 windowSize;// �E�B���h�E�T�C�Y
		float time;// ����
		DirectX::SimpleMath::Vector3 color;// �F
	};

	enum class ShaderType
	{
		NORMAL,
		STAGE_SELECT
	};
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	UI();
	~UI();
	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void Update(float elapsedTime);

	void Render();

	void SetWindowSize(const int& width, const int& height);
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; };
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	void SetTime(float time) { m_time = time; };
	float GetTime() const { return m_time; }
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
private:

	void CreateShaders();
public:// public�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ����
	float m_time;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// �e�N�X�`�����\�[�X
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_�[(���ʂ̃��j���[)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �s�N�Z���V�F�[�_�[(�X�e�[�W�Z���N�g�p)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShaderStageSelect;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// �V�F�[�_�[�̍\����(���ʂ̃��j���[)
	DrawPolygon::Shaders m_shaders;
	// �V�F�[�_�[�̍\����(�X�e�[�W�Z���N�g�p)
	DrawPolygon::Shaders m_StageSelectShaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �R���X�^���g�o�b�t�@(�V�F�[�_�[�ɓn�����)
	ConstBuffer m_constBuffer{};
	// �V�F�[�_�[�^�C�v
	ShaderType m_shaderType;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �e�N�X�`���̕��ƍ���
	int m_textureWidth, m_textureHeight;
	// �X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;
	// �x�[�X�X�P�[��
	DirectX::SimpleMath::Vector2 m_baseScale;
	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;
	// �A���J�[
	KumachiLib::ANCHOR m_anchor;
};
