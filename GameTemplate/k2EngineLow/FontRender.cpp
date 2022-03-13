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

		// 3�����x�N�g����2�����x�N�g����
		m_spPosition.x = m_position.x;
		m_spPosition.y = m_position.y;

		Font font;
		font.Begin(rc);
		font.Draw(
			m_text,				// �e�L�X�g
			m_spPosition,		// ���W
			m_color,			// �F
			m_rotation,			// ��]
			m_scale,			// �g��
			m_pivot				// ��_
		);
		font.End(rc);
	}
}
