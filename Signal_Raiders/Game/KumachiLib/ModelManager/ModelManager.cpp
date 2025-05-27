/*
*	@file ModelManager.cpp
*	@brief ���f���}�l�[�W���[�N���X
*/
#include "pch.h"
#include "ModelManager.h"

/*
*	@brief �R���X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃R���X�g���N�^
*/
ModelManager::ModelManager()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_device(nullptr) // �f�o�C�X
	, m_modelMap() // ���f���̃}�b�v
	, m_effectFactory(nullptr) // �G�t�F�N�g�t�@�N�g���[
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃f�X�g���N�^
*	@return �Ȃ�
*/
ModelManager::~ModelManager()
{
	m_modelMap.clear(); // ���f���̃}�b�v���N���A
	m_effectFactory.reset(); // �G�t�F�N�g�t�@�N�g���[�����Z�b�g
	m_device = nullptr; // �f�o�C�X��nullptr�ɐݒ�
	m_pCommonResources = nullptr; // ���ʃ��\�[�X��nullptr�ɐݒ�
}
/*
*	@brief ���f���̏�����
*	@details �e�탂�f���̃��[�h�ƃG�t�F�N�g�̐ݒ���s��
*	@return �Ȃ�
*/
void ModelManager::Initialize()
{
	m_device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);// �G�t�F�N�g�t�@�N�g���[�̍쐬
	CreateBulletModels(); // �e���f���̍쐬
	CreateEnemyModels(); // �G���f���̍쐬
	CreateVerticalAttackerModels(); // �����U���G���f���̍쐬
	CreateBossModels(); // �{�X���f���̍쐬
	CreateLastBossModels(); // ���X�{�X���f���̍쐬
}
/*
*	@brief �e���f���̍쐬
*	@details �v���C���[�ƓG�̒e���f�����쐬���A�G�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBulletModels()
{
	m_effectFactory->ReleaseCache();// �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models");// ���f���̃f�B���N�g�����w��
	m_modelMap["PlayerBullet"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Bullet.cmo", *m_effectFactory);// �e���f����ǂݍ���
	m_modelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// �G�t�F�N�g�̍X�V
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});// ���e���f���̃G�t�F�N�g��ݒ肷��
	m_effectFactory->ReleaseCache();// �L���b�V�����������
	m_modelMap["EnemyBullet"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Bullet.cmo", *m_effectFactory);// �e���f����ǂݍ���
	m_modelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)	// ���f���̃G�t�F�N�g��ݒ肷��
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1));
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);
		});// �G�e���f���̃G�t�F�N�g��ݒ肷��
}
/*
*	@brief �G���f���̍쐬
*	@details �G�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateEnemyModels()
{
	m_effectFactory->ReleaseCache(); // �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models/Enemy");// ���f���̃f�B���N�g�����w��
	m_modelMap["EnemyHead"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Head.cmo", *m_effectFactory);// �G�̓����f����ǂݍ���
	m_modelMap["EnemyAntenna"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *m_effectFactory);// �G�̃A���e�i���f����ǂݍ���
	m_modelMap["EnemyHand"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *m_effectFactory);// �G�̎胂�f����ǂݍ���
	m_modelMap["EnemyShadow"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy.cmo", *m_effectFactory);// �G�̉e�p���f����ǂݍ���
	m_modelMap["EnemyDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *m_effectFactory);// �G�̃_���[�W�烂�f����ǂݍ���
	m_modelMap["EnemyAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *m_effectFactory);// �G�̍U���烂�f����ǂݍ���
	m_modelMap["EnemyIdling"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *m_effectFactory);// �G�̕��i�̊烂�f����ǂݍ���
}
/*
*	@brief �����U���G���f���̍쐬
*	@details �����U���G�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateVerticalAttackerModels()
{
	m_effectFactory->ReleaseCache(); // �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models/VerticalAttacker");// ���f���̃f�B���N�g�����w��
	m_modelMap["VerticalAttacker"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *m_effectFactory);// �����U���G�̃��f����ǂݍ���
}
/*
*	@brief �{�X���f���̍쐬
*	@details �{�X�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBossModels()
{
	m_effectFactory->ReleaseCache(); // �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g�����w��
	m_modelMap["BossBody"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss.cmo", *m_effectFactory);// �{�X�̓��̃��f����ǂݍ���
	m_modelMap["BossFaceDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Damage.cmo", *m_effectFactory);// �{�X�̃_���[�W�烂�f����ǂݍ���
	m_modelMap["BossFaceAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *m_effectFactory);// �{�X�̍U���烂�f����ǂݍ���
	m_modelMap["BossFaceAngry"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Angry.cmo", *m_effectFactory);// �{���Ԃ̊�
}
/*
*	@brief ���X�{�X���f���̍쐬
*	@details ���X�{�X�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateLastBossModels()
{
	m_effectFactory->ReleaseCache(); // �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models/LastBoss");// ���f���̃f�B���N�g�����w��
	m_modelMap["LastBossBody"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_Body.cmo", *m_effectFactory);// ���X�{�X�̓��̃��f����ǂݍ���
	m_modelMap["LastBossFaceDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_Face.cmo", *m_effectFactory);// ���X�{�X�̃_���[�W�烂�f����ǂݍ���
	m_modelMap["LastBossFaceAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_AttackFace.cmo", *m_effectFactory);// ���X�{�X�̍U���烂�f����ǂݍ���
	m_modelMap["LastBossFaceAngry"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_AngryFace.cmo", *m_effectFactory);// ���X�{�X�̓{��烂�f����ǂݍ���
}
/*
*	@brief �o���A���f���̍쐬
*	@details �o���A�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBarrierModels()
{
	m_effectFactory->ReleaseCache(); // �L���b�V�����������
	m_effectFactory->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g�����w��
	m_modelMap["Barrier"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Barrier.cmo", *m_effectFactory);// �{�X�̃o���A���f����ǂݍ���
}

DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	auto it = m_modelMap.find(key);
	if (it != m_modelMap.end())	return it->second.get();
	return nullptr;
}
