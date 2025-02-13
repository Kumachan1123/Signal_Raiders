#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
class CommonResources;

class Effect
{
public:
	enum class EffectType
	{
		ENEMY_DEAD = 0,
		ENEMY_HIT,
		NONE
	};
private:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 count;     // �J�E���g
		DirectX::SimpleMath::Vector4 height;    // ����
		DirectX::SimpleMath::Vector4 width;     // ��

	}m_constBuffer;

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	DirectX::SimpleMath::Vector3 m_position;// �G�t�F�N�g���Đ�������W
	float m_scale;// �G�t�F�N�g�̃X�P�[��

	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//	���̓��C�A�E�g 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

	//	���ʃX�e�[�g�I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�G�t�F�N�g 
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;
	//	�e�N�X�`���n���h�� 
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;


	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	//	�v���~�e�B�u�o�b�` 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;

	// �t���[���̒��_���
	DirectX::DX11::VertexPositionTexture m_vertices[4];

	// ParticleType
	EffectType m_type;
	// ���A�G�t�F�N�g���Đ����Ă邩
	bool m_isPlaying;
	// �t���[����
	int m_anim;
	// �A�j���[�V�����X�s�[�h
	float m_animSpeed;
	// �A�j���[�V�����̌o�ߎ���
	float m_elapsedTime;

	// �t���[���̍s���Ɨ�
	int m_frameRows;
	int m_frameCols;

	// Particle�ɂ���č�����ς���
	float m_offSetY;

	// �|���S���̒��_���W
	float m_vertexMinX;//��
	float m_vertexMaxX;//�E
	float m_vertexMinY;//��
	float m_vertexMaxY;//��

public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// ���̓��C�A�E�g
public:
	// �R���X�g���N�^
	Effect(CommonResources* resources,
		EffectType type,
		DirectX::SimpleMath::Vector3 playPos,
		float scale,
		DirectX::SimpleMath::Matrix world);
	// �f�X�g���N�^
	~Effect();
	void Update(float elapsedTime);// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �`��
	void Finalize();// �I������


public:
	bool IsPlaying() const { return m_isPlaying; }// �Đ�����
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// ���W�擾
private:
	void LoadTexture(const wchar_t* path);// �摜��ǂݍ���

};