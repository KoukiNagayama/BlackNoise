#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow {
	void Bloom::InitLuminanceRenderTarget(RenderTarget& mainRenderTarget)
	{
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 
			DXGI_FORMAT_D32_FLOAT
		);
	}
	void Bloom::InitSprite(RenderTarget& mainRenderTarget)
	{
		SpriteInitData luminanceSpriteInitData;
		//�P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
		//���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ����B
		luminanceSpriteInitData.m_width = mainRenderTarget.GetWidth();
		luminanceSpriteInitData.m_height = mainRenderTarget.GetHeight();
		//�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�B
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_luminanceSprite.Init(luminanceSpriteInitData);

		InitBlur();

	}

	void Bloom::InitBlur()
	{
		// gaussianBlur[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		// gaussianBlur[1]��gaussianBlur[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		// gaussianBlur[2]��gaussianBlur[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		// gaussianBlur[3]��gaussianBlur[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}

	void Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
	{
		SpriteInitData finalspriteInitData;
		// �{�P�e�N�X�`����4���w�肷��
		finalspriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalspriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalspriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalspriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		finalspriteInitData.m_width = mainRenderTarget.GetWidth();
		finalspriteInitData.m_height = mainRenderTarget.GetHeight();
		// �ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
		// 2D�p�̃V�F�[�_�[���g�p����
		finalspriteInitData.m_fxFilePath = "Assets/shader/bloom.fx";
		finalspriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		// �������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���
		finalspriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		finalspriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_finalSprite.Init(finalspriteInitData);

		SpriteInitData spriteInitData;
		//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = mainRenderTarget.GetWidth();
		spriteInitData.m_height = mainRenderTarget.GetHeight();
		//���m�N���p�̃V�F�[�_�[���w�肷��B
		spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
		//�������I�u�W�F�N�g���g���āA�X�v���C�g������������B
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	void Bloom::Render1(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		rc.ClearRenderTargetView(mainRenderTarget);
	}

	void Bloom::Render2(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
		// �P�x���o
	   // �P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		// �P�x���o���s��
		m_luminanceSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

		// �K�E�V�A���u���[��4����s����
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

		// 4���̃{�P�摜���������ă��C�������_�����O�^�[�Q�b�g�ɉ��Z����
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// �ŏI����
		m_finalSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);
	}
	Bloom g_bloom;
}