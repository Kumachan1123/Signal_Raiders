/*
*	@file DamageEffect.h
*	@brief �U�����󂯂����̉��o�N���X
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
// ����w�b�_�[�t�@�C��
#include "Game/Player/Player.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"

// �N���X�̑O���錾
class Player;
class CommonResources;

class DamageEffect
{
public:
	// �\����
	struct ConstBuffer// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;                             // ����
		DirectX::SimpleMath::Vector4 uv;// uv���W
		DirectX::SimpleMath::Vector4 alpha;// �A���t�@�l
	}m_constBuffer;

public:

	// public�֐�
	DamageEffect(CommonResources* resources);// �R���X�g���N�^
	~DamageEffect();// �f�X�g���N�^

	void Initialize(Player* pPlayer);// ������
	void LoadTexture(const wchar_t* path);// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
	void Update(float elapsedTime);// �X�V
	void Render();// �`��

	// �A�N�Z�T
	bool GetPlayEffect()const { return m_playEffect; }// �G�t�F�N�g�Đ��t���O�擾
	DirectX::SimpleMath::Vector3 GetEnemyDirection()const { return m_enemyDirection; }// �U�����Ă����G�̌����擾
	bool Destroy()const// �j������
	{
		if (m_time >= DESTROY_TIME)return true;// �j�����Ԃ��߂�����true
		else return false;// ����ȊO��false
	}
	// public�萔
	// ���_���
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:
	void MakeShader();// �V�F�[�_�[�쐬
	float CalculateAngle()const;// �p�x�v�Z
	DirectX::SimpleMath::Vector4 GetUVFromAngle(float angle)const;// �p�x����UV���W�����߂�
private:
	// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����s��
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
};