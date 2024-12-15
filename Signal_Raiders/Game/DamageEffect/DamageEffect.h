//--------------------------------------------------------------------------------------
// File: DamageEffect.h
//
// �U�����󂯂����̉��o�N���X
//
//-------------------------------------------------------------------------------------

#pragma once
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Game/Player/Player.h"
class Player;
class CommonResources;
class DamageEffect
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		float time = 0.0f;                             // ����
		DirectX::SimpleMath::Vector2 uv;// uv���W
		float alpha = 0.0f;// �A���t�@�l
	}m_constBuffer;
private:
	//	�ϐ�
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	//	���[���h�r���[�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	// ����
	float m_time;
	// �U�����Ă����G�̂������
	DirectX::SimpleMath::Vector3 m_enemyDirection;
	// �v���C���[�̌���
	DirectX::SimpleMath::Vector3 m_playerDirection;

	// �Œ�l
	// �|���S����X�T�C�Y
	const float SIZE_X = 0.0625;
	// �|���S����Y�T�C�Y
	const float SIZE_Y = 0.11;
	// �g�嗦
	const float SCALE = 10.0f;
	// UV���W
	const float UV_X = 0.1f;// ��
	const float UV_Y = 0.1f;// ��
	const float UV_W = 0.9f;// �E
	const float UV_H = 0.9f;// ��
	const float UV_C = 0.5f;// ����
	// �Đ�����
	const float PLAY_TIME = 1.75f;
	// �j������
	const float DESTROY_TIME = 2.0f;
	// �G�t�F�N�g�Đ��t���O
	bool m_playEffect;
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	DamageEffect(CommonResources* resources);
	~DamageEffect();

	void LoadTexture(const wchar_t* path);

	void Create(DX::DeviceResources* pDR);

	void Initialize(Player* pPlayer);
	void Update(float elapsedTime);

	void Render();

	// getter
	bool GetPlayEffect()const { return m_playEffect; }
	DirectX::SimpleMath::Vector3 GetEnemyDirection()const { return m_enemyDirection; }
	bool Destroy()const
	{
		if (m_time >= DESTROY_TIME)
		{

			return true;

		}

		return false;
	}
	// setter
	void SetEnemyDirection(DirectX::SimpleMath::Vector3 direction) { m_enemyDirection = direction; }
private:

	void CreateShader();
};