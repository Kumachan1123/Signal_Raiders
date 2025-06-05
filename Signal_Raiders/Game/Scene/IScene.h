/*
*	@file	IScene.h
*	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once
#ifndef ISCENE_DEFINED
#define ISCENE_DEFINED

// �O���錾
class CommonResources;

// �V�[���̃C���^�[�t�F�C�X�N���X
class IScene
{
public:
	// �񋓌^
	// �V�[���h�c
	enum class SceneID : unsigned int
	{
		NONE,// �V�[���Ȃ�
		TITLE,// �^�C�g��
		SETTING,// �ݒ���
		STAGESELECT,// �X�e�[�W�Z���N�g
		PLAY,// �v���C
		CLEAR,// �N���A
		GAMEOVER// �Q�[���I�[�o�[
	};
public:
	// �֐�
	// �f�X�g���N�^
	virtual ~IScene() = default;
	// ������
	virtual void Initialize(CommonResources* resources) = 0;
	// �X�V
	virtual void Update(float elapsedTime) = 0;
	// �`��
	virtual void Render() = 0;
	// �I��
	virtual void Finalize() = 0;
	// ���̃V�[��ID���擾
	virtual SceneID GetNextSceneID() const = 0;
};
#endif // ISCENE_DEFINED
