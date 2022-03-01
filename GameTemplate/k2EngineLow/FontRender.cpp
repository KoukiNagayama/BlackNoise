#include "k2EngineLowPreCompile.h"
#include "FontRender.h"
#include "graphics/font/Font.h"

namespace nsK2EngineLow {
	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr)
		{
			return;
		}

		if (m_initCheck == false) {
			// 各種パラメータをフォント用構造体へ代入
			swprintf_s(m_sfont.s_text, m_text);		// テキスト
			m_sfont.s_position.x = m_position.x;	// x座標
			m_sfont.s_position.y = m_position.y;	// y座標
			m_sfont.s_rotation = m_rotation;		// 回転
			m_sfont.s_color = m_color;				// 色
			m_sfont.s_scale = m_scale;				// 拡大
			m_sfont.s_pivot = m_pivot;				// 基点

			// 構造体を配列に代入
			m_fontArray.push_back(&m_sfont);
			m_initCheck = true;
		}

		RenderFont(rc);
	}

	void FontRender::RenderFont(RenderContext& rc)
	{
		Font font;
		int	 drawNum = 0;
		font.Begin(rc);
		for (auto& fontNum : m_fontArray) {
			
			font.Draw(m_fontArray[drawNum]->s_text,
				m_fontArray[drawNum]->s_position,
				m_fontArray[drawNum]->s_color,
				m_fontArray[drawNum]->s_rotation,
				m_fontArray[drawNum]->s_scale,
				m_fontArray[drawNum]->s_pivot
			);
			drawNum += 1;
		}
		font.End(rc);
	}
}
