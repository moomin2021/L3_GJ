#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "float4.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

class Sprite {
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点データ
	struct Vertex {
		Vector2 pos;	// 座標(XYZ)
		Vector2 uv;	// UV座標(XY)
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Matrix4 mat;	// 行列
		float4 color;	// 色(RGBA)
	};

#pragma region メンバ変数
private:
	// スプライトデータ
	Vector2 position_;	// 座標(XY)
	float rotation_;	// 回転(Z)
	Vector2 size_;		// サイズ(XY)
	float4 color_;		// 色(RGBA)
	Vector2 anchorPoint_;// アンカーポイント座標(XY)
	bool isFlipX_;		// 左右反転
	bool isFlipY_;		// 上下反転

	// スプライトデータを変更したかどうか
	bool hasChanget_;

	// 行列
	Matrix4 matWorld_;		// ワールド行列
	Matrix4 matProjection_;	// 射影変換行列

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;	// 定数バッファ
	ConstBufferData* constMap_;			// マッピング処理用

	// 頂点データ
	std::vector<Vertex> vertex_;		// 頂点データ
	ComPtr<ID3D12Resource> vertexBuff_;		// 頂点バッファ
	Vertex* vertexMap_;						// マッピング処理用
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// 頂点バッファビュー

	// インデックスデータ
	std::vector<uint16_t> index_;	// インデックスデータ
	ComPtr<ID3D12Resource> indexBuff_;	// インデックスバッファ
	D3D12_INDEX_BUFFER_VIEW indexView_;	// インデックスバッファビュー
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(int textureHandle = 0);

private:
	/// <summary>
	/// スプライトデータの更新
	/// </summary>
	void UpdateData();
#pragma endregion

#pragma region セッター関数
public:

	/// <summary>
	/// 座標(XY)を設定
	/// </summary>
	/// <param name="position"> 座標(XY) </param>
	inline void SetPosition(const Vector2& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// 回転(Z)を設定
	/// </summary>
	/// <param name="rotation"> 回転(Z) </param>
	inline void SetRotation(float rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// 拡縮(XY)を設定
	/// </summary>
	/// <param name="scale"> 拡縮(XY) </param>
	inline void SetSize(const Vector2& size) { size_ = size, hasChanget_ = true; }

	/// <summary>
	/// 色(RGBA)を設定
	/// </summary>
	/// <param name="color"> 色(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }

	/// <summary>
	/// アンカーポイント(XY)を設定
	/// </summary>
	/// /// <param name="anchorPoint"> アンカーポイント(XY) </param>
	inline void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint, hasChanget_ = true; }

	/// <summary>
	/// 左右反転を設定
	/// </summary>
	/// <param name="flag"> 左右反転フラグ </param>
	inline void SetFlipX(bool flag) { isFlipX_ = flag, hasChanget_ = true; }

	/// <summary>
	/// 上下反転を設定
	/// </summary>
	/// <param name="flag"> 上下反転フラグ </param>
	inline void SetFlipY(bool flag) { isFlipY_ = flag, hasChanget_ = true; }
#pragma endregion
};