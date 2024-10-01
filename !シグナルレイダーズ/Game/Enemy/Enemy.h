/*
	@file	Enemy.h
	@brief	�G�N���X
	�쐬�ҁF���܂�
*/
#pragma once
#include "Game/KumachiLib/AudioManager.h"
#include "Game/Interface/IEnemy.h"
//�O���錾
class CommonResources;
class PlayScene;
class Player;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class EnemyBullets;
class Enemies;
class Enemy : public IEnemy
{
private:
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// �f�o�b�O�p
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<DirectX::Model> m_model;          // �e�p�̃��f��
	std::unique_ptr<EnemyModel> m_enemyModel;         // �G�̃��f��
	std::unique_ptr<EnemyAI> m_enemyAI;               // �G��AI
	std::unique_ptr<EnemyHPBar> m_HPBar;              // �G��HP�o�[
	std::unique_ptr<EnemyBullets> m_enemyBullets;     // �G�̒e

	// �e�֘A
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState_Shadow;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	// �C�����E��
	DirectX::BoundingSphere m_enemyBS;	//�G�̋��E��
	DirectX::BoundingSphere m_enemyBSToPlayerArea;// �G��Player�Ƃ̈��͈͂̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyBulletBS;// �G�̒e�̋��E��
	DirectX::BoundingSphere m_playerBS;// �v���C���[�̋��E��
	DirectX::SimpleMath::Matrix m_matrix;// �}�g���N�X

	int m_currentHP;//�G�̗̑�
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
	bool m_isHit = false;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy = false;// ���̑��̓G�Ƃ̔���
	bool m_isHitToPlayerBullet = false;// �G���v���C���[�̒e�ɓ���������
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C
	// �v���C���[�ɗ^����_���[�W
	const float PLAYER_DAMAGE = 1.0f;
	// �I�[�f�B�I�}�l�[�W���[
	AudioManager* m_audioManager;
public:
	// �R���X�g���N�^
	Enemy(Player* pPlayer);
	~Enemy();

	// �C���^�[�t�F�[�X�̎���
	void Initialize(CommonResources* resources, int hp) override;
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos) override;
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;
	void CheckHitOtherObject(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B) override;

};