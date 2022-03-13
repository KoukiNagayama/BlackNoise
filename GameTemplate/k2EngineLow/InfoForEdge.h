#pragma once
namespace nsK2EngineLow {
	// �����̍ő��
	const int SOUND_MAX_SIZE = 100;

	/// <summary>
	/// �����f�[�^�\����
	/// </summary>
	struct SoundSourceData
	{
		Vector3		s_position = Vector3::Zero;		// �����̈ʒu
		int			s_isSound = 0;		// �����͖��Ă��邩
		float		s_range = 0.0f;		// �����̉e���͈�
		Vector3		pad;
	};
	/// <summary>
	/// �����f�[�^�̒萔�o�b�t�@�p�\����
	/// </summary>
	struct CBSoundSource
	{
		SoundSourceData s_soundSourceData[SOUND_MAX_SIZE];		// �����f�[�^�z��
		int				s_numSoundSource = 0;					// �����̌�
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
		void InitForSound(int number, Vector3 position, float range, int isSound);
		/// <summary>
		/// �����̍��W�Ɖe���͈͂Ɖ������Ă��邩��ݒ�B                    
		/// �����̍��ڂŒl�̕ϓ�������ꍇ�͎g�p����B
		/// </summary>
		/// <param name="number">�z��̔ԍ�</param>
		/// <param name="position">�����̍��W</param>
		/// <param name="range">�����̉e���͈�</param>
		/// <param name="isSound">�������Ă��邩</param>
		void SetInfoForSound(int number, Vector3 position, float range, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_position = position;
			m_cbSoundSource.s_soundSourceData[number].s_range = range;
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
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
		/// <param name="isTrue">���������Ă��邩</param>
		void SetIsSound(int number, int isSound)
		{
			m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		}
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
	};
	extern InfoForEdge	 g_infoForEdge;
}


