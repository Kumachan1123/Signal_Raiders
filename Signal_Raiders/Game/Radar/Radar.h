/*
*	@file	Radar.h
*	@brief	���[�_�[�N���X
*/
#pragma once
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
class Radar
{
public:
	// ���[�_�[�̑傫���̒�`


	// �V�F�[�_�[�ɓn���萔�o�b�t�@�̍\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;      // ����
	}m_constBuffer;
private:// �񋓌^
	enum class RadarState// ���[�_�[�̏��
	{
		Background,	// �w�i
		Player,		// �v���C���[
		Enemy,		// �G
	};
public:// public�����o�֐�
	Radar(CommonResources* commonResources);// �R���X�g���N�^
	~Radar();// �f�X�g���N�^
	void Initialize(Player* pPlayer, EnemyManager* pEnemyManager);// ������
	void Update(float elapsedTime);// �X�V
	void Render();// �`��
private:// private�����o�֐�
	void LoadTexture(const wchar_t* path);// �e�N�X�`���̓ǂݍ���
	void DrawBackground();// �w�i��`�悷��
	void DrawPlayer();// �v���C���[��`�悷��
	void DrawEnemy();// �G��`�悷��
	void CreateBuffer();// �o�b�t�@���쐬����
	void DrawSetting();// �`��O�ݒ�
public:// �萔
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;// �C���v�b�g���C�A�E�g
	static const float RADAR_SIZE_L;//���[�_�[�̍��[
	static const float RADAR_SIZE_R;//���[�_�[�̉E�[
	static const float RADAR_SIZE_T;//���[�_�[�̏�[
	static const float RADAR_SIZE_B;//���[�_�[�̉��[
	static const float ENEMY_SIZE_W;// �G�̓_�̕�
	static const float ENEMY_SIZE_H;// �G�̓_�̍���
	static const float RADAR_POSITION_X;// ���[�_�[�̈ʒuX
	static const float RADAR_POSITION_Y;// ���[�_�[�̈ʒuY
	static const float PLAYER_SIZE_X;// �v���C���[��X�T�C�Y
	static const float PLAYER_SIZE_Y;// �v���C���[��Y�T�C�Y
	static const float ENEMY_SIZE_X;// �G��X�T�C�Y
	static const float ENEMY_SIZE_Y;// �G��Y�T�C�Y
	static const float RADAR_RANGE;// ���[�_�[�͈̔�
	static const float DISTANCE;// �v���C���[�Ƃ̋���
private:// private�ϐ�
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
	// �e�N�X�`��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// �V�F�[�_�[�̍\����
	DrawPolygon::Shaders m_shaders;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;

};