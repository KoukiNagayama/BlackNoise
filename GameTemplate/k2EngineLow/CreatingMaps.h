#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �[�x�l�}�b�v (�{���[���h���W�p�}�b�v)
	/// </summary>
	class CreatingMaps
	{
	public:
		CreatingMaps() {};
		~CreatingMaps() {};
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �[�x�l�}�b�v�`��p�Ƀ��f����ݒ�
		/// </summary>
		void SetModel(Model* model)
		{
			m_modelArray.push_back(model);
		}
		/// <summary>
		/// �X�V
		/// </summary>
		void Update();
		/// <summary>
		/// �[�x�l�}�b�v�ւ̕`��
		/// </summary>
		void RenderToDepthValueMap(RenderContext& rc);
		/// <summary>
		/// �[�x�l�}�b�v�������ݗp�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�[�x�l�}�b�v�p�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetDepthValueMap()
		{
			return m_depthValueMap;
		}
		/// <summary>
		/// ���[���h���W�������ݗp�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>���[���h���W�������ݗp�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetWorldCoordinateMap()
		{
			return m_worldCoordinateMap;
		}
		/// <summary>
		/// �@���}�b�v�`��p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�@���}�b�v�`��p�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetNormalMap()
		{
			return m_normalMap;
		}
	private:
		/// <summary>
		/// �[�x�l�}�b�v�`��p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargetForDepthValueMap();
		/// <summary>
		/// ���[���h���W�L�^�p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargetForWorldCoordinateMap();
		/// <summary>
		/// �@���}�b�v�`��p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargetForNormalMap();
		/// <summary>
		/// �J�����̏�����
		/// </summary>
		void InitCamera();
	private:
		std::vector<Model*> m_modelArray;			// ���f���z��
		Camera				m_camera;				// �J����
		RenderTarget		m_depthValueMap;		// �[�x�l�}�b�v�̃����_�����O�^�[�Q�b�g
		RenderTarget		m_worldCoordinateMap;	// ���[���h���W�}�b�v
		RenderTarget		m_normalMap;			// �@���}�b�v
	};

	extern CreatingMaps g_creatingMaps;
}

