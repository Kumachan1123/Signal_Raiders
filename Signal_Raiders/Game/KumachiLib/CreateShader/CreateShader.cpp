#include <pch.h>
#include "CreateShader.h"
#include <utility>
using namespace KumachiLib;

std::unique_ptr<CreateShader> CreateShader::m_instance = nullptr;

CreateShader* const CreateShader::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new CreateShader());
	}
	return m_instance.get();
}

CreateShader::CreateShader()
	: m_device(nullptr)
	, m_pIDE(nullptr)
	, m_NumElements(0)
	, m_pInputLayout(nullptr)
{
}

CreateShader::~CreateShader()
{
	// �C���v�b�g���C�A�E�g�̉��
	m_pInputLayout.Reset();
	m_pIDE = nullptr;

}

// ������
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	m_device = device;
	m_pIDE = pIDE;
	m_NumElements = NumElements;
	m_pInputLayout = InputLayout;
}

// ���_�V�F�[�_�[���쐬
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	BinaryFile VS = BinaryFile::LoadFile(fileName);
	//	���_�V�F�[�_�쐬
	if (FAILED(m_device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	m_device->CreateInputLayout(&m_pIDE[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());
}



// �s�N�Z���V�F�[�_�[���쐬
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	BinaryFile PS = BinaryFile::LoadFile(fileName);
	//	�s�N�Z���V�F�[�_�쐬
	if (FAILED(m_device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

// �W�I���g���V�F�[�_�[���쐬
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	BinaryFile GS = BinaryFile::LoadFile(fileName);
	//	�W�I���g���V�F�[�_�쐬
	if (FAILED(m_device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

}

