/*
*	@file BinaryFile.h
*	@brief �t�@�C���ǂݍ��݃N���X
*/

#pragma once

// �W�����C�u����
#include <memory>
#include <fstream>
#include <assert.h>
namespace KumachiLib
{
	class BinaryFile
	{
	protected:
		//protected�����o�ϐ�
		std::unique_ptr<char[]> m_data;		// �f�[�^
		unsigned int m_size;		// �T�C�Y
	public:
		//	�A�N�Z�T
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() const { return m_size; }
		// public�����o�֐�
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);		// �t�@�C�������w�肵�ă��[�h
		BinaryFile();// �R���X�g���N�^
		BinaryFile(BinaryFile&& in);// ���[�u�R���X�g���N�^

	};
}