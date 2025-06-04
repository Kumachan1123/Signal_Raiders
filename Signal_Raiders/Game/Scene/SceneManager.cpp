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
	// ���\�[�X��nullptr�łȂ����Ƃ��m�F
	assert(resources);
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �^�C�g���V�[���ɕύX
	ChangeScene(IScene::SceneID::TITLE);
}
/*
*	@brief �X�V����
*	@details �V�[���}�l�[�W���N���X�̍X�V
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void SceneManager::Update(float elapsedTime)
{
	// ���݂̃V�[�����X�V
	m_pCurrentScene->Update(elapsedTime);
	// ���̃V�[��ID��NONE�̏ꍇ�͂����ŏ������I���
	if (m_pCurrentScene->GetNextSceneID() == IScene::SceneID::NONE) return;
	// �V�[����ύX����Ƃ�
	ChangeScene(m_pCurrentScene->GetNextSceneID());
}
/*
*	@brief �`�悷��
*	@details �V�[���}�l�[�W���N���X�̕`��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Render()
{
	// ���݂̃V�[����`�悷��
	m_pCurrentScene->Render();
}
/*
*	@brief �I������
*	@details �V�[�����폜����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::Finalize()
{
	// ���݂̃V�[�����폜����
	DeleteScene();
}
/*
*	@brief �V�[����ύX����
*	@details ���̃V�[���������ĐV�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::ChangeScene(IScene::SceneID sceneID)
{
	// �V�[�����폜����
	DeleteScene();
	// �V�����V�[�����쐬����
	CreateScene(sceneID);
}
/*
*	@brief �V�[�����쐬����
*	@details �V�����V�[�����쐬����
*	@param sceneID �V�����V�[����ID
*	@return �Ȃ�
*/
void SceneManager::CreateScene(IScene::SceneID sceneID)
{
	// ���݂̃V�[����nullptr�ł��邱�Ƃ��m�F
	assert(m_pCurrentScene == nullptr);
	// �V�[��ID�ɂ���ď����𕪂���
	switch (sceneID)
	{
	case IScene::SceneID::TITLE:// �^�C�g���V�[��
		// �^�C�g���V�[�����쐬
		m_pCurrentScene = std::make_unique<TitleScene>(sceneID);
		// �������I����
		break;
	case IScene::SceneID::SETTING:// �ݒ�V�[��
		// �ݒ�V�[�����쐬
		m_pCurrentScene = std::make_unique<SettingScene>(sceneID);
		// �������I����
		break;
	case IScene::SceneID::STAGESELECT:// �X�e�[�W�I���V�[��
		// �X�e�[�W�I���V�[�����쐬
		m_pCurrentScene = std::make_unique<StageSelectScene>(sceneID);
		// �������I����
		break;
	case IScene::SceneID::PLAY:// �v���C�V�[��
		// �v���C�V�[�����쐬
		m_pCurrentScene = std::make_unique<PlayScene>(sceneID);
		// �������I����
		break;
	case IScene::SceneID::CLEAR:	// ���U���g�V�[��
	case IScene::SceneID::GAMEOVER:	// CLEAR��GAMEOVER�͓�������
		// ���U���g�V�[�����쐬
		m_pCurrentScene = std::make_unique<ResultScene>(sceneID);
		// �������I����
		break;
	default:// ����ȊO�̃V�[��
		// �V�[���������݂��Ȃ��ꍇ�̓G���[
		assert(!"SceneManager::CreateScene::�V�[���������݂��܂���I");
		// no break
	}
	// �V�[������������Ă��邱�Ƃ��m�F
	assert(m_pCurrentScene && "SceneManager::CreateScene::���̃V�[������������܂���ł����I");
	// �ȉ��̏����𖞂������ꍇ
	if ((GetSceneID() == IScene::SceneID::STAGESELECT ||// �X�e�[�W�I����
		GetSceneID() == IScene::SceneID::CLEAR ||		// �N���A��
		GetSceneID() == IScene::SceneID::GAMEOVER) &&	// �Q�[���I�[�o�[�̂ǂꂩ����
		m_stageNumber < SceneManager::STAGE_MAX)		// �X�e�[�W�ԍ���5�����̏ꍇ
	{
		// �v���C�V�[�����擾
		auto playScene = dynamic_cast<PlayScene*>(m_pCurrentScene.get());
		// �v���C�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		assert(playScene);
		// �X�e�[�W�ԍ����Z�b�g
		playScene->SetStageNumber(m_stageNumber);
	}
	// �v���C�V�[���̏ꍇ
	if (GetSceneID() == IScene::SceneID::PLAY)
	{
		// ���U���g�V�[�����擾
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());
		// ���U���g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		assert(resultScene);
		// �X�e�[�W�ԍ����Z�b�g
		resultScene->SetStageNumber(m_stageNumber);
	}
	// �V�[���̏�����
	m_pCurrentScene->Initialize(m_pCommonResources);
	// �V�[��ID���Z�b�g
	SetSceneID(sceneID);
}
/*
*	@brief �V�[�����폜����
*	@details ���݂̃V�[�����폜����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void SceneManager::DeleteScene()
{
	// �V�[����nullptr�̏ꍇ�͉������Ȃ�
	if (!m_pCurrentScene)return;
	// ���݂̃V�[�����u�Z���N�g�V�[���v�̏ꍇ�A�X�e�[�W�ԍ����擾����
	if (GetSceneID() == IScene::SceneID::STAGESELECT)
	{
		//�X�e�[�W�ԍ����擾����
		auto stageSelectScene = dynamic_cast<StageSelectScene*>(m_pCurrentScene.get());
		// �X�e�[�W�Z���N�g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		assert(stageSelectScene);
		// �X�e�[�W�ԍ����擾����
		m_stageNumber = stageSelectScene->GetStageNumber();
	}
	// ���݂̃V�[�����u���U���g�V�[���v�̏ꍇ�A�X�e�[�W�ԍ����擾����
	if (GetSceneID() == IScene::SceneID::CLEAR ||// �N���A�V�[����
		GetSceneID() == IScene::SceneID::GAMEOVER)// �Q�[���I�[�o�[�V�[���̏ꍇ
	{
		// �X�e�[�W�ԍ����擾����
		auto resultScene = dynamic_cast<ResultScene*>(m_pCurrentScene.get());
		// ���U���g�V�[�����擾�ł��Ă��邱�Ƃ��m�F
		assert(resultScene);
		//�X�e�[�W�ԍ����擾����
		m_stageNumber = resultScene->GetStageNumber();
	}
	// �V�[�����폜����
	m_pCurrentScene.reset();
}