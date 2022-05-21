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
		/// <param name="isShadowReceiver">�e���󂯂邩�ǂ���</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadowCaster">�e���L���X�g���邩</param>
		/// <param name="outlineType">�֊s���̎�� 0:�ʏ� 1:�A�C�e�� 2:�G�l�~�[</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			bool isShadowReceiver = true,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowCaster = true,
			int maxInstance = 0,
			int outlineType = 0
		);
		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();
		/// <summary>
		/// �C���X�^���V���O�f�[�^�̍X�V�B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="filePath"></param>
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
		/// ���f�����擾
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// ���W�A��]�A�g���S�Đݒ�B
		/// </summary>
		/// <remark>
		/// �C���X�^���V���O�`�悪�L���̏ꍇ�́A���̐ݒ�͖�������܂��B
		/// </remark>
		/// <param name="pos">���W�B</param>
		/// <param name="rotation">��]�B</param>
		/// <param name="scale">�g��B</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
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
		/// <summary>
		/// �A�j���[�V�����C�x���g�̒ǉ�
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/// <summary>
		/// �A�j���[�V�����Đ��̑��x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">���l�̕������{�ɂ���B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
	private:
		/// <summary>
		/// ���f���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadowReceiver">�e���󂯂邩</param>
		void InitModel(const char* filePath, 
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReceiver = true
		);
		/// <summary>
		/// �֊s���`�惂�f���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitModelWithContours(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// �t�H���[�h�����_�����O�p�̃��f���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitForwardRenderingModel(const char* filePath,
			int outlineType,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ		
		);
		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃��f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitModelOnShadowMap(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// �[�x�l�}�b�v�`��p�̃��f����������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitModelOnDepthValueMap(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);


	private:
		Model						m_model;							// ���f��
		Skeleton					m_skeleton;							// �X�P���g��
		Animation					m_animation;						// �A�j���[�V����
		AnimationClip*				m_animationClips;					// �A�j���[�V�����N���b�v
		int							m_numAnimationClips;				// �A�j���[�V�����N���b�v�̐�
		Vector3						m_position = Vector3::Zero;			// ���W
		Vector3						m_scale = Vector3::One;				// �g�嗦
		Quaternion					m_rotation = Quaternion::Identity;	// ��]
		std::vector<Model>			m_shadowModelArray;					// �e���f���z��
		Model						m_shadowMapModel;					// �V���h�E�}�b�v�`��p���f��
		Model						m_shadowModel;						// �e���f��
		bool						m_isShadowCaster;					// �e���L���X�g���邩
		std::vector<Model>			m_modelArray;						// ���f���z��
		Model						m_depthValueMapModel;				// �[�x�l�}�b�v�`��p���f��
		int							m_numInstance = 0;					// �C���X�^���X�̐��B
		int							m_maxInstance = 1;					// �ő�C���X�^���X���B
		int							m_fixNumInstanceOnFrame = 0;		// ���̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B�B
		bool						m_isEnableInstancingDraw = false;	// �C���X�^���V���O�`�悪�L���H
		std::unique_ptr<Matrix[]>	m_worldMatrixArray;					// ���[���h�s��̔z��B
		StructuredBuffer			m_worldMatrixArraySB;				// ���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
		int							m_outlineType;						// �֊s���̎��
		float						m_animationSpeed = 1.0f;			// �A�j���[�V�����̑����̔{��
	};
}


