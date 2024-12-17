/*
	@file	Radar.h
	@brief	���[�_�[�N���X
*/
#pragma once
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include <memory>
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
class CommonResources;
class Player;
class Enemies;
class Enemy;
class EnemyAI;
class Radar
{
public:
	// �f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;      // ����
	}m_constBuffer;
private:
	enum class RadarState
	{
		Background,
		Player,
		Enemy,
	};
	// �ϐ�
	CommonResources* m_commonResources;
	Player* m_pPlayer;
	Enemies* m_pEnemies;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cBuffer;


	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	float m_range = 300.0f;// ���[�_�[�͈̔�
	// �v���C���[�Ƃ̋���
	float m_distance = 125.0f;
	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;
	// �G�̈ʒu
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// �G�̏��
	std::vector<bool> m_enemyState;

	// ���[�_�[�\���ʒu�ƃv���C���[�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_radarPos;
	DirectX::SimpleMath::Vector2 m_playerSize;
	// �G�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_enemySize;
	// ����
	float m_time;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_playerTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_enemyTexture;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_backTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_playerTextures;
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_enemyTextures;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	// �R���X�g���N�^
	Radar(CommonResources* commonResources);
	// �f�X�g���N�^
	~Radar();
	// ������
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();

	// �e�N�X�`���̓ǂݍ���
	void LoadTexture(const wchar_t* path, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& textures);

	void DrawBackground();// �w�i��`�悷��
	void DrawPlayer();// �v���C���[��`�悷��
	void DrawEnemy();// �G��`�悷��

	void CreateBuffer(ID3D11DeviceContext1* context);// �o�b�t�@���쐬����

};