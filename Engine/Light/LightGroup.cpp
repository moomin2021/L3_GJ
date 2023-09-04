#include "LightGroup.h"
#include "DX12Cmd.h"

#include <d3dx12.h>
#include <cassert>

LightGroup::LightGroup() :
#pragma region 初期化リスト
	// 定数バッファ
	constBuff_(nullptr),

	// 環境光の色
	ambientColor_{ 1.0f, 1.0f, 1.0f },
	
	// ライトデータ
	dirLights_(DIR_LIGHT_LIMIT),		// 平行光源の配列
	pointLights_(POINT_LIGHT_LIMIT),	// 点光源の配列
	spotLights_(SPOT_LIGHT_LIMIT),		// スポットライト用
	circleShadows_(CIRCLE_SHADOW_LIMIT),// 丸影の配列

	// ダーティフラグ
	dirty_(false)
#pragma endregion
{
	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void LightGroup::Draw() {
	// 定数バッファ転送
	TransferConstBuffer();

	// コマンドリスト取得
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(3, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::AddDirLight(DirectionalLight* light) {
	dirLights_[0] = light;
}

void LightGroup::AddPointLight(PointLight* light) {
	for (size_t i = 0; i < POINT_LIGHT_LIMIT; i++) {
		// 中身が空だったら
		if (pointLights_[i] == nullptr) {
			pointLights_[i] = light;
			break;
		}
	}
}

void LightGroup::TransferConstBuffer() {

	// ダーティフラグが[OFF]なら処理を飛ばす
	//if (dirty_ == false) return;

	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

#pragma region 定数バッファマッピング
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
#pragma endregion

	if (SUCCEEDED(result)) {
#pragma region 平行光源
		// 平行光源
		for (size_t i = 0; i < DIR_LIGHT_LIMIT; i++) {
			// 中身が空ではなかったら
			if (dirLights_[i] != nullptr) {
				// ライトが有効なら設定を転送
				if (dirLights_[i]->GetActive()) {
					constMap->dirLights[i].active = true;
					constMap->dirLights[i].lightVec = -dirLights_[i]->GetLightDir();
					constMap->dirLights[i].lightColor = dirLights_[i]->GetLightColor();
				}

				// ライトが無効なら転送しない
				else {
					constMap->dirLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region 点光源
		for (size_t i = 0; i < POINT_LIGHT_LIMIT; i++) {
			// 中身が空ではなかったら
			if (pointLights_[i] != nullptr) {
				// ライトが有効なら設定を転送
				if (pointLights_[i]->GetActive()) {
					constMap->pointLights[i].active = true;
					constMap->pointLights[i].lightPos = pointLights_[i]->GetLightPos();
					constMap->pointLights[i].lightColor = pointLights_[i]->GetLightColor();
					constMap->pointLights[i].lightAtten = pointLights_[i]->GetLightAtten();
				}

				// ライトが無効ならライト色を0に
				else {
					constMap->pointLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region スポットライト
		for (size_t i = 0; i < SPOT_LIGHT_LIMIT; i++) {
			// 中身が空ではなかったら
			if (spotLights_[i] != nullptr) {
				// ライトが有効なら設定を転送
				if (spotLights_[i]->GetActive()) {
					constMap->spotLights[i].active = true;
					constMap->spotLights[i].lightVec = -spotLights_[i]->GetLightDir();
					constMap->spotLights[i].lightPos = spotLights_[i]->GetLightPos();
					constMap->spotLights[i].lightColor = spotLights_[i]->GetLightColor();
					constMap->spotLights[i].lightAtten = spotLights_[i]->GetLightAtten();
					constMap->spotLights[i].lightFactorAngleCos = spotLights_[i]->GetLightFactorAngle();
				}

				// ライトが無効ならライト色を0に
				else {
					constMap->spotLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region 丸影
		// 丸影
		for (size_t i = 0; i < CIRCLE_SHADOW_LIMIT; i++) {
			// 中身空ではなかったら
			if (circleShadows_[i] != nullptr) {
				// 有効なら設定を転送
				if (circleShadows_[i]->GetActive()) {
					constMap->circleShadows[i].active = true;
					constMap->circleShadows[i].dir = -circleShadows_[i]->GetDir();
					constMap->circleShadows[i].casterPos = circleShadows_[i]->GetCasterPos();
					constMap->circleShadows[i].distCasterLight = circleShadows_[i]->GetDistCasterLight();
					constMap->circleShadows[i].atten = circleShadows_[i]->GetAtten();
					constMap->circleShadows[i].factorAngleCos = circleShadows_[i]->GetFactorAngleCos();
				}

				// 無効なら色を0に
				else {
					constMap->circleShadows[i].active = false;
				}
			}
		}
#pragma endregion

		// 環境光
		constMap->ambientColor = ambientColor_;

		// マッピング後処理
		constBuff_->Unmap(0, nullptr);
	}

	// 転送し終わったのでダーティフラグを[OFF]にする
	dirty_ = false;
}