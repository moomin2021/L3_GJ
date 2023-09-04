#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

#include <d3d12.h>
#include <wrl.h>

class LightGroup {
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region 定数メンバ変数
	// 各ライト上限
	static const uint16_t DIR_LIGHT_LIMIT = 1;
	static const uint16_t POINT_LIGHT_LIMIT = 10;
	static const uint16_t SPOT_LIGHT_LIMIT = 10;
	static const uint16_t CIRCLE_SHADOW_LIMIT = 1;
#pragma endregion

#pragma region 構造体
public:
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Vector3 ambientColor;// 環境光の色
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_LIMIT];		// 平行光源用
		PointLight::ConstBufferData pointLights[POINT_LIGHT_LIMIT];			// 点光源用
		SpotLight::ConstBufferData spotLights[SPOT_LIGHT_LIMIT];			// スポットライト用
		CircleShadow::ConstBufferData circleShadows[CIRCLE_SHADOW_LIMIT];	// 丸影用
	};
#pragma endregion

#pragma region メンバ変数
private:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;// ライトデータ

	// 環境光の色
	Vector3 ambientColor_;
	
	// ライトデータ
	std::vector<DirectionalLight*> dirLights_;	// 平行光源の配列
	std::vector<PointLight*> pointLights_;		// 点光源の配列
	std::vector<SpotLight*> spotLights_;			// スポットライト用
	std::vector<CircleShadow*> circleShadows_;	// 丸影の配列

	// ダーティフラグ
	bool dirty_;
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LightGroup();

	/// <summary>
	/// ライト描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 平行光源追加
	/// </summary>
	void AddDirLight(DirectionalLight* light);

	/// <summary>
	/// ポイントライト追加
	/// </summary>
	void AddPointLight(PointLight* light);

	/// <summary>
	/// スポットライト追加
	/// </summary>
	void AddSpotLight(SpotLight* light);

	/// <summary>
	/// 丸影追加
	/// </summary>
	void AddCircleShadow(CircleShadow* shadow);

private:
	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();
#pragma endregion
};