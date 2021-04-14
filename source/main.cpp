#include "DxLib.h"
#include "Use/Parameter.h"
#include "Game.h"
#include "ResourceServer.h"
#include "EffekseerForDXLib.h"
#include "Use/Fade.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

	if (!Parameter::Init()) return -1;

	SetMainWindowText(_T("マジカルナンバー"));
	SetGraphMode(Parameter::_ScreenWidth, Parameter::_ScreenHeight, 32);   //画面サイズ設定
	ChangeWindowMode(true);
	//SetBackgroundColor(255, 255, 255);   //背景の色変更

	SetUseDirect3DVersion(DX_DIRECT3D_11);   //Effekseerを使用するには必ず設定する。

	if (DxLib_Init() == -1) return -1;

	if (Effekseer_Init(8000) == -1) { // Effekseerを初期化引数には画面に表示する最大パーティクル数を設定する。
		DxLib_End();
		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);  //Effekseer
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();   //Effekseer
	Effekseer_Set2DSetting(Parameter::_ScreenWidth, Parameter::_ScreenHeight);   //自分の設定に

	if (!Fade::Init()) {
		return -1;
	}

	Game game;

	if (!game.Init()) {
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		if (!game.Process()) {
			break;
		}

		Fade::Process();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer2D();

		ClearDrawScreen();

		game.Draw();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer2D();

		Fade::Draw();

		DxLib::ScreenFlip();
	}

	// エフェクトリソースを削除する。(Effekseer終了時に破棄されるので削除しなくてもいい)
	//DeleteEffekseerEffect(effectResourceHandle);
	
	Effkseer_End();   // Effekseerを終了する。
	DxLib_End();   // ＤＸライブラリ使用の終了処理
	return 0;   // ソフトの終了 
}