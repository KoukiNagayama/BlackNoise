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

		// 3次元ベクトルを2次元ベクトルに
		m_spPosition.x = m_position.x;
		m_spPosition.y = m_position.y;

		Font font;
		font.Begin(rc);
		font.Draw(
			m_text,				// テキスト
			m_spPosition,		// 座標
			m_color,			// 色
			m_rotation,			// 回転
			m_scale,			// 拡大
			m_pivot				// 基点
		);
		font.End(rc);
	}
}
