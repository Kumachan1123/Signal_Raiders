/*
*	@file	IScene.h
*	@brief	�V�[���̃C���^�[�t�F�C�X�N���X
*/
#pragma once
// �O���錾
class CommonResources;
class IScene
{
public:
	// �񋓌^
	enum class SceneID : unsigned int// �V�[���h�c
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
	virtual ~IScene() = default;// �f�X�g���N�^
	virtual void Initialize(CommonResources* resources) = 0;// ������
	virtual void Update(float elapsedTime) = 0;// �X�V
	virtual void Render() = 0;// �`��
	virtual void Finalize() = 0;// �I��
	virtual SceneID GetNextSceneID() const = 0;// ���̃V�[��ID���擾
};
