/*
*	@file	SceneManager.cpp
*	@brief	�V�[���}�l�[�W���N���X
*/
#include <pch.h>
#include "SceneManager.h"
// �X�e�[�W��(��������??? < STAGE_MAX�̂悤�Ɏg���A0�`4���X�e�[�W�A5���^�C�g���ɖ߂�{�^���̔ԍ��j
const int SceneManager::STAGE_MAX = 5;
/*
*	@brief �R���X�g���N�^
*	@details �V�[���}�l�[�W���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
SceneManager::SceneManager()
	: m_pCurrentScene{}// ���݂̃V�[��
	, m_pCommonResources{}// ���ʃ��\�[�X
	, m_stageNumber{ 0 }// �X�e�[�W�ԍ�
	, m_nowSceneID{ IScene::SceneID::NONE }// ���݂̃V�[��ID
{
}
/*
*	@brief �f�X�g���N�^
*	@details �V�[���}�l�[�W���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return	�Ȃ�
*/
SceneManager::~SceneManager() { Finalize(); }
/*
*	@brief ����������
*	@details �V�[���}�l�[�W���N���X�̏�����
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void SceneManager::Initialize(CommonResources* resources)
{
	assert(resources);// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	m_pCommonResources = resources;// ���ʃ��\�[�X���擾
	ChangeScene(IScene::SceneID::TITLE);// �^�C�g���V�[���ɕύX
}
/*
*	@brief �X�V����
*	@details �V�[���}�l�[�W���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void SceneManager::Update(float elapsedTime)
{
	m_pCurrentScene->Update(elapsedTime); // ���݂̃V�[�����X�V
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;// ���̃V�[��ID��NONE�̏ꍇ�͂����ŏ������I���
	ChangeScene(m_pCurrentScene->GetNextSceneID());// �V�[����ύX����Ƃ�
}
/*
*	@brief �`�悷��
*	@details �V�[���}�l�[�W���N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Render() { m_pCurrentScene->Render(); }
/*
*	@brief �I������
*	@details �V�[�����폜����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Finalize() { DeleteScene(); }
/*
*	@brief �V�[����ύX����
*	@details ���̃V�[���������ĐV�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	DeleteScene();// �V�[�����폜����
	CreateScene(sceneID);// �V�����V�[�����쐬����
}
/*
*	@brief �V�[�����쐬����
*	@details �V�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	assert(m_pCurrentScene == nullptr);// ���݂̃V�[����nullptr�ł��邱�Ƃ��m�F
	switch (sceneID)// �V�[��ID�ɂ���ď����𕪂���
	{
	case IScene::SceneID::TITLE:// �^�C�g���V�[��
		m_pCurrentScene = std::make_unique<TitleScene>(sceneID);// �^�C�g���V�[�����쐬
		break;// �������I����
	case IScene::SceneID::SETTING:// �ݒ�V�[��
		m_pCurrentScene = std::make_unique<SettingScene>(sceneID);// �ݒ�V�[�����쐬
		break;// �������I����
	case IScene::SceneID::STAGESELECT:// �X�e�[�W�I���V�[��
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);// �X�e�[�W�I���V�[�����쐬
		break;// �������I����
	case IScene::SceneID::PLAY:// �v���C�V�[��
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);// �v���C�V�[�����쐬
		break;// �������I����
	case IScene::SceneID::CLEAR:	// ���U���g�V�[��
	case IScene::SceneID::GAMEOVER:	// CLEAR��GAMEOVER�͓�������
		m_pCurrentScene = std::make_unique<ResultScene>(sceneID);// ���U���g�V�[�����쐬
		break;// �������I����
	default:// ����ȊO�̃V�[��
		assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");// �V�[���������݂��Ȃ��ꍇ�̓G���[
		// no break
	}
	assert(m_pCurrentScene && "SceneManager::CreateScene::���̃V�[������������܂���ł����I");// �V�[������������Ă��邱�Ƃ��m�F
	if ((GetSceneID() == IScene::SceneID::STAGESELECT ||// �X�e�[�W�I����
		GetSceneID() == IScene::SceneID::CLEAR ||		// �N���A��
		GetSceneID() == IScene::SceneID::GAMEOVER) &&	// �Q�[���I�[�o�[�̂ǂꂩ����
		m_stageNumber < SceneManager::STAGE_MAX)		// �X�e�[�W�ԍ���5�����̏ꍇ
	{
		auto playScene = dynamic_cast<PlayScene*>(m_pCurrentScene.get());// �v���C�V�[�����擾
		assert(playScene);// �v���C�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		playScene->SetStageNumber(m_stageNumber);// �X�e�[�W�ԍ����Z�b�g
	}
	if (GetSceneID() == IScene::SceneID::PLAY)// �v���C�V�[���̏ꍇ
	{
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());// ���U���g�V�[�����擾
		assert(resultScene);// ���U���g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		resultScene->SetStageNumber(m_stageNumber);// �X�e�[�W�ԍ����Z�b�g
	}
	m_pCurrentScene->Initialize(m_pCommonResources);// �V�[���̏�����
	SetSceneID(sceneID);// �V�[��ID���Z�b�g
}
/*
*	@brief �V�[�����폜����
*	@details ���݂̃V�[�����폜����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::DeleteScene()
{
	if (!m_pCurrentScene)return;// �V�[����nullptr�̏ꍇ�͉������Ȃ�
	if (GetSceneID() == IScene::SceneID::STAGESELECT)// ���݂̃V�[�����u�Z���N�g�V�[���v�̏ꍇ�A�X�e�[�W�ԍ����擾����
	{
		auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_pCurrentScene.get());//�X�e�[�W�ԍ����擾����
		assert(stageSelectScene);// �X�e�[�W�Z���N�g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		m_stageNumber = stageSelectScene->GetStageNumber();// �X�e�[�W�ԍ����擾����
	}
	if (GetSceneID() == IScene::SceneID::CLEAR ||
		GetSceneID() == IScene::SceneID::GAMEOVER)// ���݂̃V�[�����u���U���g�V�[���v�̏ꍇ�A�X�e�[�W�ԍ����擾����
	{
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());// �X�e�[�W�ԍ����擾����
		assert(resultScene);// ���U���g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		m_stageNumber = resultScene->GetStageNumber();//�X�e�[�W�ԍ����擾����
	}
	m_pCurrentScene.reset();// �V�[�����폜����
}
