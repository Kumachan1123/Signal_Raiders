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
class IEnemy;
class CommonResources;
class DamageEffect
{
public:// �񋓌^
	// �G�t�F�N�g�̎��
	enum class EffectType
	{
		DAMAGE = 0,	// �_���[�W���󂯂���
		INCOMINGENEMY,// �G���U�����悤�Ƃ��Ă��鎞		
	};
public:// �\����
	// �V�F�[�_�[�ɓn���萔�o�b�t�@�̍\����
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;   // ���[���h�s��
		DirectX::SimpleMath::Matrix matView;    // �r���[�s��
		DirectX::SimpleMath::Matrix matProj;    // �v���W�F�N�V�����s��
		DirectX::SimpleMath::Vector4 colors;    // �J���[
		DirectX::SimpleMath::Vector4 time;      // ����
		DirectX::SimpleMath::Vector4 uv;		// uv���W
		DirectX::SimpleMath::Vector4 alpha;		// �A���t�@�l
	};
public:// �A�N�Z�T
	// �v���C���[�̃|�C���^�擾
	Player* GetPlayer()const { return m_pPlayer; }
	// �v���C���[�̃|�C���^�ݒ�
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
	// �G�̃|�C���^�擾
	IEnemy* GetEnemy()const { return m_pEnemy; }
	// �G�̃|�C���^�ݒ�
	void SetEnemy(IEnemy* pEnemy) { m_pEnemy = pEnemy; }
	// �G�t�F�N�g�Đ��t���O�擾
	bool GetPlayEffect()const { return m_playEffect; }
	// �U�����Ă����G�̌����擾
	DirectX::SimpleMath::Vector3 GetEnemyDirection()const { return m_enemyDirection; }
	// �G�t�F�N�g�^�C�v�ݒ�
	void SetEffectType(EffectType type) { m_effectType = type; }

public:	// public�֐�
	// �R���X�g���N�^
	DamageEffect(CommonResources* resources);
	// �f�X�g���N�^
	~DamageEffect();
	// ������
	void Initialize();
	//// �e�N�X�`�����\�[�X�ǂݍ��݊֐�
	//void LoadTexture(const wchar_t* path);
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �G�t�F�N�g�̔j������
	bool Destroy()const;
private:// private�֐�
	// �V�F�[�_�[�쐬
	void MakeShader();
	// �p�x�v�Z
	float CalculateAngle()const;
	// �p�x����UV���W�����߂�
	DirectX::SimpleMath::Vector4 GetUVFromAngle(float angle)const;
public:// public�萔
	// ���_���
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
private:// private�萔
	// �|���S����X�T�C�Y
	const float SIZE_X = 0.0625;
	// �|���S����Y�T�C�Y
	const float SIZE_Y = 0.11;
	// �g�嗦
	const float SCALE = 10.0f;
	// UV���W:��
	const float UV_X = 0.1f;
	// UV���W:��
	const float UV_Y = 0.1f;
	// UV���W:�E
	const float UV_W = 0.9f;
	// UV���W:��
	const float UV_H = 0.9f;
	// UV���W:����
	const float UV_C = 0.5f;
	// �Đ�����
	const float PLAY_TIME = 1.75f;
	// �j������
	const float DESTROY_TIME = 2.0f;
private:// private�ϐ�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �v���C���[�̃|�C���^�[
	Player* m_pPlayer;
	// �G�̃|�C���^�[
	IEnemy* m_pEnemy;
	// �`��N���X
	DrawPolygon* m_pDrawPolygon;
	// �V�F�[�_�[�쐬�N���X
	CreateShader* m_pCreateShader;
	// �V�F�[�_�[�̍\����
	ConstBuffer m_constBuffer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_pStates;
	// �e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextures;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
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
	// �U�����Ă���G�̍��W
	DirectX::SimpleMath::Vector3 m_enemyPosition;
	// �v���C���[�̌���
	DirectX::SimpleMath::Vector3 m_playerDirection;
	// �G�t�F�N�g�Đ��t���O
	bool m_playEffect;
	// �G�t�F�N�g�^�C�v
	EffectType m_effectType;
};