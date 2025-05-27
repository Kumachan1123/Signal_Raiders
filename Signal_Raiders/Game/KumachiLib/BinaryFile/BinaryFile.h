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
	public:
		//	�A�N�Z�T
		char* GetData() { return m_pData.get(); }
		unsigned int GetSize() const { return m_size; }
		// public�����o�֐�
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);		// �t�@�C�������w�肵�ă��[�h
		BinaryFile();// �R���X�g���N�^
		BinaryFile(BinaryFile&& in)noexcept;// ���[�u�R���X�g���N�^
	protected:
		//protected�����o�ϐ�
		std::unique_ptr<char[]> m_pData;		// �f�[�^
		unsigned int m_size;		// �T�C�Y
	};
}