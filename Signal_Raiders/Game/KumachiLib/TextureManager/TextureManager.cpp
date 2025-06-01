/*
*	@file TextureManager.cpp
*	@brief �e�N�X�`���}�l�[�W���[�N���X�̎���
*/
#include <pch.h>
#include "TextureManager.h"
// �O�����C�u����
#include "Libraries/nlohmann/json.hpp"
/*
*	@brief �R���X�g���N�^
*	@details �e�N�X�`���}�l�[�W���[�N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
TextureManager::TextureManager()
	: m_pCommonResources(nullptr)// ���ʃ��\�[�X
	, m_pDevice(nullptr)// �f�o�C�X
	, m_pTextureMap()// �e�N�X�`���}�b�v
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
	for (auto& pair : m_pTextureMap)// �e�N�X�`���}�b�v�̉��
		pair.second.Reset(); // ComPtr�̃��Z�b�g
	m_pTextureMap.clear(); // �}�b�v�̃N���A
}
/*
*	@brief �e�N�X�`���̏�����
*	@details �e�N�X�`���}�l�[�W���[�N���X�̏��������s��
*	@param pDevice �f�o�C�X
*	@return �Ȃ�
*/
void TextureManager::Initialize(ID3D11Device1* pDevice)
{
	using json = nlohmann::json;// nlohmann::json�̃G�C���A�X���`
	using namespace std;// �W�����O��Ԃ��g�p
	m_pDevice = pDevice; // �f�o�C�X�̐ݒ�
	string filename = "Resources/Jsons/Textures.json";//�ǂݍ��ރt�@�C���̖��O���쐬
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
		LoadTexture(key, wpath.c_str());                 // �ǂݍ��݊֐�
	}
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
/*
*	@brief �e�N�X�`���̎擾
*	@details �w�肳�ꂽ�L�[�̃e�N�X�`�����擾����
*	@param key �e�N�X�`���̃L�[
*	@return �w�肳�ꂽ�L�[�̃e�N�X�`���ւ̃|�C���^
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(const std::string& key) { return m_pTextureMap[key].Get(); }// �w�肳�ꂽ�L�[�̃e�N�X�`�����擾