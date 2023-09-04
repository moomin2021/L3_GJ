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
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���_�f�[�^
	struct Vertex {
		Vector2 pos;	// ���W(XYZ)
		Vector2 uv;	// UV���W(XY)
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Matrix4 mat;	// �s��
		float4 color;	// �F(RGBA)
	};

#pragma region �����o�ϐ�
private:
	// �X�v���C�g�f�[�^
	Vector2 position_;	// ���W(XY)
	float rotation_;	// ��](Z)
	Vector2 size_;		// �T�C�Y(XY)
	float4 color_;		// �F(RGBA)
	Vector2 anchorPoint_;// �A���J�[�|�C���g���W(XY)
	bool isFlipX_;		// ���E���]
	bool isFlipY_;		// �㉺���]

	// �X�v���C�g�f�[�^��ύX�������ǂ���
	bool hasChanget_;

	// �s��
	Matrix4 matWorld_;		// ���[���h�s��
	Matrix4 matProjection_;	// �ˉe�ϊ��s��

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;	// �萔�o�b�t�@
	ConstBufferData* constMap_;			// �}�b�s���O�����p

	// ���_�f�[�^
	std::vector<Vertex> vertex_;		// ���_�f�[�^
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@
	Vertex* vertexMap_;						// �}�b�s���O�����p
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�r���[

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> index_;	// �C���f�b�N�X�f�[�^
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(int textureHandle = 0);

private:
	/// <summary>
	/// �X�v���C�g�f�[�^�̍X�V
	/// </summary>
	void UpdateData();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:

	/// <summary>
	/// ���W(XY)��ݒ�
	/// </summary>
	/// <param name="position"> ���W(XY) </param>
	inline void SetPosition(const Vector2& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// ��](Z)��ݒ�
	/// </summary>
	/// <param name="rotation"> ��](Z) </param>
	inline void SetRotation(float rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// �g�k(XY)��ݒ�
	/// </summary>
	/// <param name="scale"> �g�k(XY) </param>
	inline void SetSize(const Vector2& size) { size_ = size, hasChanget_ = true; }

	/// <summary>
	/// �F(RGBA)��ݒ�
	/// </summary>
	/// <param name="color"> �F(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }

	/// <summary>
	/// �A���J�[�|�C���g(XY)��ݒ�
	/// </summary>
	/// /// <param name="anchorPoint"> �A���J�[�|�C���g(XY) </param>
	inline void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint, hasChanget_ = true; }

	/// <summary>
	/// ���E���]��ݒ�
	/// </summary>
	/// <param name="flag"> ���E���]�t���O </param>
	inline void SetFlipX(bool flag) { isFlipX_ = flag, hasChanget_ = true; }

	/// <summary>
	/// �㉺���]��ݒ�
	/// </summary>
	/// <param name="flag"> �㉺���]�t���O </param>
	inline void SetFlipY(bool flag) { isFlipY_ = flag, hasChanget_ = true; }
#pragma endregion
};