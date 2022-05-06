#pragma once
namespace nsK2EngineLow {
	class MainRenderTarget
	{
	public:
		MainRenderTarget() {};
		~MainRenderTarget() {};

		/// <summary>
		/// ������
		/// </summary>
		void Init();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g���擾
		/// </summary>
		/// <returns>���C�������_�����O�^�[�Q�b�g</returns>
		RenderTarget& GetMainRenderTarget() 
		{
			return m_mainRenderTarget;
		}
	private:
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[����p�̃X�v���C�g��������
		/// </summary>
		void InitCopyMainRtToFrameBufferSprite();
	private:
		RenderTarget		m_mainRenderTarget;					// ���C�������_�����O�^�[�Q�b�g		
		Sprite				m_copyMainRtToFrameBufferSprite;	// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[����p�̃X�v���C�g

	};
	extern MainRenderTarget g_mainRenderTarget;
}


