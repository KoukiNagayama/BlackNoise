#pragma once
namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0);
		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();
		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);
		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// �g�嗦��ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

	private:
		/// <summary>
		/// ���f���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void InitModel(const char* filePath);
		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);


		Model				m_model;							// ���f��
		Skeleton			m_skeleton;							// �X�P���g��
		ModelInitData		m_initData;							// ���f�������������邽�߂̏���ݒ�B
		Animation			m_animation;						// �A�j���[�V����
		AnimationClip*		m_animationClips;					// �A�j���[�V�����N���b�v
		int					m_numAnimationClips;				// �A�j���[�V�����N���b�v�̐�
		Vector3				m_position = Vector3::Zero;			// ���W
		Vector3				m_scale = Vector3::One;				// �g�嗦
		Quaternion			m_rotation = Quaternion::Identity;	// ��]
	};
}

