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
#include "Game/CommonResources.h"
#include "Game/KumachiLib/Anchor.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class  UI
{
public:// �񋓌^
	// �V�F�[�_�[�^�C�v
	enum class ShaderType
	{
		NORMAL = 0,// �ʏ�̃��j���[�p
		STAGE_SELECT// �X�e�[�W�Z���N�g�p
	};
public:// �\����
	// �V�F�[�_�[�ɓn���l
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 windowSize;// �E�B���h�E�T�C�Y
		float time;// ����
		DirectX::SimpleMath::Vector3 color;// �F
	};

public:// �A�N�Z�T
	// �E�B���h�E�̃T�C�Y��ݒ�
	void SetWindowSize(const int& width, const int& height);
	// �V�F�[�_�[�^�C�v��ݒ�
	void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; }
	// �X�P�[�����擾
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }
	// �X�P�[����ݒ�
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }
	// �I����Ԃ̃X�P�[�����擾
	DirectX::SimpleMath::Vector2 GetSelectScale() const { return m_baseScale; }
	// �ʒu���擾
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }
	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& position) { m_position = position; };
	// �A���J�[���擾
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }
	// �A���J�[��ݒ�
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; }
	// ���Ԃ��擾
	float GetTime() const { return m_time; }
	// ���Ԃ�ݒ�
	void SetTime(float time) { m_time = time; }
	// �w��ʒu�Ƀq�b�g�������ǂ������擾
	bool IsHit(const DirectX::SimpleMath::Vector2& pos) const;
public:// public�֐�
	// �R���X�g���N�^
	UI(CommonResources* pCommonResources);
	// �f�X�g���N�^
	~UI();
	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(std::string key);
	// UI�̍쐬
	void Create(DX::DeviceResources* pDR
		, const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
private:// private�֐�
	// �V�F�[�_�[�̍쐬
	void CreateShaders();
public:// �萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
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
	// �e�N�X�`���̔z��
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
