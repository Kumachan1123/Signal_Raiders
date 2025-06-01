/*
*	@file ModelManager.h
*	@brief ���f���}�l�[�W���[�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>

class ModelManager
{
public:// �A�N�Z�T
	DirectX::Model* GetModel(const std::string& key);// ���f���̎擾
	DirectX::Model* GetSkyModel(const std::string& stageID);// �w�肳�ꂽ�X�e�[�W�̋�̃��f�����쐬���ĕԂ�
public:// public�֐�
	ModelManager();	// �R���X�g���N�^
	~ModelManager();// �f�X�g���N�^
	void Initialize(ID3D11Device1* pDevice);// ���f���̏�����
private:
	//std::wstring ConvertToWString(const std::string& str);// ������ϊ�
	void SetupBulletModelEffects();   // �e���f���ɃG�t�F�N�g�ݒ�
	void SetupStageModelEffects();   // �X�e�[�W���f���ɃG�t�F�N�g�ݒ�
	void LoadJsonFile();// JSON�t�@�C���̓ǂݍ���
	void LoadSkyModelsJson();//�V�����f���̃p�X��JSON�t�@�C������ǂݍ���
private:// �萔
	const std::unordered_map<std::string, std::string> m_skyModelPaths = // �󃂃f���̃p�X
	{
		{"Stage1", "Resources/Models/sky.cmo"			}, // �X�e�[�W1�̋󃂃f��
		{"Stage2", "Resources/Models/CloudySky.cmo"		}, // �X�e�[�W2�̋󃂃f��
		{"Stage3", "Resources/Models/EveningSky.cmo"	}, // �X�e�[�W3�̋󃂃f��
		{"Stage4", "Resources/Models/NightSky.cmo"		}, // �X�e�[�W4�̋󃂃f��
		{"Stage5", "Resources/Models/MidNightSky.cmo"	} // �X�e�[�W5�̋󃂃f��
	};
private:// private�ϐ�
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���f���̃}�b�v
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// ���f���p�X�̃}�b�v
	std::unordered_map<std::string, std::wstring> m_skyModelPathMap;
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
