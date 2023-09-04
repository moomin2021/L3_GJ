#pragma once
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
#pragma region メンバ変数
private:
	// カメラデータ
	Vector3 eye_;	// カメラの位置
	Vector3 target_;	// カメラの注視点
	Vector3 up_;		// カメラの上方向ベクトル

	// 行列
	Matrix4 matView_;		// ビュー変換行列
	Matrix4 matProjection_;	// 透視投影行列
#pragma endregion

#pragma region メンバ関数
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

#pragma region セッター関数
	/// <summary>
	/// カメラの位置を設定
	/// </summary>
	/// <param name="eye"> カメラの位置 </param>
	inline void SetEye(const Vector3& eye) { eye_ = eye; }

	/// <summary>
	/// カメラの注視点を設定
	/// </summary>
	/// <param name="target"> カメラの注視点 </param>
	inline void SetTarget(const Vector3& target) { target_ = target; }

	/// <summary>
	/// カメラの上方向ベクトル
	/// </summary>
	/// <param name="up"> カメラの上方向ベクトル </param>
	inline void SetUp(const Vector3& up) { up_ = up; }
#pragma endregion

#pragma region ゲッター関数
	/// <summary>
	/// カメラの位置を取得
	/// </summary>
	/// <returns> カメラの位置 </returns>
	inline const Vector3& GetEye() { return eye_; }

	/// <summary>
	/// カメラの注視点を取得
	/// </summary>
	/// <returns> カメラの注視点 </returns>
	inline const Vector3& GetTarget() { return target_; }

	/// <summary>
	/// カメラの上方向を取得
	/// </summary>
	/// <returns> カメラの上方向 </returns>
	inline const Vector3& GetUp() { return up_; }

	/// <summary>
	/// ビュー変換行列を取得
	/// </summary>
	/// <returns> ビュー変換行列 </returns>
	inline const Matrix4& GetMatView() { return matView_; }

	/// <summary>
	/// 透視投影行列を取得
	/// </summary>
	/// <returns> 透視投影行列 </returns>
	inline const Matrix4& GetMatProjection() { return matProjection_; }
#pragma endregion

#pragma endregion
};