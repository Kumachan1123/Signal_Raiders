/*
*	@file BinaryFile.cpp
*	@brief ファイル読み込みクラス
*/
#include "pch.h"
#include "BinaryFile.h"
/*
*	@brief ファイル名を指定してロード
*	@details ファイル名を指定してバイナリファイルを読み込む
*	@param fileName ファイル名
*	@return BinaryFile 読み込んだファイル
*/
KumachiLib::BinaryFile KumachiLib::BinaryFile::LoadFile(const wchar_t* fileName)
{
	BinaryFile bin;// バイナリファイルクラスのインスタンスを作成
	std::ifstream ifs;// 入力ファイルストリームを作成
	ifs.open(fileName, std::ios::in | std::ios::binary);// ファイルオープン
	assert(ifs);// 読み込み失敗時、強制終了
	ifs.seekg(0, std::fstream::end);// ファイルサイズを取得
	std::streamoff eofPos = ifs.tellg();// ファイルの終端位置を取得
	ifs.clear();// ストリームの状態をクリア
	ifs.seekg(0, std::fstream::beg);// ファイルの先頭位置を取得
	std::streamoff begPos = ifs.tellg();// ファイルの先頭位置を取得
	bin.m_size = (unsigned int)(eofPos - begPos);// ファイルサイズを計算
	bin.m_data.reset(new char[bin.m_size]);// 読み込むためのメモリを確保
	ifs.read(bin.m_data.get(), bin.m_size);// ファイル先頭からバッファへコピー
	ifs.close();// ファイルクローズ
	return std::move(bin);// バイナリファイルを返す
}
/*
*	@brief コンストラクタ
*	@details バイナリファイルクラスのインスタンスを作成
*	@param なし
*	@return なし
*/
KumachiLib::BinaryFile::BinaryFile()
{
	m_size = 0;// サイズを0に初期化
}
/*
*	@brief ムーブコンストラクタ
*	@details ムーブコンストラクタ
*	@param in ムーブ元のバイナリファイル
*	@return なし
*/
KumachiLib::BinaryFile::BinaryFile(BinaryFile&& in)
{
	m_data = std::move(in.m_data);// ムーブ元のデータを移動
	m_size = in.m_size;// サイズをコピー
}
