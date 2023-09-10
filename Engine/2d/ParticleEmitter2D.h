#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <deque>

class ParticleEmitter2D
{
private:
	// エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma region 構造体
private:
	// 頂点データ
	struct Vertex {
		Vector2 pos = { 0.0f, 0.0f };
		float pad;
		float scale = 0.0f;
	};

	// 定数バッファ用
	struct ConstBufferData {
		Matrix4 mat;
	};

	// パーティクル
	struct Particle {
		Vector2 position = { 0.0f, 0.0f };
		Vector2 velocity = { 0.0f, 0.0f };
		Vector2 accel = { 0.0f, 0.0f };
		uint16_t frame = 0;
		uint16_t num_frame = 0;
		float scale = 1.0f;
		float startScale = 1.0f;
		float endScale = 0.0f;
	};
#pragma endregion

#pragma region メンバ変数
private:
	// パーティクルコンテナ
	std::deque<Particle> particles_;

	// 最大頂点数
	const uint16_t MAX_VERTEX = 1024;

	// 発生位置
	Vector2 position_ = { 0.0f, 0.0f };

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_ = nullptr;
	ConstBufferData* constMap_ = nullptr;

	// 頂点データ
	D3D12_VERTEX_BUFFER_VIEW vertexView_{};	// 頂点バッファービュー
	ComPtr<ID3D12Resource> vertexBuff_ = nullptr;		// 頂点バッファ

	// インデックスデータ
	D3D12_INDEX_BUFFER_VIEW indexView_{};	// インデックスバッファビュー
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;	// インデックスバッファ

	// カメラ
	static Camera* sCamera_;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	ParticleEmitter2D(uint16_t maxVertex);

	// デストラクタ
	~ParticleEmitter2D();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(uint16_t handle = 0);

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="life"> 生存時間 </param>
	/// <param name="pos"> 初期座標 </param>
	/// <param name="velocity"> 速度 </param>
	/// <param name="accel"> 加速度 </param>
	void Add(uint16_t life, const Vector2& pos, const Vector2& velocity, const Vector2& accel, float startScale, float endScale);

private:
	// 定数バッファ生成
	void CreateConstBuff();

	// 頂点バッファ生成
	void CreateVertexBuff(uint16_t maxVertex);

	// インデックスバッファ生成
	void CreateIndexBuff();
#pragma endregion

#pragma region セッター関数
public:
	// 座標を設定
	void SetPosition(const Vector2& pos) { position_ = pos; }

	// カメラを設定
	static void SetCamera(Camera* camera) { sCamera_ = camera; }
#pragma endregion
};