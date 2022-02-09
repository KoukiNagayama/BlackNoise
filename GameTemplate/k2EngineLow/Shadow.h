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
		/// <param name="filePath">�t�@�C���p�X</param>
		void Init(const char* filePath);
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
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render(RenderContext& rc);
	private:
		RenderTarget		m_shadowMap;
		Camera				m_lightCamera;
		Model				m_shadowModel;

	};
	extern Shadow g_shadow;
}

