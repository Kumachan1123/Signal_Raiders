/*
	@file	EnemyHPBar.h
	@brief	�GHPBar�N���X
*/
#pragma once
//�O���錾
class CommonResources;
class PlayScene;
class Enemy;



class EnemyHPBar
{
private:
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	/*
		�r���{�[�h�̕`��Ɋ֘A����I�u�W�F�N�g
	*/
	// FPS�J����

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	// �l�p�`�Ŏg�p���� ���_���

	DirectX::VertexPositionTexture	m_hpbarVert[4];
	DirectX::VertexPositionTexture	m_hpbarBackVert[4];

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_gaugeTexture;
	// �G�̏��
	DirectX::SimpleMath::Vector3 m_position;		// ���W
	DirectX::SimpleMath::Vector3 m_velocity;		// ���x
	DirectX::SimpleMath::Vector3 m_rotate;		// ��]
	DirectX::SimpleMath::Vector3 m_accele;		// �����x
	DirectX::SimpleMath::Vector3 m_scale;		// �X�P�[��

	// �C�����E��
	DirectX::BoundingSphere m_enemyBoundingSphere;
	DirectX::BoundingSphere m_enemyWBoundingSphere;


	int m_maxHP;// �ő�HP(�������ꂽ�u�Ԃ̓G��HP)
	int m_currentHP;// ���݂̓G��HP
	float m_displayedHP = 100.0f; // �\�������HP��ǐՂ���V���������o�[
	float m_lerpSpeed = 2.0f; // ��ԑ��x
	bool m_isDead = false;//�G��HP��0�ɂȂ�����True


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

};

