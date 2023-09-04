#pragma region 自クラス読み込み(含める)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> キーボード入力
#include "Mouse.h"// -> マウスキーボード入力
#include "Object3D.h"// -> オブジェクト3Dクラス
#include "Sound.h"
#include "ImGuiManager.h"
#pragma endregion

#include "SceneManager.h"

// --Windowsアプリでのエントリーポイント(main関数)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// WinAPIの初期化
	WinAPI::GetInstance()->Initialize(1280, 720);

	// DirectX12の初期化
	DX12Cmd::GetInstance()->Initialize();

	// --テクスチャクラス-- //
	Texture::GetInstance()->Initialize();

	// サウンドクラス
	Sound::GetInstance()->Initialize();

	// ImGuiクラス
	ImGuiManager::GetInstance()->Initialize();

	// --ゲームループ-- //
	while (true) {
		// --終了メッセージが来ていたらループ終了-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;

		// キーボード入力更新処理
		Key::GetInstance()->Update();

		// マウス入力更新処理
		Mouse::GetInstance()->Update();

		// ImGui描画受付開始
		//ImGuiManager::GetInstance()->Begin();

		// シーン管理クラス更新処理
		SceneManager::GetInstance()->Update();

		// ImGui描画受付終了
		//ImGuiManager::GetInstance()->End();

		// シーン管理クラス描画処理
		SceneManager::GetInstance()->Draw();
	
		// ImGui描画処理
		//ImGuiManager::GetInstance()->Draw();
	}

	return 0;
}