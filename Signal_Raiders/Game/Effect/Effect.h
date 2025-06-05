/*
*	@file Effect.h
*	@brief �G�t�F�N�g�N���X
*/
#pragma once
#ifndef EFFECT_DEFINED
#define EFFECT_DEFINED
// DirectX
#include <PrimitiveBatch.h>
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <DeviceResources.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <Libraries/MyLib/DebugString.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

// �O���錾
class CommonResources;

// �G�t�F�N�g�N���X
class Effect
{
public:
	// �񋓌^
	// �G�t�F�N�g�̎��
	enum class EffectType
	{
		ENEMY_DEAD = 0,// �G���S�G�t�F�N�g
		ENEMY_HIT,// �G�q�b�g�G�t�F�N�g
		NONE,// �Ȃ�
	};
public:
	// �\����
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��
	};
public:
	// �A�N�Z�T
	// �Đ������̃t���O�擾
	bool IsPlaying() const { return m_isPlaying; }
	// ���W�擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
public:
	// public�����o�֐�
	// �R���X�g���N�^
	Effect(CommonResources* resources,// ���ʃ��\�[�X
		EffectType type,// �G�t�F�N�g�̎��
		const DirectX::SimpleMath::Vector3& playPos,// �Đ�������W
		float scale,// �X�P�[��
		const DirectX::SimpleMath::Matrix& world);// ���[���h�s��
	// �f�X�g���N�^
	~Effect();
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �I������
	void Finalize();
private:
	// private�����o�֐�
	// �摜��ǂݍ���
	void LoadTexture(const wchar_t* path);
private:
	// �萔
	// ���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �|���S���̒��_���W
	static const float m_vertexMinX;//��
	static const float m_vertexMaxX;//�E
	static const float m_vertexMinY;//��
	static const float m_vertexMaxY;//��
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �G�t�F�N�g���Đ�������W
	DirectX::SimpleMath::Vector3 m_position;
	// �G�t�F�N�g�̃X�P�[��
	float m_scale;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	// �e�N�X�`���n���h�� 
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	//	�v���~�e�B�u�o�b�` 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	// �t���[���̒��_���
	DirectX::DX11::VertexPositionTexture m_vertices[4];
	// �G�t�F�N�g�̎��
	EffectType m_type;
	// ���A�G�t�F�N�g���Đ����Ă邩
	bool m_isPlaying;
	// �t���[����
	int m_anim;
	// �A�j���[�V�����X�s�[�h
	float m_animSpeed;
	// �A�j���[�V�����̌o�ߎ���
	float m_animTime;
	// �t���[���̍s���Ɨ�
	int m_frameRows;//�s
	int m_frameCols;//��
	// �G�t�F�N�g�ɂ���č�����ς���
	float m_offSetY;
};
#endif // EFFECT_DEFINED