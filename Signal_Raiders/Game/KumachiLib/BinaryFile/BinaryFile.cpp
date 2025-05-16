/*
*	@file BinaryFile.cpp
*	@brief �t�@�C���ǂݍ��݃N���X
*/
#include "pch.h"
#include "BinaryFile.h"
/*
*	@brief �t�@�C�������w�肵�ă��[�h
*	@details �t�@�C�������w�肵�ăo�C�i���t�@�C����ǂݍ���
*	@param fileName �t�@�C����
*	@return BinaryFile �ǂݍ��񂾃t�@�C��
*/
KumachiLib::BinaryFile KumachiLib::BinaryFile::LoadFile(const wchar_t* fileName)
{
	BinaryFile bin;// �o�C�i���t�@�C���N���X�̃C���X�^���X���쐬
	std::ifstream ifs;// ���̓t�@�C���X�g���[�����쐬
	ifs.open(fileName, std::ios::in | std::ios::binary);// �t�@�C���I�[�v��
	assert(ifs);// �ǂݍ��ݎ��s���A�����I��
	ifs.seekg(0, std::fstream::end);// �t�@�C���T�C�Y���擾
	std::streamoff eofPos = ifs.tellg();// �t�@�C���̏I�[�ʒu���擾
	ifs.clear();// �X�g���[���̏�Ԃ��N���A
	ifs.seekg(0, std::fstream::beg);// �t�@�C���̐擪�ʒu���擾
	std::streamoff begPos = ifs.tellg();// �t�@�C���̐擪�ʒu���擾
	bin.m_size = (unsigned int)(eofPos - begPos);// �t�@�C���T�C�Y���v�Z
	bin.m_data.reset(new char[bin.m_size]);// �ǂݍ��ނ��߂̃��������m��
	ifs.read(bin.m_data.get(), bin.m_size);// �t�@�C���擪����o�b�t�@�փR�s�[
	ifs.close();// �t�@�C���N���[�Y
	return std::move(bin);// �o�C�i���t�@�C����Ԃ�
}
/*
*	@brief �R���X�g���N�^
*	@details �o�C�i���t�@�C���N���X�̃C���X�^���X���쐬
*	@param �Ȃ�
*	@return �Ȃ�
*/
KumachiLib::BinaryFile::BinaryFile()
{
	m_size = 0;// �T�C�Y��0�ɏ�����
}
/*
*	@brief ���[�u�R���X�g���N�^
*	@details ���[�u�R���X�g���N�^
*	@param in ���[�u���̃o�C�i���t�@�C��
*	@return �Ȃ�
*/
KumachiLib::BinaryFile::BinaryFile(BinaryFile&& in)
{
	m_data = std::move(in.m_data);// ���[�u���̃f�[�^���ړ�
	m_size = in.m_size;// �T�C�Y���R�s�[
}
