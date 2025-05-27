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
	void SetCommonResources(CommonResources* commonResources) { m_pCommonResources = commonResources; } // ���ʃ��\�[�X��ݒ肷��
public:// public�֐�
	ModelManager();	// �R���X�g���N�^
	~ModelManager();// �f�X�g���N�^
	void Initialize();// ���f���̏�����
private:
	void CreateSkyModels();// �󃂃f���̍쐬
	void CreateBulletModels(); // �e���f���̍쐬
	void CreateEnemyModels(); // �G���f���̍쐬
	void CreateVerticalAttackerModels(); // �����U���G���f���̍쐬
	void CreateBossModels(); // �{�X���f���̍쐬
	void CreateLastBossModels(); // ���X�{�X���f���̍쐬
	void CreateBarrierModels(); // �o���A���f���̍쐬
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// ���f���̃}�b�v
	std::unordered_map<std::string, std::shared_ptr<DirectX::Model>> m_pModelMap;
	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
