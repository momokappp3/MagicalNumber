#include "DxLib.h"
#include "Use/Parameter.h"
#include "Game.h"
#include "ResourceServer.h"
#include "EffekseerForDXLib.h"
#include "Use/Fade.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	if (!Parameter::Init()) return -1;

	SetMainWindowText(_T("�}�W�J���i���o�["));
	SetGraphMode(Parameter::_ScreenWidth, Parameter::_ScreenHeight, 32);   //��ʃT�C�Y�ݒ�
	ChangeWindowMode(true);
	//SetBackgroundColor(255, 255, 255);   //�w�i�̐F�ύX

	SetUseDirect3DVersion(DX_DIRECT3D_11);   //Effekseer���g�p����ɂ͕K���ݒ肷��B

	if (DxLib_Init() == -1) return -1;

	if (Effekseer_Init(8000) == -1) { // Effekseer�������������ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
		DxLib_End();
		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);  //Effekseer
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();   //Effekseer
	Effekseer_Set2DSetting(Parameter::_ScreenWidth, Parameter::_ScreenHeight);   //�����̐ݒ��

	if (!Fade::Init()) {
		return -1;
	}

	Game game;

	if (!game.Init()) {
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// Z�o�b�t�@��L���ɂ���B
    // Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		if (!game.Process()) {
			break;
		}

		Fade::Process();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer2D();

		ClearDrawScreen();

		game.Draw();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer2D();

		Fade::Draw();

		DxLib::ScreenFlip();
	}

	// �G�t�F�N�g���\�[�X���폜����B(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă�����)
	//DeleteEffekseerEffect(effectResourceHandle);
	
	Effkseer_End();   // Effekseer���I������B
	DxLib_End();   // �c�w���C�u�����g�p�̏I������
	return 0;   // �\�t�g�̏I�� 
}