#include "stdafx.h"
#include "Item.h"

bool Item::Start()
{
	for (int i = 0; i < 5; i++)
	{
		m_itemList[i] = 0;
	}
	return true;
}

void Item::Update()
{
	if (m_pendingItem == enNoItem) {
		return;
	}

	Identification();
}

void Item::Identification()
{	
	// アイテムリスト配列の空き空間
	int ArrayFreeSpace;
	// アイテムリスト配列の空き空間を見つける
	for (int i = 0; i < 5; i++) {
		if (m_itemList[i] == 0) {
			ArrayFreeSpace = i;
			break;
		}
		// 空き空間が存在していない場合
		if (i == 4 && m_itemList[i] != 0) {
			// 保留アイテムは破棄する
			m_pendingItem = enNoItem;
		}
	}
	// 空き空間に入手アイテムを入れる
	switch (m_pendingItem)
	{
	// レコード1の場合
	case enRecord1:
		m_itemList[ArrayFreeSpace] = enRecord1;
		break;
	// レコード2の場合
	case enRecord2:
		m_itemList[ArrayFreeSpace] = enRecord2;
		break;
	default:
		break;
	}
	m_pendingItem = enNoItem;
}