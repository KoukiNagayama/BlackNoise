#pragma once
namespace nsK2EngineLow {
	class DepthValueMap
	{
	public:
		DepthValueMap() {};
		~DepthValueMap() {};
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

	private:
		/// <summary>
		/// �[�x�l�}�b�v�`��p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargetForDepthValueMap();
		/// <summary>
		/// �J�����̏�����
		/// </summary>
		void InitCamera();
	private:
		std::vector<Model*> m_modelArray;		// ���f���z��
		Camera				m_camera;			// �J����
		RenderTarget		m_depthValueMap;	// �[�x�l�}�b�v�̃����_�����O�^�[�Q�b�g
	};

	extern DepthValueMap g_depthValueMap;
}

