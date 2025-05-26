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
	// �񋓌^
	enum class ShaderType// �V�F�[�_�[�^�C�v
	{
		NORMAL = 0,// �ʏ�̃��j���[�p
		STAGE_SELECT// �X�e�[�W�Z���N�g�p
	};
public:// �A�N�Z�T
	void SetWindowSize(const int& width, const int& height);// �E�B���h�E�̃T�C�Y��ݒ�
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; };// �V�F�[�_�[�^�C�v��ݒ�
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }// �X�P�[�����擾
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };// �X�P�[����ݒ�
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }// �I����Ԃ̃X�P�[�����擾
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; } // �ʒu���擾
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };// �ʒu��ݒ�
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }// �A���J�[���擾
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }// �A���J�[��ݒ�
	float GetTime() const { return m_time; }// ���Ԃ��擾
	void SetTime(float time) { m_time = time; }// ���Ԃ�ݒ�
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const; // �w��ʒu�Ƀq�b�g�������ǂ������擾
public:// public�֐�
	UI();// �R���X�g���N�^
	~UI();// �f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`���̓ǂݍ���
	void Create(DX::DeviceResources* pDR// UI�̍쐬
		, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:// private�֐�
	void CreateShaders();// �V�F�[�_�[�̍쐬
public:// �萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// �C���v�b�g���C�A�E�g
private:// private�ϐ�
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
