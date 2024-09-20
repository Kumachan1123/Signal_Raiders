/*
	@file	Radar.h
	@brief	���[�_�[�N���X
	�쐬�ҁF���܂�
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

#include <memory>
#include "Game/Player/Player.h"
#include "Game/Enemy/Enemies/Enemies.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
class CommonResources;
class Player;
class Enemies;
class Enemy;
class EnemyAI;
class Radar
{
	// �ϐ�
	CommonResources* m_commonResources;
	Player* m_pPlayer;
	Enemies* m_pEnemies;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	float m_range = 400.0f;// ���[�_�[�͈̔�
	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;
	// �G�̈ʒu
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// �G�̏��
	std::vector<bool> m_enemyState;

	// ���[�_�[�\���ʒu�ƃT�C�Y
	DirectX::SimpleMath::Vector2 m_radarPos;
	DirectX::SimpleMath::Vector2 m_radarSize;
public:
	// �R���X�g���N�^
	Radar(CommonResources* commonResources);
	// �f�X�g���N�^
	~Radar();
	// ������
	void Initialize(Player* pPlayer, Enemies* pEnemies);
	// �X�V
	void Update();
	// �`��
	void Render();



};