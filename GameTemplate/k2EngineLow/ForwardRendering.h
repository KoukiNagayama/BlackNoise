#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// フォワードレンダリング
	/// </summary>
	class ForwardRendering
	{
	public:
		ForwardRendering() {};
		~ForwardRendering() {};
		/// <summary>
		/// レンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render(RenderContext& rc);
		/// <summary>
		/// フォワードレンダリング用のモデルを設定
		/// </summary>
		/// <param name="model">モデル</param>
		void SetModel(Model* model)
		{
			m_frModelArray.push_back(model);
		}
	private:
		std::vector<Model*>		m_frModelArray;			// フォワードレンダリング用モデル
	};
	extern ForwardRendering g_forwardRendering;
}
	

