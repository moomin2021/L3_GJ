//#include "BillBoard.h"
//
//// --DirextX12-- //
//#include "DX12Cmd.h"
//
//// --�e�N�X�`���N���X-- //
//#include "Texture.h"
//
//// --�֗��n�֐�-- //
//#include "Util.h"
//
//// --���f���ǂݍ��݂̂���-- //
//#include <fstream>
//#include <sstream>
//#include <string>
//using namespace std;
//
//BillBoard::BillBoard() :
//#pragma region ���������X�g
//	// ���W�A��]�p�A�X�P�[��
//	position_{ 0.0f, 0.0f, 0.0f },
//	rotation_(0.0f),
//	scale_{ 1.0f, 1.0f },
//
//	// �F
//	color_{ 1.0f, 1.0f, 1.0f, 1.0f },
//
//	// --�������e�s��̌v�Z-- //
//	matProjection_{},
//
//	// --���_�f�[�^-- //
//	vertices_{},// -> ���_�f�[�^
//	vbView_{},// -> ���_�o�b�t�@�r���[
//	vertBuff_(nullptr),// -> ���_�o�b�t�@
//
//	// --�C���f�b�N�X�f�[�^-- //
//	indices_{},// -> �C���f�b�N�X�f�[�^
//	ibView_{},// -> �C���f�b�N�X�o�b�t�@�r���[
//	indexBuff_(nullptr),// -> �C���f�b�N�X�o�b�t�@
//
//	// --�萔�o�b�t�@-- //
//	constBuff_(nullptr)
//#pragma endregion
//{
//#pragma region �萔�o�b�t�@����������
//	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
//	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
////	HRESULT result;
////
////	// �萔�o�b�t�@�̃q�[�v�ݒ�
////	D3D12_HEAP_PROPERTIES heapProp{};
////	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
////
////	// �萔�o�b�t�@�̃��\�[�X�ݒ�
////	D3D12_RESOURCE_DESC resdesc{};
////	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
////	resdesc.Width = (sizeof(ObjectBuff) + 0xff) & ~0xff;
////	resdesc.Height = 1;
////	resdesc.DepthOrArraySize = 1;
////	resdesc.MipLevels = 1;
////	resdesc.SampleDesc.Count = 1;
////	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
////
////	// �萔�o�b�t�@�̐���
////	result = DX12Cmd::GetDevice()->CreateCommittedResource(
////		&heapProp,
////		D3D12_HEAP_FLAG_NONE,
////		&resdesc,
////		D3D12_RESOURCE_STATE_GENERIC_READ,
////		nullptr,
////		IID_PPV_ARGS(&constBuff_));
////	assert(SUCCEEDED(result));
////
////	// �萔�o�b�t�@�̃}�b�s���O
////	ObjectBuff* constMap;
////	result = constBuff_->Map(0, nullptr, (void**)&constMap);
////	assert(SUCCEEDED(result));
////
////	// �}�b�s���O�I��
////	constBuff_->Unmap(0, nullptr);
////#pragma endregion
////
////	// --�������e�s��̌v�Z-- //
////	matProjection_ = XMMatrixPerspectiveFovLH(
////		Util::Degree2Radian(45.0f),// -----------> �㉺��p45�x
////		(float)WinAPI::GetWidth() / WinAPI::GetHeight(),// -> �A�X�y�N�g��i��ʉ���/��ʏc���j
////		0.1f, 1000.0f// ------------------------> �O�[�A���[
////	);
////
////	// �l�p�`�̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬
////	CreateSquare();
//}
//
//void BillBoard::Update(Camera* camera, BillBoardType type)
//{
//	// ���_���W
//	//XMVECTOR eyePos = XMLoadFloat3(&camera->eye_);
//
//	//// �����_���W
//	//XMVECTOR targetPos = XMLoadFloat3(&camera->target_);
//
//	//// �����
//	//XMVECTOR upVec = XMLoadFloat3(&camera->up_);
//
//	//// �J����Z��
//	//XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);
//
//	//// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O //
//	//assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
//	//assert(!XMVector3IsInfinite(cameraAxisZ));
//	//assert(!XMVector3Equal(upVec, XMVectorZero()));
//	//assert(!XMVector3IsInfinite(upVec));
//
//	//// �x�N�g���𐳋K�� //
//	//cameraAxisZ = XMVector3Normalize(cameraAxisZ);
//
//	//// �J������X��(�E����) //
//	//XMVECTOR cameraAxisX;
//
//	//// X���͏������Z���̊O�ςŋ��܂� //
//	//cameraAxisX = XMVector3Cross(upVec, cameraAxisZ);
//
//	//// �x�N�g���𐳋K�� //
//	//cameraAxisX = XMVector3Normalize(cameraAxisX);
//
//	//// �J������Y��(�����) //
//	//XMVECTOR cameraAxisY;
//
//	//// Y����Z����X���̊O�ςŋ��܂� //
//	//cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
//
//	//// �J������]�s�� //
//	//XMMATRIX matCameraRot;
//
//	//// �J�������W�n�����[���h���W�n�̕ϊ��s�� //
//	//matCameraRot.r[0] = cameraAxisX;
//	//matCameraRot.r[1] = cameraAxisY;
//	//matCameraRot.r[2] = cameraAxisZ;
//	//matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
//
//	//XMMATRIX matBillboard;
//
//	//if (type == BILLBOARDX) {
//
//	//}
//
//	//else if (type == BILLBOARDY) {
//	//	// �J����X���AY���AZ�� //
//	//	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
//
//	//	// X���͋��� //
//	//	ybillCameraAxisX = cameraAxisX;
//
//	//	// Y���̓��[���h���W�n��Y�� //
//	//	ybillCameraAxisY = XMVector3Normalize(upVec);
//
//	//	// Z����X����Y���̊O�ςŋ��܂� //
//	//	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);
//
//	//	// Y�����̃r���{�[�h�s�� //
//	//	matBillboard.r[0] = ybillCameraAxisX;
//	//	matBillboard.r[1] = ybillCameraAxisY;
//	//	matBillboard.r[2] = ybillCameraAxisZ;
//	//	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
//	//}
//
//	//else if (type == BILLBOARDZ) {
//
//	//}
//
//	//else if (type == BILLBOARDALL) {
//	//	// �S�����r���{�[�h�s��̌v�Z //
//	//	matBillboard.r[0] = cameraAxisX;
//	//	matBillboard.r[1] = cameraAxisY;
//	//	matBillboard.r[2] = cameraAxisZ;
//	//	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
//	//}
//
//	// �]�u�ɂ��t�s��(�t��])���v�Z //
//	//matView = XMMatrixTranspose(matCameraRot);
//
//	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
//	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
//	//HRESULT result;
//
//	// --�X�P�[���A��]�A���s�ړ��s��̌v�Z-- //
//	XMMATRIX matScale, matRot, matTrans;
//	matScale = XMMatrixScaling(scale_.x, scale_.y, 1.0f);
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation_));
//	matTrans = XMMatrixTranslation(position_.x, position_.y, position_.z);
//
//	// --���[���h�s��̍���-- //
//	XMMATRIX matWorld = XMMatrixIdentity();// -> �ό`�̃��Z�b�g
//	//matWorld *= matBillboard;
//	matWorld *= matScale;// -> ���[���h�s��ɃX�P�[�����O�𔽉f
//	matWorld *= matRot;// -> ���[���h�s��ɉ�]�𔽉f
//	matWorld *= matTrans;// -> ���[���h�s��ɕ��s�ړ��𔽉f
//
//	// --�}�b�s���O-- //
//	//ObjectBuff* constMap = nullptr;
//	//result = constBuff_->Map(0, nullptr, (void**)&constMap);
//	//assert(SUCCEEDED(result));
//
//	// --�萔�o�b�t�@�փf�[�^�]��-- //
//	//constMap->mat = matWorld * camera->GetMatView() * matProjection_;// -> �s��
//
//	// --�q���������-- //
//	//constBuff_->Unmap(0, nullptr);
//}
//
//void BillBoard::Draw(int textureHandle)
//{
//	// �R�}���h���X�g�擾
//	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();
//
//	// SRV�q�[�v�̃n���h�����擾
//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
//
//	// �n���h�����w�肳�ꂽ���܂Ői�߂�
//	srvGpuHandle.ptr += textureHandle;
//
//	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
//	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
//
//	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
//	cmdList->IASetVertexBuffers(0, 1, &vbView_);
//
//	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
//	cmdList->IASetIndexBuffer(&ibView_);
//
//	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
//	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
//
//	// �`��R�}���h
//	cmdList->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
//}
//
//void BillBoard::PreDraw()
//{
//	//// �p�C�v���C���X�e�[�g�̐ݒ�
//	//DX12Cmd::GetCmdList()->SetPipelineState(DX12Cmd::GetBillBoardPipeline().pipelineState.Get());
//	//// ���[�g�V�O�l�`���̐ݒ�
//	//DX12Cmd::GetCmdList()->SetGraphicsRootSignature(DX12Cmd::GetBillBoardPipeline().rootSignature.Get());
//	//// �v���~�e�B�u�`���ݒ�
//	//DX12Cmd::GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//// --�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h-- //
//	//ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
//	//DX12Cmd::GetCmdList()->SetDescriptorHeaps(1, ppHeaps);
//}
//
//void BillBoard::CreateSquare()
//{
//	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
//	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
//	//HRESULT result;
//
//#pragma region ���_�f�[�^�쐬
//	// �����̒��_�f�[�^
//	Vertex3D vertices[] = {
//		// �O��
//		{{ -0.5f, -0.5f, -0.5f}, {}, {0.0f, 1.0f}},// -> ���� 0
//		{{ -0.5f,  0.5f, -0.5f}, {}, {0.0f, 0.0f}},// -> ���� 1
//		{{  0.5f, -0.5f, -0.5f}, {}, {1.0f, 1.0f}},// -> �E�� 2
//		{{  0.5f,  0.5f, -0.5f}, {}, {1.0f, 0.0f}},// -> �E�� 3
//	};
//
//	// ���_�f�[�^���R�s�[
//	for (size_t i = 0; i < _countof(vertices); i++) {
//		vertices_.push_back(vertices[i]);
//	}
//#pragma endregion
//
//#pragma region �C���f�b�N�X�f�[�^�쐬
//	// �����̃C���f�b�N�X�f�[�^
//	uint16_t indices[] = {
//		// �O��
//		0, 1, 2,// -> �O�p�`1��
//		2, 1, 3,// -> �O�p�`2��
//	};
//
//	// �C���f�b�N�X�f�[�^���R�s�[
//	for (size_t i = 0; i < _countof(indices); i++) {
//		indices_.push_back(indices[i]);
//	}
//#pragma endregion
//
//#pragma region �@���v�Z
//	for (size_t i = 0; i < indices_.size() / 3; i++) {
//		// --�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
//		unsigned short index0 = indices_[i * 3 + 0];
//		unsigned short index1 = indices_[i * 3 + 1];
//		unsigned short index2 = indices_[i * 3 + 2];
//
//		// --�O�p�`���\�����钸�_���W���x�N�g���ɑ��
//		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
//		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
//		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);
//
//		// --p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
//		XMVECTOR v1 = XMVectorSubtract(p1, p0);
//		XMVECTOR v2 = XMVectorSubtract(p2, p0);
//
//		// --�O�ς͗������琂���ȃx�N�g��
//		XMVECTOR normal = XMVector3Cross(v1, v2);
//
//		// --���K��
//		normal = XMVector3Normalize(normal);
//
//		// --���߂��@���𒸓_�f�[�^�ɑ��
//		XMStoreFloat3(&vertices_[index0].normal, normal);
//		XMStoreFloat3(&vertices_[index1].normal, normal);
//		XMStoreFloat3(&vertices_[index2].normal, normal);
//	}
//#pragma endregion
//
//	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
//	CreateBuffer();
//}
//
//void BillBoard::CreateBuffer()
//{
//	// �f�o�C�X�擾
//	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();
//
//	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
//	HRESULT result;
//
//#pragma region ���_�o�b�t�@�쐬
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());
//
//	// --���_�o�b�t�@�̐ݒ�-- //
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//
//	// --���\�[�X�ݒ�-- //
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// --���_�o�b�t�@�̐���-- //
//	result = device->CreateCommittedResource(
//		&heapProp, // �q�[�v�ݒ�
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc, // ���\�[�X�ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff_));
//	assert(SUCCEEDED(result));
//
//	// --���_�o�b�t�@�r���[�̍쐬-- //
//	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();// -> GPU���z�A�h���X
//	vbView_.SizeInBytes = sizeVB;// -> ���_�o�b�t�@�̃T�C�Y
//	vbView_.StrideInBytes = sizeof(vertices_[0]);// -> ���_1���̃f�[�^�T�C�Y
//
//	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
//	Vertex3D* vertMap = nullptr;
//	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//
//	// --�S���_�ɑ΂���-- //
//	for (int i = 0; i < vertices_.size(); i++)
//	{
//		vertMap[i] = vertices_[i]; // ���W���R�s�[
//	}
//
//	// --�q���������-- //
//	vertBuff_->Unmap(0, nullptr);
//#pragma endregion
//
//#pragma region �C���f�b�N�X�o�b�t�@�쐬
//	// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
//	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices_.size());
//
//	// --���\�[�X�ݒ�-- //
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// --�C���f�b�N�X�o�b�t�@�̐���-- //
//	result = device->CreateCommittedResource(
//		&heapProp,// -> �q�[�v�ݒ�
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc,// -> ���\�[�X�ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff_)
//	);
//
//	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
//	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
//	ibView_.Format = DXGI_FORMAT_R16_UINT;
//	ibView_.SizeInBytes = sizeIB;
//
//	// --�C���f�b�N�X�o�b�t�@���}�b�s���O-- //
//	uint16_t* indexMap = nullptr;
//	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
//	assert(SUCCEEDED(result));
//
//	// --�S�C���f�b�N�X�ɑ΂���-- //
//	for (size_t i = 0; i < indices_.size(); i++)
//	{
//		indexMap[i] = indices_[i];
//	}
//
//	// --�}�b�s���O����-- //
//	indexBuff_->Unmap(0, nullptr);
//#pragma endregion
//}
