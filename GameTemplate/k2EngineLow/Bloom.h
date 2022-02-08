#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		Bloom() {};
		~Bloom() {};
		/// <summary>
		/// �u���[�����s�ɕK�v�ȑS�ėv�f�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitBloom(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�[�^�[�Q�b�g</param>
		void InitLuminanceRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o�p�̃X�v���C�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �K�E�V�A���u���[��������
		/// </summary>
		void InitBlur();
		/// <summary>
		/// �{�P�摜�����Z��������X�v���C�g��������
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void ChangeRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// �P�x���o
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void LuminanceExtraction(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// �u���[�����s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RunBlur(RenderContext& rc);
		/// <summary>
		/// �{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void CompositeImageToMainRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);


	private:
		RenderTarget		m_luminanceRenderTarget;
		GaussianBlur		m_gaussianBlur[4];
		Sprite				m_luminanceSprite;
		Sprite				m_finalSprite;
		Sprite				m_copyToFrameBufferSprite;

	};

	extern Bloom g_bloom;
}
 
