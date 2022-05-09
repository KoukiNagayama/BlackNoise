#include "k2EngineLowPreCompile.h"
#include "ForwardRendering.h"
#include "MainRenderTarget.h"

namespace nsK2EngineLow {
	void ForwardRendering::Render(RenderContext& rc)
	{
		rc.WaitUntilToPossibleSetRenderTarget(g_mainRenderTarget.GetMainRenderTarget());
		rc.SetRenderTarget(
			g_mainRenderTarget.GetMainRenderTarget().GetRTVCpuDescriptorHandle(),
			g_mainRenderTarget.GetMainRenderTarget().GetDSVCpuDescriptorHandle()
		);
		for (auto& model : m_frModelArray)
		{
			model->Draw(rc);
		}
		rc.WaitUntilFinishDrawingToRenderTarget(g_mainRenderTarget.GetMainRenderTarget());
		m_frModelArray.clear();
	}

	ForwardRendering g_forwardRendering;
}

