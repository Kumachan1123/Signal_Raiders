/*
*	@file ModelManager.cpp
*	@brief ���f���}�l�[�W���[�N���X
*/
#include <pch.h>
#include "ModelManager.h"

/*
*	@brief �R���X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ModelManager::ModelManager()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_pDevice(nullptr) // �f�o�C�X
	, m_pModelMap() // ���f���̃}�b�v
	, m_pEffectFactory(nullptr) // �G�t�F�N�g�t�@�N�g���[
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ModelManager::~ModelManager()
{
	m_pModelMap.clear(); // ���f���̃}�b�v���N���A
	m_pEffectFactory.reset(); // �G�t�F�N�g�t�@�N�g���[�����Z�b�g
	m_pDevice = nullptr; // �f�o�C�X��nullptr�ɐݒ�
	m_pCommonResources = nullptr; // ���ʃ��\�[�X��nullptr�ɐݒ�
}
/*
*	@brief ���f���̏�����
*	@details �e�탂�f���̃��[�h�ƃG�t�F�N�g�̐ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::Initialize()
{
	m_pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// �f�o�C�X���擾
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);// �G�t�F�N�g�t�@�N�g���[�̍쐬
	m_pEffectFactory->SetSharing(false);// �G�t�F�N�g�̋��L�𖳌��ɂ���
	CreateBulletModels(); // �e���f���̍쐬
	CreateEnemyModels(); // �G���f���̍쐬
	CreateVerticalAttackerModels(); // �����U���G���f���̍쐬
	CreateBossModels(); // �{�X���f���̍쐬
	CreateLastBossModels(); // ���X�{�X���f���̍쐬
	CreateBarrierModels(); // �o���A���f���̍쐬
	CreateStageModels(); // �X�e�[�W���f���̍쐬
}

/*
*	@brief �e���f���̍쐬
*	@details �v���C���[�ƓG�̒e���f�����쐬���A�G�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBulletModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models");// ���f���̃f�B���N�g�����w��
	m_pModelMap["PlayerBullet"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Bullet.cmo", *m_pEffectFactory);// �e���f����ǂݍ���
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// �G�t�F�N�g�̍X�V
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});// ���e���f���̃G�t�F�N�g��ݒ肷��
	m_pModelMap["EnemyBullet"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Bullet.cmo", *m_pEffectFactory);// �e���f����ǂݍ���
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)	// ���f���̃G�t�F�N�g��ݒ肷��
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
	m_pEffectFactory->SetDirectory(L"Resources/Models/Enemy");// ���f���̃f�B���N�g�����w��
	m_pModelMap["EnemyHead"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Head.cmo", *m_pEffectFactory);// �G�̓����f����ǂݍ���
	m_pModelMap["EnemyAntenna"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *m_pEffectFactory);// �G�̃A���e�i���f����ǂݍ���
	m_pModelMap["EnemyHand"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Hand.cmo", *m_pEffectFactory);// �G�̎胂�f����ǂݍ���
	m_pModelMap["EnemyShadow"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy.cmo", *m_pEffectFactory);// �G�̉e�p���f����ǂݍ���
	m_pModelMap["EnemyDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *m_pEffectFactory);// �G�̃_���[�W�烂�f����ǂݍ���
	m_pModelMap["EnemyAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *m_pEffectFactory);// �G�̍U���烂�f����ǂݍ���
	m_pModelMap["EnemyIdling"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *m_pEffectFactory);// �G�̕��i�̊烂�f����ǂݍ���
}
/*
*	@brief �����U���G���f���̍쐬
*	@details �����U���G�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateVerticalAttackerModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/VerticalAttacker");// ���f���̃f�B���N�g�����w��
	m_pModelMap["VerticalAttacker"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *m_pEffectFactory);// �����U���G�̃��f����ǂݍ���
}
/*
*	@brief �{�X���f���̍쐬
*	@details �{�X�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBossModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g�����w��
	m_pModelMap["BossBody"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss.cmo", *m_pEffectFactory);// �{�X�̓��̃��f����ǂݍ���
	m_pModelMap["BossFaceDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Damage.cmo", *m_pEffectFactory);// �{�X�̃_���[�W�烂�f����ǂݍ���
	m_pModelMap["BossFaceAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *m_pEffectFactory);// �{�X�̍U���烂�f����ǂݍ���
	m_pModelMap["BossFaceAngry"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Angry.cmo", *m_pEffectFactory);// �{���Ԃ̊�
}
/*
*	@brief ���X�{�X���f���̍쐬
*	@details ���X�{�X�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateLastBossModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g�����w��
	m_pModelMap["LastBossBody"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_Body.cmo", *m_pEffectFactory);// ���X�{�X�̓��̃��f����ǂݍ���
	m_pModelMap["LastBossFaceDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_DamageFace.cmo", *m_pEffectFactory);// ���X�{�X�̃_���[�W�烂�f����ǂݍ���
	m_pModelMap["LastBossFaceAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_Face.cmo", *m_pEffectFactory);// ���X�{�X�̍U���烂�f����ǂݍ���
	m_pModelMap["LastBossFaceAngry"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_AngryFace.cmo", *m_pEffectFactory);// ���X�{�X�̓{��烂�f����ǂݍ���
}
/*
*	@brief �o���A���f���̍쐬
*	@details �o���A�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateBarrierModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// ���f���̃f�B���N�g�����w��
	m_pModelMap["Barrier"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Barrier.cmo", *m_pEffectFactory);// �{�X�̃o���A���f����ǂݍ���
}
/*
*	@brief �X�e�[�W���f���̍쐬
*	@details �X�e�[�W�̃��f�����쐬����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::CreateStageModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/models/Stage");// ���f���̃f�B���N�g����ݒ�
	m_pModelMap["Stage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/models/Stage/Stage.cmo", *m_pEffectFactory);// �X�e�[�W���f����ǂݍ���
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)	// ���f���̃G�t�F�N�g�����X�V����
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (!basicEffect)return;// �G�t�F�N�g��nullptr�̏ꍇ�͏������I����
			basicEffect->SetLightEnabled(0, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(1, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetLightEnabled(2, false);// ���C�g�𖳌��ɂ���
			basicEffect->SetEmissiveColor(DirectX::Colors::White);// ���f����������������
		}
	);
}
/*
*	@brief ���f�����擾����
*	@details �w�肳�ꂽ�L�[�ɑΉ����郂�f�����擾����
*	@param key ���f���̃L�[
*	@return �w�肳�ꂽ�L�[�ɑΉ����郂�f���̃|�C���^�B������Ȃ��ꍇ��nullptr��Ԃ�
*/
DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	auto it = m_pModelMap.find(key);// �L�[�ɑΉ����郂�f��������
	if (it != m_pModelMap.end())	return it->second.get();// ���������ꍇ�̓��f����Ԃ�
	return nullptr;// ������Ȃ������ꍇ��nullptr��Ԃ�
}
/*
*	@brief �w�肳�ꂽ�X�e�[�W�̋�̃��f�����쐬���ĕԂ�
*	@details �X�e�[�WID�ɉ�������̃��f�����擾����
*	@param stageID �X�e�[�WID
*	@return �w�肳�ꂽ�X�e�[�W�̋�̃��f���̃|�C���^�B������Ȃ��ꍇ��nullptr��Ԃ�
*/
DirectX::Model* ModelManager::GetSkyModel(const std::string& stageID)
{
	auto it = m_skyModelPaths.find(stageID); // �X�e�[�WID�ɑΉ������̃��f���̃p�X������
	if (it == m_skyModelPaths.end())return nullptr; // ������Ȃ������ꍇ��nullptr��Ԃ�
	m_pEffectFactory->SetDirectory(L"Resources/Models/sky");// ���f���̃f�B���N�g�����w��
	std::wstring wpath = ConvertToWString(it->second); // ����������C�h������ɕϊ�
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory); // ���f����ێ�
	return m_pModelMap["Sky"].get(); // ���f����Ԃ�
}
/*
*	@brief ����������C�h������ɕϊ�����
*	@details std::string��std::wstring�ɕϊ�����
*	@param str �ϊ����镶����
*	@return �ϊ���̃��C�h������
*/
std::wstring ModelManager::ConvertToWString(const std::string& str) { return std::wstring(str.begin(), str.end()); }