/*
	@file	EnemyHPBar.h
	@brief	�GHPBar�N���X
*/
#pragma once
#include <SimpleMath.h>
#include "Game/Enemy/Enemy.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include <cassert>
#include <random>
#include <memory>
#include <Libraries/Microsoft/DebugDraw.h>
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
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;//	���[���h�s��
		DirectX::SimpleMath::Matrix		matView;	//	�r���[�s��
		DirectX::SimpleMath::Matrix		matProj;	//	�v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4	colors;
		DirectX::SimpleMath::Vector4	hp;
	}m_constBuffer;
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

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	// �l�p�`�Ŏg�p���� ���_���

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DirectX::VertexPositionTexture	m_hpbarVert[4];
	DirectX::VertexPositionTexture	m_hpbarBackVert[4];

	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_gaugeTexture;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Vector3 m_accele;		// �����x
	DirectX::SimpleMath::Vector3 m_scale;		// �X�P�[��

	// �C�����E��
	DirectX::BoundingSphere m_enemyBoundingSphere;
	DirectX::BoundingSphere m_enemyWBoundingSphere;

	float m_time;
	int m_maxHP;// �ő�HP(�������ꂽ�u�Ԃ̓G��HP)
	int m_currentHP;// ���݂̓G��HP
	float m_displayedHP = 100.0f; // �\�������HP��ǐՂ���V���������o�[
	float m_lerpSpeed = 2.0f; // ��ԑ��x
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
public:
	// ���_���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }

	int GetHP() const { return m_currentHP; }
	bool GetIsDead() const { return m_isDead; }
	// setter
	void SetBoundingSphereCenter(DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int& hp) { m_maxHP = hp; }
	void SetScale(DirectX::SimpleMath::Vector3 scale) { m_scale = scale; }
public:
	// �����X�e�[�^�X��ݒ�
	EnemyHPBar();
	~EnemyHPBar();

	void Initialize(CommonResources* resources);
	void Update(float elapsedTime, int currentHP);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot);
private:
	void LoadTexture(const wchar_t* path);
	void CreateBuffer();
	void CreateShader();
};


