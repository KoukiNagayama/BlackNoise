#pragma once

namespace nsK2EngineLow {

	// 文字数サイズ
	const int MAX_TEXT_SIZE = 256;


	class FontRender
	{
	public:
		// フォント構造体
		struct SFont {
			wchar_t		s_text[MAX_TEXT_SIZE];
			Vector2		s_position;
			float		s_scale = 1.0f;
			Vector4		s_color = g_vec4White;;
			float		s_rotation = 0.0f;
			Vector2		s_pivot = Sprite::DEFAULT_PIVOT;
		};


		~FontRender() {};
		/// <summary>
		/// 表示する文字を設定
		/// </summary>
		/// <param name="text">表示する文字</param>
		void SetText(wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		/// <summary>
		/// 表示する文字を取得
		/// </summary>
		/// <returns>表示する文字</returns>
		const wchar_t* GetText() const
		{
			return m_text;
		}
		/// <summary>
		/// 座標を設定。zは0.0f
		/// </summary>
		/// <param name="x">X座標</param>
		/// <param name="y">Y座標</param>
		/// <param name="z">Z座標</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 大きさを設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを取得
		/// </summary>
		/// <returns>大きさ</returns>
		const float GetScale() const
		{
			return m_scale;
		}
		/// <summary>
		/// 色を設定
		/// </summary>
		/// <param name="r">赤</param>
		/// <param name="g">緑</param>
		/// <param name="b">青</param>
		/// <param name="a">透明度</param>
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r, g, b, a });
		}
		/// <summary>
		/// 色を設定
		/// </summary>
		/// <param name="color">色</param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		/// <summary>
		/// 色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector4& GetColor() const
		{
			return m_color;
		}
		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const float rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 回転を取得
		/// </summary>
		/// <returns>回転</returns>
		const float GetRotation() const
		{
			return m_rotation;
		}
		/// <summary>
		/// ピボットを設定
		/// </summary>
		/// <param name="x">ピボットのX座標</param>
		/// <param name="y">ピボットのY座標</param>
		void SetPivot(float x, float y)
		{
			SetPivot({ x, y });
		}
		/// <summary>
		/// ピボットを設定
		/// </summary>
		/// <param name="pivot">
		///	ピボット
		/// x = 0.5, y = 0.5で画像の中心が基点
		/// x = 0.0, y = 0.0で画像の左下
		/// x = 1.0, y = 1.0で画像の右上
		/// UnityのuGUIに準拠
		/// </param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得
		/// </summary>
		/// <returns>ピボット</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Draw(RenderContext& rc);
		/// <summary>
		/// 影のパラメータを設定
		/// </summary>
		/// <param name="isDrawShadow">影を描画する</param>
		/// <param name="shadowOffset">影を描画するときのピクセルのオフセット量</param>
		/// <param name="shadowColor">影の色</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
	private:
		/// <summary>
		/// 文字の描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderFont(RenderContext& rc);

	private:
		wchar_t		m_text[MAX_TEXT_SIZE];
		Vector3		m_position = Vector3::Zero;
		float		m_scale = 1.0f;
		Vector4		m_color = g_vec4White;;
		float		m_rotation = 0.0f;
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;
		Font		m_font;
		std::vector<SFont*> m_fontArray;
		int			m_initCount = 0;
		bool		m_initCheck = false;
		
		SFont		m_sfont;

	};
}


