#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		Shadow();
		~Shadow() {};
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void Init(const char* filePath);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render(RenderContext& rc);
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
		void SetShadowModel(Model* model)
		{
			m_shadowModelArray.push_back(model); 
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
		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃��f���̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		void InitModelForShadowMap(const char* filePath);

	private:
		RenderTarget		m_shadowMap;			// �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
		Camera				m_lightCamera;			// ���C�g�J����
		std::vector<Model*>  m_shadowModelArray;		// �e���f��
		//Sprite				m_copyToFrameBufferSprite;           
	};
	extern Shadow g_shadow;
}

