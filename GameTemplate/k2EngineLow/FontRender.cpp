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


		m_spPosition.x = m_position.x;
		m_spPosition.y = m_position.y;

		Font font;
		font.Begin(rc);
		font.Draw(
			m_text,
			m_spPosition,
			m_color,
			m_rotation,
			m_scale,
			m_pivot
		);
		font.End(rc);
	}
}
