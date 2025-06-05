/*
*	@file	Radar.h
*	@brief	���[�_�[�N���X
*/
#pragma once
#ifndef RADAR_DEFINED
#define RADAR_DEFINED
// �W�����C�u����
#include <vector>
#include <cassert>
#include <memory>
// DirectXTK
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
// �O�����C�u����
#include "Game/CommonResources.h"
#include <DeviceResources.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
// ����w�b�_�[�t�@�C��
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"
#include "Game/Player/Player.h"
#include "Game/Enemy/EnemyManager/EnemyManager.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/EnemyAI/EnemyAI.h"
#include "Game/KumachiLib/DrawPolygon/DrawPolygon.h"
#include "Game/KumachiLib/CreateShader/CreateShader.h"

// �O���錾
class CommonResources;
class Player;
class EnemyManager;
class Enemy;
class EnemyAI;

// ���[�_�[�N���X
class Radar
{
public:
	// �\����
	// �V�F�[�_�[�ɓn���萔�o�b�t�@�̍\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;      // ����
	};
private:
	// �񋓌^
	enum class RadarState// ���[�_�[�̏��
	{
		Background,	// �w�i
		Player,		// �v���C���[
		Enemy,		// �G
	};
public:
	// public�����o�֐�
	// �R���X�g���N�^
	Radar(CommonResources* commonResources);
	// �f�X�g���N�^
	~Radar();
	// ������
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
private:
	// private�����o�֐�
	// �w�i��`�悷��
	void DrawBackground();
	// �v���C���[��`�悷��
	void DrawPlayer();
	// �G��`�悷��
	void DrawEnemy();
	// �o�b�t�@���쐬����
	void CreateBuffer();
	// �`��O�ݒ�
	void DrawSetting();
public:
	// �萔
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	//���[�_�[�̍��[
	static const float RADAR_SIZE_L;
	//���[�_�[�̉E�[
	static const float RADAR_SIZE_R;
	//���[�_�[�̏�[
	static const float RADAR_SIZE_T;
	//���[�_�[�̉��[
	static const float RADAR_SIZE_B;
	// �G�̓_�̕�
	static const float ENEMY_SIZE_W;
	// �G�̓_�̍���
	static const float ENEMY_SIZE_H;
	// ���[�_�[�̈ʒuX
	static const float RADAR_POSITION_X;
	// ���[�_�[�̈ʒuY
	static const float RADAR_POSITION_Y;
	// �v���C���[��X�T�C�Y
	static const float PLAYER_SIZE_X;
	// �v���C���[��Y�T�C�Y
	static const float PLAYER_SIZE_Y;
	// �G��X�T�C�Y
	static const float ENEMY_SIZE_X;
	// �G��Y�T�C�Y
	static const float ENEMY_SIZE_Y;
	// ���[�_�[�͈̔�
	static const float RADAR_RANGE;
	// �v���C���[�Ƃ̋���
	static const float DISTANCE;
private:
	// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �v���C���[
	Player* m_pPlayer;
	// �G�}�l�[�W���[
	EnemyManager* m_pEnemyManager;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_pCBuffer;
	// ���[�_�[�͈̔�
	float m_range;
	// �v���C���[�Ƃ̋���
	float m_distance;
	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;
	// �G�̈ʒu
	std::vector<DirectX::SimpleMath::Vector3> m_enemyPos;
	// �G�̏��
	std::vector<bool> m_enemyState;
	// ���[�_�[�\���ʒu
	DirectX::SimpleMath::Vector2 m_radarPos;
	// �v���C���[�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_playerSize;
	// �G�̃T�C�Y
	DirectX::SimpleMath::Vector2 m_enemySize;
	// ����
	float m_time;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �V�F�[�_�[�ɓn���萔�o�b�t�@
	ConstBuffer m_constBuffer;
	// �e�N�X�`���z��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTexture;
};
#endif // RADAR_DEFINED