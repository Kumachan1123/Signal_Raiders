/*
*	@file PlayerUI.h
*	@brief �v���C���[UI�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/Anchor.h"
// �O���錾
class CommonResources;
class PlayerUI
{
public:// �񋓌^
	enum class ShaderType// �V�F�[�_�[�̎��
	{
		HP = 0,// HP�Q�[�W�̂悤�ȓ���������
		CIRCLE,// �~�`�Q�[�W�̂悤�ȓ���������
		ANIM,// UV�A�j���[�V����������
		OTHER,// ���̑�
	};
	// �\����
	// �V�F�[�_�[�ɑ���萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;	// �E�B���h�E�T�C�Y
		DirectX::SimpleMath::Vector2	frame;		// �t���[����
		float							animCount = 0.0f;	// �A�j���[�V�����J�E���g
		float							renderRatio = 0.0f;// �����_�����O�䗦
	};
public:// �A�N�Z�T
	void SetWindowSize(const int& width, const int& height);// �E�B���h�E�T�C�Y�̐ݒ�
	void SetScale(DirectX::SimpleMath::Vector2 scale) { m_scale = scale; };// �X�P�[���̐ݒ�
	DirectX::SimpleMath::Vector2 GetScale() const { return m_scale; }// �X�P�[���̎擾
	DirectX::SimpleMath::Vector2 GetBaseScale() const { return m_baseScale; }// �x�[�X�X�P�[���̎擾
	DirectX::SimpleMath::Vector2 GetPosition() const { return m_position; }// �`��ʒu�̎擾
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; };// �`��ʒu�̐ݒ�
	KumachiLib::ANCHOR GetAnchor() const { return m_anchor; }// �A���J�[�̎擾
	void SetAnchor(KumachiLib::ANCHOR anchor) { m_anchor = anchor; };// �A���J�[�̐ݒ�
	float GetRenderRatio() const { return m_renderRatio; }// �����_�����O�䗦�̎擾
	void SetRenderRatio(float ratio) { m_renderRatio = ratio; };// �����_�����O�䗦�̐ݒ�
	float GetRenderRatioOffset() const { return m_renderRatioOffset; }// �����_�����O�䗦�̃I�t�Z�b�g�̎擾
	void SetRenderRatioOffset(float offset) { m_renderRatioOffset = offset; };// �����_�����O�䗦�̃I�t�Z�b�g�̐ݒ�
	void SetShaderType(ShaderType type) { m_shaderType = type; }// �V�F�[�_�[�̎�ނ̐ݒ�
	void SetAnim(int anim) { m_anim = anim; }// �A�j���[�V�����̃t���[�����̐ݒ�
	void SetFrameRows(int rows) { m_frameRows = rows; }// �t���[���̍s���̐ݒ�
	void SetFrameCols(int cols) { m_frameCols = cols; }// �t���[���̗񐔂̐ݒ�
public:// public�֐�
	PlayerUI(CommonResources* pCommonResources);// �R���X�g���N�^
	~PlayerUI();// �f�X�g���N�^
	void LoadTexture(std::string key);// �e�N�X�`���̓ǂݍ���
	void Create(DX::DeviceResources* pDR// UI�̍쐬
		, std::string key
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
	void Render();// �`��

private:// private�֐�
	void CreateShaders();// �V�F�[�_�[�̍쐬
private:// �萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// �C���v�b�g���C�A�E�g
private:// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexture;
	// �e�N�X�`���n���h���̔z��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���\�[�X�n���h��
	Microsoft::WRL::ComPtr<ID3D11Resource> m_pTextureResource;
	// HP�ȊO�̃V�F�[�_�[
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	// HP�Ŏg���V�F�[�_�[
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pHPPixelShader;
	// �~�`�Q�[�W�̃V�F�[�_�[
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pCirclePixelShader;
	// UV�A�j���[�V�����p�V�F�[�_�[
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pAnimPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_hpShaders;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_circleShaders;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_animShaders;
	// �R���X�^���g�o�b�t�@�i�V�F�[�_�[�ɑ���f�[�^�j
	ConstBuffer m_constBuffer;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �e�N�X�`���̕��ƍ���
	int m_textureWidth, m_textureHeight;
	// �t���[���̍s�� 
	int m_frameRows;
	// �t���[���̗�
	int m_frameCols;
	// �t���[����
	int m_anim;
	// �X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;
	// �x�[�X�X�P�[��
	DirectX::SimpleMath::Vector2 m_baseScale;
	// �`��ʒu
	DirectX::SimpleMath::Vector2 m_position;
	// �A���J�[
	KumachiLib::ANCHOR m_anchor;
	// �V�F�[�_�[�̎��
	ShaderType m_shaderType;
	// �����_�����O�䗦
	float m_renderRatio;
	// �����_�����O�䗦�̃I�t�Z�b�g
	float m_renderRatioOffset;
};


