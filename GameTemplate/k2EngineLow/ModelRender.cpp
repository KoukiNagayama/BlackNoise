#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Light.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		bool isShadowReceiver,
		EnModelUpAxis enModelUpAxis
	)
	{
		// �A�j���[�V��������(�A�j���[�V�����̗L������̂���)
		m_animationClips = animationClips;
		// �X�P���g���̏�����
		InitSkeleton(filePath);
		// �A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips);

		g_shadow.Init(filePath);
		InitModel(filePath,enModelUpAxis);
		g_shadow.SetShadowModel(&m_model);
		auto& renderContext = g_graphicsEngine->GetRenderContext();
		g_shadow.Render(renderContext);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		m_animationClips = animationClips;
		
		if (m_animationClips != nullptr) {
			// �A�j���[�V������������
			m_animation.Init(
				m_skeleton,					// �A�j���[�V�����𗬂����ރX�P���g��
				m_animationClips,			// �A�j���[�V�����N���b�v
				m_numAnimationClips			// �A�j���[�V�����̐�
			);
		}
	}

	void ModelRender::InitModel(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";


		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//���f���̏�������w�肷��B
			modelInitData.m_modelUpAxis = enModelUpAxis;
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}


		//���C�g�̏���悷�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^���邽�߂�
		//���f���̏��������Ƃ��ēn���B
		modelInitData.m_expandConstantBuffer = g_light.GetLightData();
		modelInitData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());

	/*	// �V���h�E�}�b�v���g��SRV�ɐݒ肷��B
		modelInitData.m_expandShaderResoruceView[0] = &g_shadow.GetShadowMap().GetRenderTargetTexture();
		// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��
		modelInitData.m_expandConstantBuffer = (void*)&g_shadow.GetLightCamera().GetViewProjectionMatrix();
		modelInitData.m_expandConstantBufferSize = sizeof(&g_shadow.GetLightCamera().GetViewProjectionMatrix());
	*/

		//�쐬�����������f�[�^�����ƂɃ��f��������������B
		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelOnShadowMap(const char* filePath)
	{
		ModelInitData modelInitData;
		//���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxisY;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_fxFilePath = "Assets/shader/shadowReciever.fx";
		// �V���h�E�}�b�v���g��SRV�ɐݒ肷��B
		modelInitData.m_expandShaderResoruceView[0] = &g_shadow.GetShadowMap().GetRenderTargetTexture();
		// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��
		modelInitData.m_expandConstantBuffer = (void*)&g_shadow.GetLightCamera().GetViewProjectionMatrix();
		modelInitData.m_expandConstantBufferSize = sizeof(&g_shadow.GetLightCamera().GetViewProjectionMatrix());

		m_model.Init(modelInitData);
	}

	
	void ModelRender::Update()
	{
		if (m_skeleton.IsInited()) {
			// �X�P���g�����X�V
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		// �A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
		// ���[���h�s����X�V
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);

		
	}
}

