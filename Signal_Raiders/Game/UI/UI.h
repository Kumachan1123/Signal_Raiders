//--------------------------------------------------------------------------------------
//@file   UI.h
//@brief  UI�N���X
//-------------------------------------------------------------------------------------

#pragma once
#include "Game/KumachiLib/Anchor.h"
#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
class  UI
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 windowSize;
		float time;
		DirectX::SimpleMath::Vector3 color;
	};
	ConstBuffer m_constBuffer{};
public:
	//	�ϐ�
	DX::DeviceResources* m_pDR;// �f�o�C�X���\�[�X
	float m_time;// ����
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;// ���̓��C�A�E�g
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_pPrimitiveBatch;// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::CommonStates> m_pStates;// �X�e�[�g
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;

	int m_windowWidth, m_windowHeight;
	int m_textureWidth, m_textureHeight;
	DirectX::SimpleMath::Vector2 m_scale;// �X�P�[��
	DirectX::SimpleMath::Vector2 m_baseScale;// �x�[�X�X�P�[��
	DirectX::SimpleMath::Vector2 m_position;// �ʒu
	// �A���J�[
	KumachiLib::ANCHOR m_anchor;
	// �֐�
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

	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	void SetTime(float time) { m_time = time; };
	float GetTime() const { return m_time; }

private:

	void CreateShader();
};
