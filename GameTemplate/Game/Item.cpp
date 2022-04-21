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
	// �A�C�e�����X�g�z��̋󂫋��
	int ArrayFreeSpace;
	// �A�C�e�����X�g�z��̋󂫋�Ԃ�������
	for (int i = 0; i < 5; i++) {
		if (m_itemList[i] == 0) {
			ArrayFreeSpace = i;
			break;
		}
		// �󂫋�Ԃ����݂��Ă��Ȃ��ꍇ
		if (i == 4 && m_itemList[i] != 0) {
			// �ۗ��A�C�e���͔j������
			m_pendingItem = enNoItem;
		}
	}
	// �󂫋�Ԃɓ���A�C�e��������
	switch (m_pendingItem)
	{
	// ���R�[�h1�̏ꍇ
	case enRecord1:
		m_itemList[ArrayFreeSpace] = enRecord1;
		break;
	// ���R�[�h2�̏ꍇ
	case enRecord2:
		m_itemList[ArrayFreeSpace] = enRecord2;
		break;
	default:
		break;
	}
	m_pendingItem = enNoItem;
}