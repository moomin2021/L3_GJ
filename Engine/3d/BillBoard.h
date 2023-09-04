//#pragma once
//// --Direct3D 12 �p-- //
//#include <d3d12.h>
//#pragma comment(lib, "d3d12.lib")
//
//// --���w�֐�-- //
//#include <DirectXMath.h>
//using namespace DirectX;
//
//// --ComPtr�p-- //
//#include <wrl.h>
//using namespace Microsoft::WRL;
//
//// --�V�[�P���X�R���e�i�̈��-- //
//#include <vector>
//
//// --�J�����N���X-- //
//#include "Camera.h"
//
//// --�I�u�W�F�N�g3D�N���X-- //
//#include "Object3D.h"
//
//enum BillBoardType {
//	BILLBOARDX,
//	BILLBOARDY,
//	BILLBOARDZ,
//	BILLBOARDALL,
//};
//
//class BillBoard {
//	/// �����o�ϐ�
//public:
//	// ���W�A��]�p�A�X�P�[��
//	XMFLOAT3 position_;
//	float rotation_;
//	XMFLOAT2 scale_;
//
//	// �F
//	XMFLOAT4 color_;
//
//private:
//	// --���_�f�[�^-- //
//	std::vector<Vertex3D> vertices_;// -> ���_�f�[�^
//	D3D12_VERTEX_BUFFER_VIEW vbView_;// -> ���_�o�b�t�@�r���[
//	ComPtr<ID3D12Resource> vertBuff_;// -> ���_�o�b�t�@
//
//	// --�C���f�b�N�X�f�[�^-- //
//	std::vector<uint16_t> indices_;// -> �C���f�b�N�X�f�[�^
//	D3D12_INDEX_BUFFER_VIEW ibView_;// -> �C���f�b�N�X�o�b�t�@�r���[
//	ComPtr<ID3D12Resource> indexBuff_;// -> �C���f�b�N�X�o�b�t�@
//
//	// --�萔�o�b�t�@-- //
//	ComPtr<ID3D12Resource> constBuff_;
//
//	// --�������e�s��̌v�Z-- //
//	XMMATRIX matProjection_;
//
//	/// �����o�֐�
//public:
//	// --�R���X�g���N�^-- //
//	BillBoard();
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
//	void CreateBuffer();
//
//	// --�X�V����-- //
//	void Update(Camera* camera, BillBoardType type);
//
//	// --�`�揈��-- //
//	void Draw(int textureHandle = 0);
//
//	// --�`��O����-- //
//	static void PreDraw();
//
//private:
//	// --�l�p�`�̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬-- //
//	void CreateSquare();
//};