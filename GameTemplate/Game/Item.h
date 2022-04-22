#pragma once
class Item : public IGameObject
{
public:
	enum ItemType
	{
		enNoItem,				// �A�C�e��������
		enRecord1,				// ���R�[�h1
		enRecord2,				// ���R�[�h2
		enHummer				// �n���}�[
	};

	Item() {};
	~Item() {};
	bool Start();
	void Update();
	void Draw(RenderContext& rc);
	/// <summary>
	/// �ۗ��A�C�e���g�Ƀ��R�[�h��ݒ�
	/// </summary>
	/// <param name="number">���R�[�h�̔ԍ�</param>
	void SetRecordToPendingItem(const int number)
	{
		if (number == 1) {
			m_pendingItem = enRecord1;
		}
		else if (number == 2) {
			m_pendingItem = enRecord2;
		}
	}
	/// <summary>
	/// �ۗ��A�C�e���g�Ƀn���}�[��ݒ�
	/// </summary>
	void SetHummerToPendingItem()
	{
		m_pendingItem = enHummer;
	}

	/// <summary>
	/// ���R�[�h1�����R�[�h2�������Ă��邩
	/// </summary>
	/// <returns></returns>
	const int GetHaveRecord() const 
	{
		for (int i = 0; i < 5; i++) {
			if (m_itemList[i] == 1) {
				return 1;
			}
			else if (m_itemList[i] == 2) {
				return 2;
			}
			if (i == 4) {
				return 0;
			}
		}
	}

	void IsUseRecord(const int recordNum)
	{
		for (int i = 0; i < 5; i++) {
			if (m_itemList[i] == recordNum) {
				m_itemList[i] = 0;
			}
		}
	}
private:
	/// <summary>
	/// �A�C�e���̔���
	/// </summary>
	void Identification();
private:
	int				m_itemList[5];					// �A�C�e�����X�g
	ItemType		m_pendingItem = enNoItem;		// �擾��̔��ʂ���Ă��Ȃ��ۗ��A�C�e��
};

