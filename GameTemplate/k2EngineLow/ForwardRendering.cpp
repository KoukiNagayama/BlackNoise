#include "k2EngineLowPreCompile.h"
#include "ForwardRendering.h"

namespace nsK2EngineLow {
	void ForwardRendering::Render(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		for (auto& model : m_frModelArray)
		{
			model->Draw(rc);
		}
		m_frModelArray.clear();
	}

	ForwardRendering g_forwardRendering;
}

