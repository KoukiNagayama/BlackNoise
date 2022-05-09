#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �t�H���[�h�����_�����O
	/// </summary>
	class ForwardRendering
	{
	public:
		ForwardRendering() {};
		~ForwardRendering() {};
		/// <summary>
		/// �����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render(RenderContext& rc);
		/// <summary>
		/// �t�H���[�h�����_�����O�p�̃��f����ݒ�
		/// </summary>
		/// <param name="model">���f��</param>
		void SetModel(Model* model)
		{
			m_frModelArray.push_back(model);
		}
	private:
		std::vector<Model*>		m_frModelArray;			// �t�H���[�h�����_�����O�p���f��
	};
	extern ForwardRendering g_forwardRendering;
}
	

