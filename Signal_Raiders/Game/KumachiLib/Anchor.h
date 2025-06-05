/*
*	@file Anchor.h
*	@brief UIのアンカーポイントの列挙クラス
*/
#pragma once
#ifndef ANCHOR_DEFINED
#define ANCHOR_DEFINED
namespace KumachiLib
{
	//UIのアンカーポイントの列挙数
	enum ANCHOR
	{
		TOP_LEFT = 0,// 左上
		TOP_CENTER,// 中央上
		TOP_RIGHT,// 右上

		MIDDLE_LEFT,// 左中央
		MIDDLE_CENTER,// 中央
		MIDDLE_RIGHT,// 右中央

		BOTTOM_LEFT,// 左下
		BOTTOM_CENTER,// 中央下
		BOTTOM_RIGHT,// 右下
	};
}
#endif // ANCHOR_DEFINED
