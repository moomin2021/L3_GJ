#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "float4.h"
#include "Matrix4.h"
#include "Camera.h"

#include <d3d12.h>
#include <wrl.h>
#include <vector>

class Sprite {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �\����
	// ���_�f�[�^
	struct Vertex {
		Vector2 pos;// ���W(XYZ)
		Vector2 uv;	// UV���W(XY)
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Matrix4 mat;	// �s��
		float4 color;	// �F(RGBA)
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �J����
	static Camera* sCamera_;

	// �X�v���C�g�f�[�^
	Vector2 position_ = { 0.0f, 0.0f };				// ���W(XY)
	float rotation_ = 0.0f;						// ��](Z)
	Vector2 size_ = { 100.0f, 100.0f };			// �T�C�Y(XY)
	float4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };	// �F(RGBA)
	Vector2 anchorPoint_ = { 0.0f, 0.0f };				// �A���J�[�|�C���g���W(XY)
	bool isFlipX_ = false;					// ���E���]
	bool isFlipY_ = false;					// �㉺���]

	// ���[���h�s��
	Matrix4 matWorld_ = {};

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_ = nullptr;	// �萔�o�b�t�@
	ConstBufferData* constMap_ = nullptr;	// �}�b�s���O�����p

	// ���_�f�[�^
	std::vector<Vertex> vertex_ = {};		// ���_�f�[�^
	ComPtr<ID3D12Resource> vertexBuff_ = nullptr;	// ���_�o�b�t�@
	Vertex* vertexMap_ = nullptr;	// �}�b�s���O�����p
	D3D12_VERTEX_BUFFER_VIEW vertexView_ = {};		// ���_�o�b�t�@�r���[

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> index_ = {};		// �C���f�b�N�X�f�[�^
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;	// �C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW indexView_ = {};		// �C���f�b�N�X�o�b�t�@�r���[
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Sprite();

	// �s��X�V����
	void MatUpdate();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="textureHandle"> �e�N�X�`���n���h�� </param>
	void Draw(int textureHandle = 0);
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �J������ݒ�
	static void SetCamera(Camera* camera) { sCamera_ = camera; }

	// ���W(XY)��ݒ�
	inline void SetPosition(const Vector2& position) { position_ = position; }

	// ��](Z)(�x���@)��ݒ�
	inline void SetRotation(float rotation) { rotation_ = rotation; }

	// �T�C�Y(XY)��ݒ�
	inline void SetSize(const Vector2& size) { size_ = size; }

	// �F(RGBA)��ݒ�
	inline void SetColor(const float4& color) { color_ = color; }

	// �A���J�[�|�C���g(XY)��ݒ�
	inline void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	// ���E�����ݒ�
	inline void SetFlipX(bool flag) { isFlipX_ = flag; }

	// �㉺�����ݒ�
	inline void SetFlipY(bool flag) { isFlipY_ = flag; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	// ���W(XY)���擾
	inline const Vector2& GetPosition() { return position_; }

	// ��](Z)(�x���@)���擾
	inline float GetRotation() { return rotation_; }

	// �T�C�Y(XY)���擾
	inline const Vector2& GetSize() { return size_; }

	// �F(RGBA)���擾
	inline const float4& GetColor() { return color_; }

	// �A���J�[�|�C���g(XY)���擾
	inline const Vector2& GetAnchorPoint() { return anchorPoint_; }

	// ���E������擾
	inline bool GetFlipX() { return isFlipX_; }

	// �㉺������擾
	inline bool GetFlipY() { return isFlipY_; }
#pragma endregion
};