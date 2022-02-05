#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Light.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips)
	{
		// �A�j���[�V��������(�A�j���[�V�����̗L������̂���)
		m_animationClips = animationClips;
		// �X�P���g���̏�����
		InitSkeleton(filePath);
		// ���f���̏�����
		InitModel(filePath);
		// �A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips);


	}

	void ModelRender::InitModel(const char* filePath)
	{
		// tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		m_initData.m_tkmFilePath = filePath;
		// �V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		m_initData.m_fxFilePath = "Assets/shader/model.fx";

		if (m_animationClips != nullptr) {
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			m_initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//�X�P���g�����w�肷��B
			m_initData.m_skeleton = &m_skeleton;
		}
		else {
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			m_initData.m_vsEntryPointFunc = "VSMain";
		}

		//���f���̏�������w�肷��B
		m_initData.m_modelUpAxis = enModelUpAxisY;
		//���C�g�̏���悷�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v�ɓo�^���邽�߂�
		//���f���̏��������Ƃ��ēn���B
		m_initData.m_expandConstantBuffer = g_light.GetLightData();
		m_initData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
	

		//�쐬�����������f�[�^�����ƂɃ��f��������������B
		m_model.Init(m_initData);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		m_skeleton.Init("Assets/modelData/unityChan.tks");
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		
		// �A�j���[�V������������
		m_animation.Init(
			m_skeleton,					// �A�j���[�V�����𗬂����ރX�P���g��
			m_animationClips,			// �A�j���[�V�����N���b�v
			m_numAnimationClips			// �A�j���[�V�����̐�
		);
	}
	void ModelRender::Update()
	{
		// �X�P���g�����X�V
		m_skeleton.Update(m_model.GetWorldMatrix());
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

