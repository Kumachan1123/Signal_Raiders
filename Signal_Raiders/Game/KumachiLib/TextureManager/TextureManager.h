/*
*	@file TextureManager.h
*	@brief �e�N�X�`���}�l�[�W���[�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>
 // �O�����C�u����
#include "nlohmann/json.hpp"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// �O���錾
class CommonResources;
class TextureManager
{
public:
	TextureManager();	// �R���X�g���N�^
	~TextureManager();	// �f�X�g���N�^
	void Initialize(CommonResources* commonResources); // ������
	void Load();// Json�t�@�C������e�N�X�`���p�X�ƃL�[��ǂݍ���
	void LoadTexture(const std::string& key, const wchar_t* path); // �e�N�X�`���̓ǂݍ���
	ID3D11ShaderResourceView* GetTexture(const std::string& key); // �e�N�X�`���̎擾
	void ClearTextures(); // �e�N�X�`���̃N���A

private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// �e�N�X�`���̃}�b�v
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;
};