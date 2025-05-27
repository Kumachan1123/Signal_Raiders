/*
*	@file CreateShaders.cpp
*	@brief �V�F�[�_�[���쐬����V���O���g���N���X
*	@details �|���S����`�悷��ۂɎg�p����V�F�[�_�[���쐬����
*/
#include <pch.h>
#include "CreateShader.h"

using namespace KumachiLib;
// �V���O���g���C���X�^���X�̏�����
std::unique_ptr<CreateShader> CreateShader::m_pInstance = nullptr;

/*
*	@brief �V���O���g���C���X�^���X���擾
*	@details CreateShader�N���X�̃V���O���g���C���X�^���X���擾����
*	@param �Ȃ�
*	@return �V���O���g���C���X�^���X
*/
CreateShader* const CreateShader::GetInstance()
{
	if (m_pInstance == nullptr)// �C���X�^���X���Ȃ��ꍇ
	{
		m_pInstance.reset(new CreateShader());// �C���X�^���X�𐶐�
	}
	return m_pInstance.get();// �C���X�^���X��Ԃ�
}
/*
*	@brief �R���X�g���N�^
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
CreateShader::CreateShader()
	: m_pDevice(nullptr)// �f�o�C�X
	, m_pIDE(nullptr)// ���̓��C�A�E�g
	, m_NumElements(0)// �v�f��
	, m_pInputLayout(nullptr)// ���C�A�E�g
{
}
/*
*	@brief �f�X�g���N�^
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
CreateShader::~CreateShader()
{
	m_pInputLayout.Reset();// �C���v�b�g���C�A�E�g�̉��
	m_pIDE = nullptr;// ���̓��C�A�E�g�̉��
	m_pDevice = nullptr;// �f�o�C�X�̉��
}
/*
*	@brief ������
*	@details �V�F�[�_�[���쐬���邽�߂̏��������s��
*	@param device �f�o�C�X
*	@param pIDE ���̓��C�A�E�g
*	@param NumElements �v�f��
*	@param InputLayout ���C�A�E�g
*	@return �Ȃ�
*/
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	m_pDevice = device;// �f�o�C�X
	m_pIDE = pIDE;// ���̓��C�A�E�g
	m_NumElements = NumElements;// �v�f��
	m_pInputLayout = InputLayout;// ���C�A�E�g
}
/*
*	@brief ���_�V�F�[�_�[���쐬
*	@details ���_�V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param vs ���_�V�F�[�_�[�̊i�[��
* 	@return �Ȃ�
*/
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	BinaryFile VS = BinaryFile::LoadFile(fileName);// �o�C�i���t�@�C����ǂݍ���
	if (FAILED(m_pDevice->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))//�V�F�[�_�[���쐬
	{
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);// �G���[����
		return;// �I��
	}
	m_pDevice->CreateInputLayout(&m_pIDE[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());// ���̓��C�A�E�g���쐬
}
/*
*	@brief �s�N�Z���V�F�[�_�[���쐬
*	@details �s�N�Z���V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param ps �s�N�Z���V�F�[�_�[�̊i�[��
*	@return �Ȃ�
*/
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	BinaryFile PS = BinaryFile::LoadFile(fileName);// �o�C�i���t�@�C����ǂݍ���
	if (FAILED(m_pDevice->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))// �s�N�Z���V�F�[�_�쐬
	{
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);// �G���[����
		return;// �I��
	}
}
/*
*	@brief �W�I���g���V�F�[�_�[���쐬
*	@details �W�I���g���V�F�[�_�[���쐬����
*	@param fileName �V�F�[�_�[�̃t�@�C����
*	@param gs �W�I���g���V�F�[�_�[�̊i�[��
*	@return �Ȃ�
*/
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	BinaryFile GS = BinaryFile::LoadFile(fileName);// �o�C�i���t�@�C����ǂݍ���
	if (FAILED(m_pDevice->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))// �W�I���g���V�F�[�_�쐬
	{
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);// �G���[����
		return;// �I��
	}

}
/*
*	@brief �R���X�^���g�o�b�t�@���쐬
*	@details �R���X�^���g�o�b�t�@���쐬����
*	@param cBuffer �R���X�^���g�o�b�t�@�̊i�[��
*	@param bufferSize �o�b�t�@�̃T�C�Y
*	@return �Ȃ�
*/
void CreateShader::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize)
{
	D3D11_BUFFER_DESC bd;// �o�b�t�@�̐���
	ZeroMemory(&bd, sizeof(bd));// �o�b�t�@�̐�����������
	bd.Usage = D3D11_USAGE_DEFAULT;// �g�p�@
	bd.ByteWidth = bufferSize;// �o�b�t�@�̃T�C�Y
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// �o�C���h�t���O
	bd.CPUAccessFlags = 0;// CPU�A�N�Z�X�t���O
	m_pDevice->CreateBuffer(&bd, nullptr, &cBuffer);// �o�b�t�@���쐬
}
