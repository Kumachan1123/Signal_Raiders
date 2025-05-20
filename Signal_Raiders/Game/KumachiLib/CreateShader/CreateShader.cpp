/*
*	@file CreateShaders.cpp
*	@brief シェーダーを作成するシングルトンクラス
*	@details 板ポリゴンを描画する際に使用するシェーダーを作成する
*/
#include <pch.h>
#include "CreateShader.h"

using namespace KumachiLib;
// シングルトンインスタンスの初期化
std::unique_ptr<CreateShader> CreateShader::m_instance = nullptr;

/*
*	@brief シングルトンインスタンスを取得
*	@param なし
*	@return シングルトンインスタンス
*/
CreateShader* const CreateShader::GetInstance()
{
	if (m_instance == nullptr)// インスタンスがない場合
	{
		m_instance.reset(new CreateShader());// インスタンスを生成
	}
	return m_instance.get();// インスタンスを返す
}
/*
*	@brief コンストラクタ
*	@param なし
*	@return なし
*/
CreateShader::CreateShader()
	: m_device(nullptr)// デバイス
	, m_pIDE(nullptr)// 入力レイアウト
	, m_NumElements(0)// 要素数
	, m_pInputLayout(nullptr)// レイアウト
{
}
/*
*	@brief デストラクタ
*	@param なし
*	@return なし
*/
CreateShader::~CreateShader()
{
	m_pInputLayout.Reset();// インプットレイアウトの解放
	m_pIDE = nullptr;// 入力レイアウトの解放
	m_device = nullptr;// デバイスの解放
}
/*
*	@brief 初期化
*	@details シェーダーを作成するための初期化を行う
*	@param device デバイス
*	@param pIDE 入力レイアウト
*	@param NumElements 要素数
*	@param InputLayout レイアウト
*	@return なし
*/
void CreateShader::Initialize(ID3D11Device1* device, const D3D11_INPUT_ELEMENT_DESC* pIDE, UINT NumElements, Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout)
{
	m_device = device;// デバイス
	m_pIDE = pIDE;// 入力レイアウト
	m_NumElements = NumElements;// 要素数
	m_pInputLayout = InputLayout;// レイアウト
}
/*
*	@brief 頂点シェーダーを作成
*	@details 頂点シェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param vs 頂点シェーダーの格納先
* 	@return なし
*/
void CreateShader::CreateVertexShader(const wchar_t* fileName,
	Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs)
{

	BinaryFile VS = BinaryFile::LoadFile(fileName);// バイナリファイルを読み込む
	if (FAILED(m_device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, vs.ReleaseAndGetAddressOf())))//シェーダーを作成
	{
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);// エラー処理
		return;// 終了
	}
	m_device->CreateInputLayout(&m_pIDE[0], m_NumElements, VS.GetData(), VS.GetSize(), m_pInputLayout.GetAddressOf());// 入力レイアウトを作成
}
/*
*	@brief ピクセルシェーダーを作成
*	@details ピクセルシェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param ps ピクセルシェーダーの格納先
*	@return なし
*/
void CreateShader::CreatePixelShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps)
{
	BinaryFile PS = BinaryFile::LoadFile(fileName);// バイナリファイルを読み込む
	if (FAILED(m_device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, ps.ReleaseAndGetAddressOf())))// ピクセルシェーダ作成
	{
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);// エラー処理
		return;// 終了
	}
}
/*
*	@brief ジオメトリシェーダーを作成
*	@details ジオメトリシェーダーを作成する
*	@param fileName シェーダーのファイル名
*	@param gs ジオメトリシェーダーの格納先
*	@return なし
*/
void CreateShader::CreateGeometryShader(const wchar_t* fileName, Microsoft::WRL::ComPtr<ID3D11GeometryShader>& gs)
{
	BinaryFile GS = BinaryFile::LoadFile(fileName);// バイナリファイルを読み込む
	if (FAILED(m_device->CreateGeometryShader(GS.GetData(), GS.GetSize(), NULL, gs.ReleaseAndGetAddressOf())))// ジオメトリシェーダ作成
	{
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);// エラー処理
		return;// 終了
	}

}
/*
*	@brief コンスタントバッファを作成
*	@details コンスタントバッファを作成する
*	@param cBuffer コンスタントバッファの格納先
*	@param bufferSize バッファのサイズ
*	@return なし
*/
void CreateShader::CreateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& cBuffer, UINT bufferSize)
{
	D3D11_BUFFER_DESC bd;// バッファの説明
	ZeroMemory(&bd, sizeof(bd));// バッファの説明を初期化
	bd.Usage = D3D11_USAGE_DEFAULT;// 使用法
	bd.ByteWidth = bufferSize;// バッファのサイズ
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;// バインドフラグ
	bd.CPUAccessFlags = 0;// CPUアクセスフラグ
	m_device->CreateBuffer(&bd, nullptr, &cBuffer);// バッファを作成
}
