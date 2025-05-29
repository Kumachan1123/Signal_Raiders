/*
*	@file ModelManager.h
*	@brief ���f���}�l�[�W���[�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <unordered_map>
#include <string>
// DirectX
#include <DeviceResources.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
// �O���錾
class CommonResources;
class ModelManager
{
public:// �A�N�Z�T
	DirectX::Model* GetModel(const std::string& key);// ���f���̎擾
	DirectX::Model* GetSkyModel(const std::string& stageID);// �w�肳�ꂽ�X�e�[�W�̋�̃��f�����쐬���ĕԂ�
	void SetCommonResources(CommonResources* commonResources) { m_pCommonResources = commonResources; } // ���ʃ��\�[�X��ݒ肷��
public:// public�֐�
	ModelManager();	// �R���X�g���N�^
	~ModelManager();// �f�X�g���N�^
	void Initialize();// ���f���̏�����
private:
	std::wstring ConvertToWString(const std::string& str);// ������ϊ�
	void CreateBulletModels(); // �e���f���̍쐬
	void CreateEnemyModels(); // �G���f���̍쐬
	void CreateVerticalAttackerModels(); // �����U���G���f���̍쐬
	void CreateBossModels(); // �{�X���f���̍쐬
	void CreateLastBossModels(); // ���X�{�X���f���̍쐬
	void CreateBarrierModels(); // �o���A���f���̍쐬
	void CreateStageModels(); // �X�e�[�W���f���̍쐬
private:// �萔
	const std::unordered_map<std::string, std::string> m_skyModelPaths = // �󃂃f���̃p�X
	{
		{"Stage1", "Resources/Models/sky/sky.cmo"}, // �X�e�[�W1�̋󃂃f��
		{"Stage2", "Resources/Models/sky/CloudySky.cmo"}, // �X�e�[�W2�̋󃂃f��
		{"Stage3", "Resources/Models/sky/EveningSky.cmo"}, // �X�e�[�W3�̋󃂃f��
		{"Stage4", "Resources/Models/sky/NightSky.cmo"}, // �X�e�[�W4�̋󃂃f��
		{"Stage5", "Resources/Models/sky/MidNightSky.cmo"} // �X�e�[�W5�̋󃂃f��
	};
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���f���̃}�b�v
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
