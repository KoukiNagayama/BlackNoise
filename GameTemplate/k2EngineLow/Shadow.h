#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		Shadow() {};
		~Shadow() {};
		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// ���C�g�J�������擾
		/// </summary>
		/// <returns>���C�g�J����</returns>
		Camera& GetLightCamera() 
		{
			return m_lightCamera;
		}
		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}
		/// <summary>
		/// �V���h�E�}�b�v�`��p�Ƀ��f����ݒ�
		/// </summary>
		/// <param name="model">�V���h�E�}�b�v�`��p���f��</param>
		void SetShadowModel(Model* model)
		{
			//m_shadowModelArray.push_back(model);
			m_shadowModel = model;
		}
	private:
		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargetForShadowMap();
		/// <summary>
		/// �e�`��p�̃��C�g�J�����̏�����
		/// </summary>
		void InitLightCamera();


	private:
		RenderTarget		m_shadowMap;			// �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
		Camera				m_lightCamera;			// ���C�g�J����
		std::vector<Model*>  m_shadowModelArray;		// �e���f�� 
		Model*				m_shadowModel;
	};
	extern Shadow g_shadow;
}

