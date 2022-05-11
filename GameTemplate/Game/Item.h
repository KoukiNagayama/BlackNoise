#pragma once
class Item : public IGameObject
{
public:
	enum ItemType
	{
		enNoItem,				// アイテム未所持
		enRecord1,				// レコード1
		enRecord2,				// レコード2
		enCrowbar				// バール
	};

	Item() {};
	~Item() {};
	bool Start();
	void Update();
	void Draw(RenderContext& rc);
	/// <summary>
	/// 保留アイテム枠にレコードを設定
	/// </summary>
	/// <param name="number">レコードの番号</param>
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
	/// 保留アイテム枠にハンマーを設定
	/// </summary>
	void SetCrowbarToPendingItem()
	{
		m_pendingItem = enCrowbar;
	}

	/// <summary>
	/// レコード1かレコード2を持っているか
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
	/// <summary>
	/// レコードを使ったか
	/// </summary>
	/// <param name="recordNum">レコードの番号</param>
	void IsUseRecord(const int recordNum)
	{
		for (int i = 0; i < 5; i++) {
			if (m_itemList[i] == recordNum) {
				m_itemList[i] = 0;
			}
		}
	}

	/// <summary>
	/// バールを持っているか
	/// </summary>
	/// <returns>持っているとき->true</returns>
	const bool GetHaveCrowbar() const
	{
		for (int i = 0; i < 5; i++) {
			if (m_itemList[i] == enCrowbar) {
				return true;
			}
		}
	}
	/// <summary>
	/// アイテムを使い終わった
	/// </summary>
	/// <returns></returns>
	void ItemReset() 
	{
		for (int i = 0; i < 5; i++)
		{
			m_itemList[i] = 0;
		}
	}
private:
	/// <summary>
	/// アイテムの判別
	/// </summary>
	void Identification();
private:
	int				m_itemList[5];					// アイテムリスト
	ItemType		m_pendingItem = enNoItem;		// 取得後の判別されていない保留アイテム
};

