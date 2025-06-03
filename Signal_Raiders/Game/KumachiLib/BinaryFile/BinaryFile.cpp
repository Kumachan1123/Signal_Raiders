/*
*	@file BinaryFile.cpp
*	@brief �t�@�C���ǂݍ��݃N���X
*/
#include <pch.h>
#include "BinaryFile.h"
/*
*	@brief �R���X�g���N�^
*	@details �o�C�i���t�@�C���N���X�̃C���X�^���X���쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
KumachiLib::BinaryFile::BinaryFile() { m_size = 0; }// �T�C�Y��0�ɏ�����
/*
*	@brief ���[�u�R���X�g���N�^
*	@details ���[�u�R���X�g���N�^
*	@param in ���[�u���̃o�C�i���t�@�C��
*	@return �Ȃ�
*/
KumachiLib::BinaryFile::BinaryFile(BinaryFile&& in) noexcept
	: m_pData{}// ���[�u���̃f�[�^��������
	, m_size{ 0 }// �T�C�Y��0�ɏ�����
{
	// ���[�u���̃f�[�^���ړ�
	m_pData = std::move(in.m_pData);
	// �T�C�Y���R�s�[
	m_size = in.m_size;
}

/*
*	@brief �t�@�C�������w�肵�ă��[�h
*	@details �t�@�C�������w�肵�ăo�C�i���t�@�C����ǂݍ���
*	@param fileName �t�@�C����
*	@return BinaryFile �ǂݍ��񂾃t�@�C��
*/
KumachiLib::BinaryFile KumachiLib::BinaryFile::LoadFile(const wchar_t* fileName)
{
	// �o�C�i���t�@�C���N���X�̃C���X�^���X���쐬
	BinaryFile bin;
	// ���̓t�@�C���X�g���[�����쐬
	std::ifstream ifs;
	// �t�@�C���I�[�v��
	ifs.open(fileName, std::ios::in | std::ios::binary);
	// �ǂݍ��ݎ��s���A�����I��
	assert(ifs);
	// �t�@�C���T�C�Y���擾
	ifs.seekg(0, std::fstream::end);
	// �t�@�C���̏I�[�ʒu���擾
	std::streamoff eofPos = ifs.tellg();
	// �X�g���[���̏�Ԃ��N���A
	ifs.clear();
	// �t�@�C���̐擪�ʒu���擾
	ifs.seekg(0, std::fstream::beg);
	// �t�@�C���̐擪�ʒu���擾
	std::streamoff begPos = ifs.tellg();
	// �t�@�C���T�C�Y���v�Z
	bin.m_size = (unsigned int)(eofPos - begPos);
	// �ǂݍ��ނ��߂̃��������m��
	bin.m_pData.reset(new char[bin.m_size]);
	// �t�@�C���擪����o�b�t�@�փR�s�[
	ifs.read(bin.m_pData.get(), bin.m_size);
	// �t�@�C���N���[�Y
	ifs.close();
	// �o�C�i���t�@�C����Ԃ�
	return bin;
}
