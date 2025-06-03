/*
*	@file BinaryFile.cpp
*	@brief ファイル読み込みクラス
*/
#include <pch.h>
#include "BinaryFile.h"
/*
*	@brief コンストラクタ
*	@details バイナリファイルクラスのインスタンスを作成
*	@param なし
*	@return なし
*/
KumachiLib::BinaryFile::BinaryFile() { m_size = 0; }// サイズを0に初期化
/*
*	@brief ムーブコンストラクタ
*	@details ムーブコンストラクタ
*	@param in ムーブ元のバイナリファイル
*	@return なし
*/
KumachiLib::BinaryFile::BinaryFile(BinaryFile&& in) noexcept
	: m_pData{}// ムーブ元のデータを初期化
	, m_size{ 0 }// サイズを0に初期化
{
	// ムーブ元のデータを移動
	m_pData = std::move(in.m_pData);
	// サイズをコピー
	m_size = in.m_size;
}

/*
*	@brief ファイル名を指定してロード
*	@details ファイル名を指定してバイナリファイルを読み込む
*	@param fileName ファイル名
*	@return BinaryFile 読み込んだファイル
*/
KumachiLib::BinaryFile KumachiLib::BinaryFile::LoadFile(const wchar_t* fileName)
{
	// バイナリファイルクラスのインスタンスを作成
	BinaryFile bin;
	// 入力ファイルストリームを作成
	std::ifstream ifs;
	// ファイルオープン
	ifs.open(fileName, std::ios::in | std::ios::binary);
	// 読み込み失敗時、強制終了
	assert(ifs);
	// ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	// ファイルの終端位置を取得
	std::streamoff eofPos = ifs.tellg();
	// ストリームの状態をクリア
	ifs.clear();
	// ファイルの先頭位置を取得
	ifs.seekg(0, std::fstream::beg);
	// ファイルの先頭位置を取得
	std::streamoff begPos = ifs.tellg();
	// ファイルサイズを計算
	bin.m_size = (unsigned int)(eofPos - begPos);
	// 読み込むためのメモリを確保
	bin.m_pData.reset(new char[bin.m_size]);
	// ファイル先頭からバッファへコピー
	ifs.read(bin.m_pData.get(), bin.m_size);
	// ファイルクローズ
	ifs.close();
	// バイナリファイルを返す
	return bin;
}
