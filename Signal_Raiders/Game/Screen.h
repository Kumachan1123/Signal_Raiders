/*
*	@file	Screen.h
*	@brief	��ʂɂ܂��萔���܂Ƃ߂��N���X
*/
#pragma once
class Screen
{

public:// �萔
	// UI�ɂ������ʃT�C�Y
	// ��
	static const int UI_WIDTH = 1280;
	// ����
	static const int UI_HEIGHT = 720;
	// ����X���W
	static const int UI_CENTER_X = UI_WIDTH / 2;
	// ����Y���W
	static const int UI_CENTER_Y = UI_HEIGHT / 2;
	// ��[
	static const int UI_TOP = 0;
	// ���[
	static const int UI_BOTTOM = UI_HEIGHT;
	// ���[
	static const int UI_LEFT = 0;
	// �E�[
	static const int UI_RIGHT = UI_WIDTH;
	// ��ʃT�C�Y
	// ��
	static const int WIDTH = 1920;
	// ����
	static const int HEIGHT = 1080;
	// ����X���W
	static const int CENTER_X = WIDTH / 2;
	// ����Y���W
	static const int CENTER_Y = HEIGHT / 2;
	// ��[
	static const int TOP = 0;
	// ���[
	static const int BOTTOM = HEIGHT;
	// ���[
	static const int LEFT = 0;
	// �E�[
	static const int RIGHT = WIDTH;
public:// public�֐�
	// �R���X�g���N�^
	Screen() = default;
	// �f�X�g���N�^
	~Screen() = default;
};
