/*
*	@file	Screen.h
*	@brief	��ʂɂ܂��萔���܂Ƃ߂��N���X
*/
#pragma once
class Screen
{
public:// �R���X�g���N�^�E�f�X�g���N�^
	Screen() = default;
	~Screen() = default;
public:// �萔
	// UI�ɂ������ʃT�C�Y
	static const int UI_WIDTH = 1280;				// ��
	static const int UI_HEIGHT = 720;				// ����

	static const int UI_CENTER_X = UI_WIDTH / 2;	// ����X���W
	static const int UI_CENTER_Y = UI_HEIGHT / 2;	// ����Y���W

	static const int UI_TOP = 0;					// ��[
	static const int UI_BOTTOM = UI_HEIGHT;			// ���[
	static const int UI_LEFT = 0;					// ���[
	static const int UI_RIGHT = UI_WIDTH;			// �E�[


	// ��ʃT�C�Y
	static const int WIDTH = 1920;					// ��
	static const int HEIGHT = 1080;					// ����

	static const int CENTER_X = WIDTH / 2;			// ����X���W
	static const int CENTER_Y = HEIGHT / 2;			// ����Y���W

	static const int TOP = 0;						// ��[
	static const int BOTTOM = HEIGHT;				// ���[
	static const int LEFT = 0;						// ���[
	static const int RIGHT = WIDTH;					// �E�[
};
