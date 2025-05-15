/*
	@file	EnemyHPBar.h
	@brief	�GHPBar�N���X
*/
#pragma once
// �W�����C�u����
#include <SimpleMath.h>
#include <cassert>
#include <random>
#include <memory>
// �O�����C�u����
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/Enemy/Enemy.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
//�O���錾
class CommonResources;
class PlayScene;
class Enemy;
class EnemyHPBar
{
private:
	// �\����
	struct ConstBuffer	// �萔�o�b�t�@�̍\����
	{
		DirectX::SimpleMath::Matrix		matWorld;//	���[���h�s��
		DirectX::SimpleMath::Matrix		matView;	//	�r���[�s��
		DirectX::SimpleMath::Matrix		matProj;	//	�v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4	colors;//		�F
		DirectX::SimpleMath::Vector4	hp;//		HP
	};

public:
	// �A�N�Z�T
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }// �G�̋��E�����擾
	void SetBoundingSphereCenter(DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }// �G�̋��E���̒��S��ݒ�
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }// �ʒu���擾
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }// �ʒu��ݒ�
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }// ���x���擾
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }// �����x���擾
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }// �X�P�[�����擾
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }// �X�P�[����ݒ�
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; } // ��]���擾
	int GetCurrentHP() const { return m_currentHP; } // ���݂�HP���擾
	void SetCurrentHP(int& hp) { m_currentHP = hp; } // ���݂�HP��ݒ�
	void SetEnemyMaxHP(int& hp) { m_maxHP = hp; } // �G�̍ő�HP��ݒ�
	bool GetIsDead() const { return m_isDead; } // �G������ł��邩�ǂ������擾
public:
	// public�֐�
	EnemyHPBar();// �R���X�g���N�^
	~EnemyHPBar();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void Update(float elapsedTime);// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot);// �`��
private:
	// private�֐�
	void LoadTexture(const wchar_t* path);// �e�N�X�`���ǂݍ���
	void CreateBuffer();// �o�b�t�@�쐬
	void CreateShaders();// �V�F�[�_�[�쐬
public:
	// public�萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;	// ���_���C�A�E�g
	static const int VERTEX_COUNT = 4;	// ���_��
	const float HPBAR_X_MIN = -1.5f;	// HP�o�[��X���Wmin
	const float HPBAR_X_MAX = 1.5f;	// HP�o�[��X���Wmax
	const float HPBAR_Y_MAX = 3.25f;	// HP�o�[��Y���Wmin
	const float HPBAR_Y_MIN = 3.0f;	// HP�o�[��Y���Wmax
	const float HPBARBACK_X_MIN = -1.53f;	// HP�o�[�̔w�i��X���Wmin
	const float HPBARBACK_X_MAX = 1.53f;	// HP�o�[�̔w�i��X���Wmax
	const float HPBARBACK_Y_MAX = 3.28f;	// HP�o�[�̔w�i��Y���Wmax
	const float HPBARBACK_Y_MIN = 2.98f;	// HP�o�[�̔w�i��Y���Wmin
	const float BAR_LEFT = -1.50f;	// HP�o�[�̕�
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
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
	// �R���X�^���g�o�b�t�@
	ConstBuffer m_constBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// ���_���
	DirectX::VertexPositionTexture	m_hpbarVert[VERTEX_COUNT];// HP�o�[�̒��_���
	DirectX::VertexPositionTexture	m_hpbarBackVert[VERTEX_COUNT];// HP�o�[�̔w�i�̒��_���
	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_gaugeTexture;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Vector3 m_accele;		// �����x
	DirectX::SimpleMath::Vector3 m_scale;		// �X�P�[��
	// �G�̋��E��
	DirectX::BoundingSphere m_enemyBoundingSphere;
	// �G�̃��[���h���E��
	DirectX::BoundingSphere m_enemyWBoundingSphere;
	// �o�ߎ���
	float m_time;
	// �ő�HP(�������ꂽ�u�Ԃ̓G��HP)
	int m_maxHP;
	// ���݂̓G��HP
	int m_currentHP;
	// �\�������HP
	float m_displayedHP;
	// ��ԑ��x
	float m_lerpSpeed;
	//�G��HP��0�ɂȂ�����True
	bool m_isDead;
};


