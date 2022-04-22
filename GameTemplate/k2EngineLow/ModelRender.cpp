#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Light.h"
#include "Shadow.h"
#include "CreatingMaps.h"
#include "sound/SoundSource.h"
#include "InfoForEdge.h"

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
		EnModelUpAxis enModelUpAxis,
		bool isShadowCaster,
		int maxInstance
	)
	{
		// �A�j���[�V��������(�A�j���[�V�����̗L������̂���)
		m_animationClips = animationClips;
		// �X�P���g���̏�����
		InitSkeleton(filePath);
		// �A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips);
		/*if (isShadowCaster == true) {
			// �V���h�E�}�b�v�`��p���f���̏�����
			InitModelOnShadowMap(filePath);
		}*/
		// �[�x�l�}�b�v�`��p���f���̏�����
		InitModelOnDepthValueMap(filePath);
		// ���f���̏�����
		//InitModel(filePath, enModelUpAxis, isShadowReceiver);
		InitModelWithContours(filePath, enModelUpAxis);
		// �e���L���X�g���邩
		m_isShadowCaster = isShadowCaster;
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

	/*void ModelRender::InitModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver
	)
	{
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;

		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		if (isShadowReceiver == false) {
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
			//�쐬�����������f�[�^�����ƂɃ��f��������������B
			m_model.Init(modelInitData);
		}
		else {
			modelInitData.m_fxFilePath = "Assets/shader/shadowReciever.fx";
			modelInitData.m_modelUpAxis = enModelUpAxis;
			// �V���h�E�}�b�v���g��SRV�ɐݒ肷��B
			modelInitData.m_expandShaderResoruceView[0] = &g_shadow.GetShadowMap().GetRenderTargetTexture();
			// ���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��
			modelInitData.m_expandConstantBuffer = (void*)&g_shadow.GetLightCamera().GetViewProjectionMatrix();
			modelInitData.m_expandConstantBufferSize = sizeof(g_shadow.GetLightCamera().GetViewProjectionMatrix());
			m_model.Init(modelInitData);
		}

		
		
	}*/

	void ModelRender::InitModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver
	)
	{
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		//���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		//���C�g�̏���悷�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^���邽�߂�
		//���f���̏��������Ƃ��ēn���B
		modelInitData.m_expandConstantBuffer = g_light.GetLightData();
		modelInitData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
		//�쐬�����������f�[�^�����ƂɃ��f��������������B
		m_model.Init(modelInitData);
	}

	void ModelRender::InitModelWithContours(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData modelInitData;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		modelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		//modelInitData.m_fxFilePath = "Assets/shader/edgeExtraction.fx";
		modelInitData.m_fxFilePath = "Assets/shader/edge.fx";
		// �J���[�o�b�t�@�̃t�H�[�}�b�g���w�肷��B
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// �e��}�b�v���g��SRV�ɐݒ肷��B
		modelInitData.m_expandShaderResoruceView[0] = &g_creatingMaps.GetDepthValueMap().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[1] = &g_creatingMaps.GetWorldCoordinateMap().GetRenderTargetTexture();
		modelInitData.m_expandShaderResoruceView[2] = &g_creatingMaps.GetNormalMap().GetRenderTargetTexture();
		// ���f���̏�������w�肷��B
		modelInitData.m_modelUpAxis = enModelUpAxis;
		//
		modelInitData.m_expandConstantBuffer = (void *)&g_infoForEdge.GetSoundSourceData();
		modelInitData.m_expandConstantBufferSize = sizeof(g_infoForEdge.GetSoundSourceData());
		// �쐬�����������f�[�^�����ƂɃ��f��������������B
		m_model.Init(modelInitData);
	}
	
	void ModelRender::InitModelOnShadowMap(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData shadowModelInitData;
		//���f���̏�������w�肷��B
		shadowModelInitData.m_modelUpAxis = enModelUpAxis;
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		shadowModelInitData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		// �������f�[�^�����ƂɃ��f����������
		m_shadowMapModel.Init(shadowModelInitData);

	}

	void ModelRender::InitModelOnDepthValueMap(const char* filePath,
		EnModelUpAxis enModelUpAxis)
	{
		ModelInitData modelInitData;
		// ���f���̏�������w�肷��
		modelInitData.m_modelUpAxis = enModelUpAxis;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_fxFilePath = "Assets/shader/depthValueMap.fx";
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��
		modelInitData.m_tkmFilePath = filePath;
		// �������f�[�^�����ƂɃ��f����������
		m_depthValueMapModel.Init(modelInitData);
	}

	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "�C���X�^���X�̐����������ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_model.CalcWorldMatrix(pos, rot, scale);

		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			m_animation.Progress(g_gameTime->GetFrameDeltaTime());
		}
		m_numInstance++;
	}
	
	void ModelRender::Update()
	{
		if (m_skeleton.IsInited()) {
			// �X�P���g�����X�V
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		// �A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());

		// �ʏ탌���_�����O�p���f���̃��[���h�s����X�V
		m_model.UpdateWorldMatrix(
			m_position, 
			m_rotation,
			m_scale
		);

		// �V���h�E�}�b�v�`��p���f���̃��[���h�s����X�V
		m_shadowMapModel.UpdateWorldMatrix(
			m_position,
			m_rotation,
			m_scale
		);

		// �[�x�l�}�b�v�`��p���f���̃��[���h�s����X�V
		m_depthValueMapModel.UpdateWorldMatrix(
			m_position,
			m_rotation,
			m_scale
		);

		if (m_isShadowCaster == true) {
			// ���f���̏��̎󂯓n��
			g_shadow.SetShadowModel(&m_shadowMapModel);
		}

		g_creatingMaps.SetModel(&m_depthValueMapModel);

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}

