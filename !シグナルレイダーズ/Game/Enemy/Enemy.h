/*
	@file	Enemy.h
	@brief	�G�N���X
	�쐬�ҁF���܂�
*/
#pragma once
//�O���錾
class CommonResources;
class PlayScene;
class EnemyAI;
class EnemyHPBar;
class EnemyBullet;
class EnemyModel;
class Enemy
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//�f�o�b�O�p
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
	std::unique_ptr<EnemyModel>		m_enemyModel;
	std::unique_ptr<EnemyAI>		m_enemyAI;
	std::unique_ptr<EnemyHPBar>		m_HPBar;
	std::vector<std::unique_ptr<EnemyBullet>> m_bullets; // �e�̃��X�g
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>m_depthStencilState;
	// �֊s�i���j
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;	// �u�����h�X�e�[�g
	// �ǉ��F�A�E�g���C���p�̃V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_outlinePS;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Vector3 m_accele;		// �����x
	DirectX::SimpleMath::Vector3 m_nowScale;		// ���݃X�P�[��
	DirectX::SimpleMath::Vector3 m_startScale;		// �����X�P�[��
	DirectX::SimpleMath::Vector3 m_endScale;		// �ŏI�X�P�[��
	DirectX::SimpleMath::Quaternion m_rotation; // �N�H�[�^�j�I�� (�ǉ�)
	// �C�����E��
	DirectX::BoundingSphere m_enemyBoundingSphere;	//�G�̋��E��
	DirectX::BoundingSphere m_enemyBoundingSphereToPlayer;// �G��Player�Ƃ̈��͈͂Ƃ̓����蔻��Ɏg��
	DirectX::BoundingSphere m_enemyWBoundingSphere;
	DirectX::BoundingSphere m_enemyBulletBS;
	DirectX::BoundingSphere m_playerBS;
	DirectX::SimpleMath::Matrix m_matrix;				// �}�g���N�X

	int m_currentHP;//�G�̗̑�
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True
	bool m_isHit = false;// �v���C���[�Ƃ̔���
	bool m_isHitToOtherEnemy = false;// ���̑��̓G�Ƃ̔���
	bool m_isBullethit = false;// �G�̒e���v���C���[�ɓ���������
	float m_attackCooldown;  // �U���̃N�[���_�E���^�C��

	// Player
	const float PLAYER_DAMAGE = 0.1f;
public:
	//	getter
	DirectX::BoundingSphere& GetBoundingSphere() { return m_enemyBoundingSphere; }
	DirectX::BoundingSphere& GetBulletBoundingSphere() { return m_enemyBulletBS; }
	DirectX::BoundingSphere& GetPlayerBoundingSphere() { return m_playerBS; }
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_velocity; }
	DirectX::SimpleMath::Vector3 GetAccele() const { return m_accele; }
	DirectX::SimpleMath::Vector3 GetNowScale() const { return m_nowScale; }
	DirectX::SimpleMath::Vector3 GetRotate() const { return m_rotate; }
	DirectX::SimpleMath::Vector3 GetStartScale() const { return m_startScale; }
	DirectX::SimpleMath::Vector3 GetEndScale() const { return m_endScale; }
	int GetHP() const { return m_currentHP; }
	bool GetEnemyIsDead() const { return m_isDead; }
	bool GetHitToPlayer()const { return m_isHit; }
	bool GetHitToOtherEnemy() const { return m_isHitToOtherEnemy; }
	bool GetBulletHitToPlayer() const { return m_isBullethit; }
	// setter
	void SetBoundingSphereCenter(DirectX::SimpleMath::Vector3& cen) { m_enemyBoundingSphere.Center = cen; }
	void SetPosition(DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	void SetEnemyHP(int hp) { m_currentHP = hp; }
	void SetHitToPlayer(bool isHitToPlayer) { m_isHit = isHitToPlayer; }
	void SetHitToOtherEnemy(bool isHitToOtherEnemy) { m_isHitToOtherEnemy = isHitToOtherEnemy; }
	void SetBulletBoundingSphere(DirectX::BoundingSphere& bs) { m_enemyBulletBS = bs; }
	void SetPlayerBoundingSphere(DirectX::BoundingSphere& playerBS) { m_playerBS = playerBS; }
	void SetPlayerHP(float& HP) const { HP -= PLAYER_DAMAGE; }
	void SetBulletHitToPlayer(bool hit) { m_isBullethit = hit; }
public:
	// �����X�e�[�^�X��ݒ�
	Enemy();
	~Enemy();
	void Initialize(CommonResources* resources, int hp);
	void Update(float elapsedTime, DirectX::SimpleMath::Vector3 playerPos);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void CheckHitOtherEnemy(DirectX::BoundingSphere& A, DirectX::BoundingSphere& B);
public:
	void AddBullet(std::unique_ptr<EnemyBullet> bullet);
	void UpdateBullets(float elapsedTime);

};