#pragma once
namespace nsK2EngineLow {
	// �����̍ő��
	const int SOUND_MAX_SIZE = 100;

	/// <summary>
	/// �����f�[�^�\����
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
	/// �����f�[�^�̒萔�o�b�t�@�p�\����
	/// </summary>
	struct CBSoundSource
	{
		SoundSourceData s_soundSourceData[SOUND_MAX_SIZE];		// �����f�[�^�z��
		int				s_numSoundSource = 0;					// �����̌�
		float			s_colorRatio = 0.0f;
	};


	/// <summary>
	/// �A�E�g���C���`��p�����f�[�^�N���X�B
	/// SoundSource�N���X���g�p����ۂɂ�������g�p����B
	/// </summary>
	class InfoForEdge
	{
	public:
		InfoForEdge() {};
		~InfoForEdge() {};
		/// <summary>
		/// �����f�[�^��������
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="position">�����̍��W</param>
		/// <param name="range">�����̉e���͈�</param>
		/// <param name="isSound">���߂��特�����Ă��邩</param>
		/// <param name="rate">�����̉e���� 0.00f�`1.00f</param>
		void InitForSound(int number, Vector3 position, float range, int isSound, float rate, int color = 0);
		/// <summary>
		/// �����̍��W�Ɖe���͈͂Ɖ������Ă��邩��ݒ�B                    
		/// �����̍��ڂŒl�̕ϓ�������ꍇ�͎g�p����B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="position">�����̍��W</param>
		/// <param name="range">�����̉e���͈�</param>
		/// <param name="isSound">�������Ă��邩</param>
		/// <param name="rate">�����̉e���� 0.00f�`1.00f</param>
		void SetInfoForSound(int number, Vector3 position, float range, int isSound, float rate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
			m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		}
		/// <summary>
		/// �����̍��W��ݒ�B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="position">�����̍��W</param>
		void SetPosition(int number, Vector3 position)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
		}
		/// <summary>
		/// �����̉e���͈͂�ݒ�B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="range">�����̉e���͈�</param>
		void SetRange(int number, float range)
		{
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
		}
		/// <summary>
		/// ���������Ă��邩�ǂ�����ݒ�
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="isSound">���������Ă��邩</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		}
		/// <summary>
		/// �����̉e����
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="rate">�e����</param>
		void SetRate(int number, float rate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		}

		/*void SetCurrentRate(int number, float currentRate)
		{
			m_cbSoundSource.s_soundSourceData[number].s_currentRate = currentRate;
		}*/
		/// <summary>
		/// �X�V
		/// </summary>
		void Update();
		/// <summary>
		///	�����f�[�^�p�萔�o�b�t�@�\���̂��擾
		/// </summary>
		/// <returns>�����f�[�^�p�萔�o�b�t�@�\����</returns>
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


