/*
*	@file TextureManager.h
*	@brief �e�N�X�`���}�l�[�W���[�N���X
*/
#pragma once
// �W�����C�u����
#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>
// DirectX
#include <DeviceResources.h>
// �O���錾
class CommonResources;
class TextureManager
{
public:
	// �A�N�Z�T
	ID3D11ShaderResourceView* GetTexture(const std::string& key); // �e�N�X�`���̎擾
public:// public�֐�
	TextureManager(); // �R���X�g���N�^
	~TextureManager(); // �f�X�g���N�^
	void Initialize(ID3D11Device1* pDevice); // �e�N�X�`���̏�����
private:
	void LoadTexture(const std::string& key, const wchar_t* path); // �e�N�X�`���̓ǂݍ���
private:// �萔
	const std::string m_textureJsonPath = "Resources/Jsons/textures.json"; // �e�N�X�`���f�[�^��JSON�t�@�C���p�X
private:
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �f�o�C�X
	ID3D11Device1* m_pDevice;
	// �e�N�X�`���̃}�b�v
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_pTextureMap;

};