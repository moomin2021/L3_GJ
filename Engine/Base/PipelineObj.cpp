#include "PipelineObj.h"
#include "DX12Cmd.h"
#include "Util.h"

#include <d3dcompiler.h>
#include <d3d12.h>
#include <cassert>

PipelineObj::PipelineObj()
{
}

void PipelineObj::LoadShader(std::string fileName, ShaderType shaderType)
{
	switch (shaderType)
	{
		// 頂点シェーダー
	case VS: ShaderCompileFromFile(fileName, "vs_5_0", &vsBlob_);
		break;
		// ピクセルシェーダー
	case PS: ShaderCompileFromFile(fileName, "ps_5_0", &psBlob_);
		break;
		// ジオメトリシェーダー
	case GS: ShaderCompileFromFile(fileName, "gs_5_0", &gsBlob_);
		break;
	}
}

void PipelineObj::AddInputLayout(const char* semanticName, DXGI_FORMAT format, uint16_t index)
{
	// インプットレイアウトを設定＆追加
	inputLayout_.emplace_back(D3D12_INPUT_ELEMENT_DESC{
		semanticName,								// セマンティック名
		index,										// 同じセマンティック名が複数あるときに使うインデックス（0でよい）
		format,										// 要素数とビット数を表す
		0,											// 入力スロットインデックス（0でよい）
		D3D12_APPEND_ALIGNED_ELEMENT,				// データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,	// 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
		0											// 一度に描画するインスタンス数（0でよい）
		});
}

void PipelineObj::CreateRootParams(uint16_t texRegisterNum, uint16_t constBuffNum)
{
	// ルートパラメータの生成カウンター
	uint16_t counter = 0;

	rootParams_.resize(texRegisterNum + constBuffNum);
	descRangeSRV_.resize(texRegisterNum);

	for (size_t i = 0; i < texRegisterNum; i++) {
		descRangeSRV_[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, static_cast<UINT>(i));// t i レジスタ

		// ルートパラメータ設定
		rootParams_[counter].InitAsDescriptorTable(1, &descRangeSRV_[i], D3D12_SHADER_VISIBILITY_ALL);

		// ルートパラメータを生成＆設定し終わったためインクリメント
		counter++;
	}

	for (size_t i = 0; i < constBuffNum; i++) {
		// ルートパラメータ設定
		rootParams_[counter].InitAsConstantBufferView(static_cast<UINT>(i), 0, D3D12_SHADER_VISIBILITY_ALL);

		// ルートパラメータを生成＆設定し終わったためインクリメント
		counter++;
	}
}

void PipelineObj::CreatePipeline(uint16_t renderTargetNum, BLENDMODE blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType, bool isDepth) {
	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// デバイス取得
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

#pragma region シェーダー設定
	// 頂点シェーダー
	if (vsBlob_) {
		pipelineDesc.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
		pipelineDesc.VS.BytecodeLength = vsBlob_->GetBufferSize();
	}

	// ピクセルシェーダー
	if (psBlob_) {
		pipelineDesc.PS.pShaderBytecode = psBlob_->GetBufferPointer();
		pipelineDesc.PS.BytecodeLength = psBlob_->GetBufferSize();
	}

	// ジオメトリシェーダー
	if (gsBlob_) {
		pipelineDesc.GS.pShaderBytecode = gsBlob_->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = gsBlob_->GetBufferSize();
	}
#pragma endregion

#pragma region サンプルマスク設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
#pragma endregion

#pragma region ラスタライザ設定
	// カリングとは描画する必要がないポリゴンを描画しないようにする手法
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// D3D12_CULL_MODE_NONE		= 常にすべての三角形を描画する
	// D3D12_CULL_MODE_FRONT	= 正面を向いた三角形を描画しない
	// D3D12_CULL_MODE_BACK		= 後ろ向きの三角形を描画しない

	// 三角形をレンダリングするときに使用する塗りつぶしモードを指定
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	// D3D12_FILL_MODE_WIREFRAME	= 頂点を結ぶ線を描画する
	// D3D12_FILL_MODE_SOLID		= 頂点によって形成された三角形を塗りつぶす

	// 距離に基づいてクリッピングを有効にするか指定
	pipelineDesc.RasterizerState.DepthClipEnable = true;
#pragma endregion

#pragma region デプスステンシルステート
	// 立体を描画するとき、正しい前後関係を描くための機能が深度テスト

	// 深度テストを行うか
	pipelineDesc.DepthStencilState.DepthEnable = true;

	// 深度データによって変更できる深度ステンシルバッファの一部を識別する
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	if (isDepth) pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;								// 深度値フォーマット
#pragma endregion

#pragma region ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};

	// ブレンドを有効にするか
	blendDesc.BlendEnable = true;

	// RGBAそれぞれの値に書き込むかどうか
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// アルファをどうブレンドするか
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	// ピクセルシェーダーが出力するアルファ値に対して実行する操作を指定
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;

	// レンダーターゲットの現在のアルファ値に対して実行する操作を指定
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// 半透明
	if (blendMode == ALPHA) {
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}

	// 加算
	else if (blendMode == ADD) {
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
	}

	// 減算
	else if (blendMode == SUB) {
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
	}

	// 反転
	else if (blendMode == INV) {
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ZERO;
	}

	// ブレンドステートの設定
	for (size_t i = 0; i < renderTargetNum; i++) {
		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}
#pragma endregion

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout_.data();
	pipelineDesc.InputLayout.NumElements = static_cast<UINT>(inputLayout_.size());

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = primitiveType;

	// その他の設定
	pipelineDesc.NumRenderTargets = renderTargetNum;					// 描画対象は2つ
	for (size_t i = 0; i < renderTargetNum; i++) {
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0~255指定のRGBA
	}
	pipelineDesc.SampleDesc.Count = 1;									// 1ピクセルにつき1回サンプリング

	// テクスチャサンプラーの設定
	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//全てリニア保管
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		static_cast<UINT>(rootParams_.size()), rootParams_.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature_.Get();

	// パイプランステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

void PipelineObj::ShaderCompileFromFile(std::string fileName, LPCSTR target, ID3D10Blob** shaderBlob)
{
	// 関数が成功したかどうかを判別する用変数
	HRESULT result;

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		Util::StringToWideChar(fileName).data(),		// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				// インクルード可能にする
		"main", target,									// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,// デバッグ用設定
		0,
		shaderBlob, errorBlob_.GetAddressOf());

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}