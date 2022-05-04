#pragma once
namespace nsK2EngineLow {
	// 音源の最大個数
	const int SOUND_MAX_SIZE = 100;

	/// <summary>
	/// 音源データ構造体
	/// </summary>
	struct SoundSourceData
	{
		Vector3		s_position = Vector3::Zero;
		int			s_isSound = 0;
		float		s_range = 0.0f;
		float		s_rate = 0.0f;
		int			s_color = 0;
		float		pad;
	};
	/// <summary>
	/// 音源データの定数バッファ用構造体
	/// </summary>
	struct CBSoundSource
	{
		SoundSourceData s_soundSourceData[SOUND_MAX_SIZE];		// 音源データ配列
		int				s_numSoundSource = 0;					// 音源の個数
		float			s_colorRatio = 0.0f;
	};


	/// <summary>
	/// アウトライン描画用音源データクラス。
	/// SoundSourceクラスを使用する際にこちらも使用する。
	/// </summary>
	class InfoForEdge
	{
	public:
		InfoForEdge() {};
		~InfoForEdge() {};
		/// <summary>
		/// 音源データを初期化
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="position">音源の座標</param>
		/// <param name="range">音源の影響範囲</param>
		/// <param name="isSound">初めから音が鳴っているか</param>
		/// <param name="rate">音源の影響率 0.00f～1.00f</param>
		void InitForSound(int number, Vector3 position, float range, int isSound, float rate, int color = 0);
		/// <summary>
		/// 音源の座標と影響範囲と音が鳴っているかを設定。                    
		/// 複数の項目で値の変動がある場合は使用する。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="position">音源の座標</param>
		/// <param name="range">音源の影響範囲</param>
		/// <param name="isSound">音が鳴っているか</param>
		/// <param name="rate">音源の影響率 0.00f～1.00f</param>
		void SetInfoForSound(int number, Vector3 position, float range, int isSound, float rate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
			m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		}
		/// <summary>
		/// 音源の座標を設定。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="position">音源の座標</param>
		void SetPosition(int number, Vector3 position)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
		}
		/// <summary>
		/// 音源の影響範囲を設定。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="range">音源の影響範囲</param>
		void SetRange(int number, float range)
		{
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
		}
		/// <summary>
		/// 音源が鳴っているかどうかを設定
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="isSound">音源が鳴っているか</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		}
		/// <summary>
		/// 音源の影響率
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="rate">影響率</param>
		void SetRate(int number, float rate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		}

		/*void SetCurrentRate(int number, float currentRate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_currentRate = currentRate;
		}*/
		/// <summary>
		/// 更新
		/// </summary>
		void Update();
		/// <summary>
		///	音源データ用定数バッファ構造体を取得
		/// </summary>
		/// <returns>音源データ用定数バッファ構造体</returns>
		CBSoundSource& GetSoundSourceData()
		{
			return m_cbSoundSource;
		}
	private:
		CBSoundSource   m_cbSoundSource;
		float			m_colorRatio = 1.0f;
		int				m_sizeChenge = 0;
	};
	extern InfoForEdge	 g_infoForEdge;
}


