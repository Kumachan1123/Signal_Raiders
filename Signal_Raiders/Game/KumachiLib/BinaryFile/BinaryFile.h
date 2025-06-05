/*
*	@file BinaryFile.h
*	@brief �t�@�C���ǂݍ��݃N���X
*/
#pragma once
#ifndef BINARYFILE_DEFINED
#define BINARYFILE_DEFINED
// �W�����C�u����
#include <memory>
#include <fstream>
#include <assert.h>

namespace KumachiLib
{
	// �t�@�C���ǂݍ��݃N���X
	class BinaryFile
	{
	public:
		//	�A�N�Z�T
		// �f�[�^���擾
		char* GetData() { return m_pData.get(); }
		// �t�@�C���̃T�C�Y���擾
		unsigned int GetSize() const { return m_size; }
	public:
		// public�����o�֐�
		// �t�@�C�������w�肵�ă��[�h
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);
		// �R���X�g���N�^
		BinaryFile();
		// ���[�u�R���X�g���N�^
		BinaryFile(BinaryFile&& in)noexcept;
	protected:
		//protected�����o�ϐ�
		// �f�[�^
		std::unique_ptr<char[]> m_pData;
		// �T�C�Y
		unsigned int m_size;
	};
}
#endif // BINARYFILE_DEFINED