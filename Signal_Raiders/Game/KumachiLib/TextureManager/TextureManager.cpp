/*
*	@file TextureManager.cpp
*	@brief �e�N�X�`���}�l�[�W���[�N���X
*/
#include <pch.h>
#include "TextureManager.h"
/*
*	@brief �R���X�g���N�^
*	@details �e�N�X�`���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_pDevice(nullptr) // �f�o�C�X
	, m_pTextureMap() // �e�N�X�`���̃}�b�v
{
}
/*
*	@brief �f�X�g���N�^
*	@details �e�N�X�`���}�l�[�W���[�N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TextureManager::~TextureManager()
{
}
/*
*	@brief ������
*	@details �e�N�X�`���}�l�[�W���[�N���X�̏�����
*	@param CommonResources* commonResources	���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void TextureManager::Initialize(CommonResources* commonResources)
{
	m_pCommonResources = commonResources; // ���ʃ��\�[�X�̐ݒ�
	m_pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice(); // �f�o�C�X�̎擾
	Load(); // �e�N�X�`���̓ǂݍ���
}
/*
*	@brief �ǂݍ���
*	@details Json�t�@�C������e�N�X�`���p�X�ƃL�[��ǂݍ���
*	@param �Ȃ�
*	@return �Ȃ�
*/
void TextureManager::Load()
{
	//using json = nlohmann::json;
	//using namespace std;
	//string filename = "Resources/Jsons/Textures.json";//�ǂݍ��ރt�@�C���̖��O���쐬
	//ifstream ifs(filename.c_str());//�t�@�C�����J��
	//if (ifs.good())// �t�@�C��������ɊJ������
	//{
	//	json j;							//json�I�u�W�F�N�g
	//	ifs >> j;						//�t�@�C������ǂݍ���
	//	ifs.close();					//�t�@�C�������
	//	for (auto it = j.begin(); it != j.end(); ++it)
	//	{
	//		string key = it.key();               // �e�N�X�`���̃L�[
	//		string path = it.value();            // �t�@�C���p�X
	//		wstring wpath(path.begin(), path.end()); // std::string �� std::wstring �ɕϊ�
	//		LoadTexture(key, wpath.c_str()); // �e�N�X�`���̓ǂݍ���
	//	}
	//}
}
/*
*	@brief �e�N�X�`���̓ǂݍ���
*	@details �w�肳�ꂽ�L�[�ƃp�X�Ńe�N�X�`����ǂݍ��݁A�}�b�v�ɒǉ�����
*	@param key �e�N�X�`���̃L�[
*	@param path �e�N�X�`���̃p�X
*	@return �Ȃ�
*/
void TextureManager::LoadTexture(const std::string& key, const wchar_t* path)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;// �e�N�X�`���i�[�p
	DirectX::CreateWICTextureFromFile(m_pDevice, path, nullptr, texture.ReleaseAndGetAddressOf());// �e�N�X�`���ǂݍ���
	m_pTextureMap[key] = texture;           // �}�b�v�ɒǉ�
}

ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key)
{
	return nullptr;
}

void TextureManager::ClearTextures()
{
	return;
}
