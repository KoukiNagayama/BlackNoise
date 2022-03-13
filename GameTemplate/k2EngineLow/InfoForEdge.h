#pragma once
namespace nsK2EngineLow {
	// 音源の最大個数
	const int SOUND_MAX_SIZE = 100;

	/// <summary>
	/// 音源データ構造体
	/// </summary>
	struct SoundSourceData
	{
		Vector3		s_position = Vector3::Zero;		// 音源の位置
		int			s_isSound = 0;		// 音源は鳴っているか
		float		s_range = 0.0f;		// 音源の影響範囲
		Vector3		pad;
	};
	/// <summary>
	/// 音源データの定数バッファ用構造体
	/// </summary>
	struct CBSoundSource
	{
		SoundSourceData s_soundSourceData[SOUND_MAX_SIZE];		// 音源データ配列
		int				s_numSoundSource = 0;					// 音源の個数
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
		void InitForSound(int number, Vector3 position, float range, int isSound);
		/// <summary>
		/// 音源の座標と影響範囲と音が鳴っているかを設定。                    
		/// 複数の項目で値の変動がある場合は使用する。
		/// </summary>
		/// <param name="number">配列の番号</param>
		/// <param name="position">音源の座標</param>
		/// <param name="range">音源の影響範囲</param>
		/// <param name="isSound">音が鳴っているか</param>
		void SetInfoForSound(int number, Vector3 position, float range, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
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
		/// <param name="isTrue">音源が鳴っているか</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		}
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
	};
	extern InfoForEdge	 g_infoForEdge;
}


