/*
*	@file MyMouse.h
*	@brief InputManager�݂̂ł͂ł��Ȃ��}�E�X�̓��͂��g������N���X
*/
#pragma once
// DirectX
#include <Mouse.h>

// �g���}�E�X���̓N���X
class MyMouse
{
public:
	// �֐�
	// ���N���b�N�������ꂽ�u�Ԃ��擾
	static bool IsLeftMouseButtonPressed(const DirectX::Mouse::State& currentState);
public:
	// �ϐ�
	// �O��̃}�E�X�{�^���̏��
	static bool m_prevMouseButtonState;
};
