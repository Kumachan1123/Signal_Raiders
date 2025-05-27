/*
*	@file	Fade.h
*	@brief	�t�F�[�h�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include "Libraries/MyLib/DebugString.h"
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/KumachiLib.h"
// �O���錾
class CommonResources;
class Fade
{
public:
	// �\����
	struct ConstBuffer
	{
		float smoothness = 0;                             // �t�F�[�h�̊��炩��
		float fadeAmount = 0;                       // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
		DirectX::SimpleMath::Vector2 padding;							// �p�f�B���O
	};
	// �񋓌^
	enum class FadeState
	{
		None,// �Ȃ�
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̊J�n���
		FadeIn,// �t�F�[�h�C��
		FadeOut,// �t�F�[�h�A�E�g
		// �ȉ��A�t�F�[�h�C���E�A�E�g�̓r�����
		FadeInEnd,// �t�F�[�h�C���I��
		FadeOutEnd,// �t�F�[�h�A�E�g�I��
	};


public:
	// �A�N�Z�T
	void SetState(FadeState state) { m_fadeState = state; }
	FadeState GetState() const { return m_fadeState; }
public:
	// public�֐�
	Fade(CommonResources* commonResources);	//	�R���X�g���N�^
	~Fade();	//	�f�X�g���N�^
	void LoadTexture(const wchar_t* path);// �e�N�X�`���̓ǂݍ���
	void Initialize();// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:
	// private�֐�
	void CreateShaders();// �V�F�[�_�[�̍쐬
	void FadeOut(float elapsedTime);// �t�F�[�h�A�E�g
	void FadeIn(float elapsedTime);// �t�F�[�h�C��
public:
	// public�萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �t�F�[�h�ŏ��l
	static const float FADE_MIN;
	// �t�F�[�h�ő�l
	static const float FADE_MAX;
	// �t�F�[�h���x
	static const float FADE_SPEED;
	// �t�F�[�h�����l
	static const float FADE_INIT;
	// �t�F�[�h�̊��炩��
	static const float FADE_SMOOTHNESS;
private:
	// �ϐ�
	DX::DeviceResources* m_pDR;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;

	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;
	// �t�F�[�h���
	FadeState m_fadeState;
	// ����
	float m_time;
};