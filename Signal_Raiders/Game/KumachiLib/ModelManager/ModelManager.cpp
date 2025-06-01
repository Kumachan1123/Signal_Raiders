/*
*	@file ModelManager.cpp
*	@brief ���f���}�l�[�W���[�N���X
*/
#include <pch.h>
#include "ModelManager.h"
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"

/*
*	@brief �R���X�g���N�^
*	@details ���f���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
ModelManager::ModelManager()
	: m_pDevice(nullptr) // �f�o�C�X
	, m_pModelMap() // ���f���̃}�b�v
	, m_skyModelPathMap() // �V�����f���p�X�̃}�b�v
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
}
/*
*	@brief ���f���̏�����
*	@details �e�탂�f���̃��[�h�ƃG�t�F�N�g�̐ݒ���s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::Initialize(ID3D11Device1* pDevice)
{
	m_pDevice = pDevice; // �f�o�C�X��ݒ�
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);// �G�t�F�N�g�t�@�N�g���[�̍쐬
	m_pEffectFactory->SetSharing(false);// �G�t�F�N�g�̋��L�𖳌��ɂ���
	LoadJsonFile();// JSON�t�@�C���̓ǂݍ���
	LoadSkyModelsJson(); // �V�����f���̃p�X��JSON�t�@�C������ǂݍ���
	SetupBulletModelEffects(); // �e���f���̍쐬
	SetupStageModelEffects(); // �X�e�[�W���f���̍쐬
}
/*
*	@brief JSON�t�@�C���̓ǂݍ���
*	@details ���f���̃p�X��JSON�t�@�C������ǂݍ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::LoadJsonFile()
{
	using json = nlohmann::json;// nlohmann::json�̃G�C���A�X���`
	using namespace std;// �W�����O��Ԃ��g�p
	string filename = "Resources/Jsons/Models.json";//�ǂݍ��ރt�@�C���̖��O���쐬
	ifstream ifs(filename.c_str());//�t�@�C�����J��
	if (!ifs.good())return;// �t�@�C��������ɊJ���Ȃ������狭���I��
	json j;							//json�I�u�W�F�N�g
	ifs >> j;						//�t�@�C������ǂݍ���
	ifs.close();					//�t�@�C�������
	for (const auto& item : j.items())// JSON�̊e�A�C�e���ɑ΂��ă��[�v
	{
		std::string key = item.key();                    // �L�[
		std::string path = item.value();                 // �l�i�t�@�C���p�X�j
		std::wstring wpath(path.begin(), path.end());    // ������ϊ�
		m_pEffectFactory->SetDirectory(L"Resources/Models");// ���f���̃f�B���N�g�����w��
		m_pModelMap[key] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory);// ���f����ǂݍ���
	}

}
void ModelManager::LoadSkyModelsJson()
{
	using json = nlohmann::json;// nlohmann::json�̃G�C���A�X���`
	using namespace std;// �W�����O��Ԃ��g�p
	string filename = "Resources/Jsons/SkyModels.json";//�ǂݍ��ރt�@�C���̖��O���쐬
	ifstream ifs(filename.c_str());//�t�@�C�����J��
	if (!ifs.good())return;// �t�@�C��������ɊJ���Ȃ������狭���I��
	json j;							//json�I�u�W�F�N�g
	ifs >> j;						//�t�@�C������ǂݍ���
	ifs.close();					//�t�@�C�������
	for (const auto& item : j.items())// JSON�̊e�A�C�e���ɑ΂��ă��[�v
	{
		std::string key = item.key();                    // �L�[
		std::string path = item.value();                 // �l�i�t�@�C���p�X�j
		std::wstring wpath(path.begin(), path.end());    // ������ϊ�
		m_skyModelPathMap[key] = wpath; // ���f���p�X���}�b�v�ɒǉ�
	}
}
/*
*	@brief �e���f���̃G�t�F�N�g�ݒ�
*	@details ���e�ƓG�e�̃��f���ɃG�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::SetupBulletModelEffects()
{
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// ���e���f���̃G�t�F�N�g��ݒ肷��
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);// �G�t�F�N�g��BasicEffect�ɃL���X�g
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);// �g�U�F��ݒ�
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan); // �������F��ݒ�
		});
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// �G�e���f���̃G�t�F�N�g��ݒ肷��
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);// �G�t�F�N�g��BasicEffect�ɃL���X�g
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1)); // �g�U�F��ݒ�
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red); // �����̐F��ݒ�
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato); // �������F��ݒ�
		});
}

/*
*	@brief �X�e�[�W���f���̃G�t�F�N�g�ݒ�
*	@details �X�e�[�W���f���ɃG�t�F�N�g��ݒ肷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void ModelManager::SetupStageModelEffects()
{
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)// �X�e�[�W���f���̃G�t�F�N�g��ݒ肷��
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
	auto it = m_skyModelPathMap.find(stageID); // �X�e�[�WID�ɑΉ������̃��f���̃p�X������
	if (it == m_skyModelPathMap.end())	return nullptr; // ������Ȃ������ꍇ��nullptr��Ԃ�
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, it->second.c_str(), *m_pEffectFactory); // ���f����ێ�
	return m_pModelMap["Sky"].get(); // ���f����Ԃ�
}
