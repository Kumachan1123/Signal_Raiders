/*
*	@file	PlayerBullet.h
*	@brief	�v���C���[�̒e�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
// DirectX���C�u����
#include <SimpleMath.h>
// �O�����C�u����
#include <DeviceResources.h>
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <Libraries/Microsoft/DebugDraw.h>
#include "Libraries/Microsoft/ReadData.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Interface/IBullet.h"
//�O���錾
class CommonResources;

class PlayerBullet : public IBullet
{
public:
	// �A�N�Z�T
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	void SetAdditionalDamage(int damage) { m_additionalDamage = damage; }
	int GetAdditionalDamage()const { return m_additionalDamage; }
	// public�֐�
	PlayerBullet();
	~PlayerBullet()override;
	void Initialize(CommonResources* resources)override;// ������
	void Update(float elapsedTime)override;// �X�V
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �`��
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;// �e�̕`��
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// �����蔻��̕`��
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir);// �e�𐶐�
	bool IsExpired() const { return m_time >= BulletParameters::PLAYER_BULLET_LIFETIME; }// �e����������Ă���̌o�ߎ��Ԃ������𒴂������ǂ����𔻒肷��
	int Damage()const { return BulletParameters::DAMAGE + m_additionalDamage; }// �G�Ƀ_���[�W��^����


private:
	// private�ϐ�
	// �O�՗p�̗�����ۑ�����z��
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// �u�e�v���E�{�b�N�X
	DirectX::BoundingSphere m_boundingSphere;
	// ���f���̉e
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �e�̎��]
	float m_angle;
	// �e�̋O�Ճ|�C���^�[
	std::unique_ptr<Particle> m_bulletTrail;
	// �ǉ��_���[�W
	int m_additionalDamage;
	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_trailTexture;
	// �J����
	DirectX::SimpleMath::Vector3 m_cameraEye;// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_cameraTarget;	// �J�����̌���
	DirectX::SimpleMath::Vector3 m_cameraUp;	// �J�����̏����
	// �e�̃p�����[�^
	DirectX::SimpleMath::Vector3 m_position;		// �e�̍��W
	DirectX::SimpleMath::Vector3 m_velocity;		// �e�̑���
	DirectX::SimpleMath::Vector3 m_direction;		// �e����ԕ���
	float m_time;									// ��������
	// �e���f��
	std::unique_ptr<DirectX::Model> m_model;
};