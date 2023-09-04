#include "ImGuiManager.h"
#include "WinAPI.h"
#include "DX12Cmd.h"
#include "Texture.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

#include <cassert>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager inst;
	return &inst;
}

void ImGuiManager::Initialize()
{
	// �C���X�^���X�擾
	WinAPI* winAPI = WinAPI::GetInstance();
	DX12Cmd* dx12 = DX12Cmd::GetInstance();
	Texture* tex = Texture::GetInstance();

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// �f�X�N���v�^�q�[�v����
	HRESULT result = dx12->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	// ImGui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(winAPI->GetHWND());

	ImGui_ImplDX12_Init(dx12->GetDevice(), static_cast<uint32_t>(dx12->GetBackBufferNum()), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(), srvHeap_->GetCPUDescriptorHandleForHeapStart(), srvHeap_->GetGPUDescriptorHandleForHeapStart());

	ImGuiIO& io = ImGui::GetIO();

	// �W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Begin()
{
	// ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// �`��O����
	ImGui::Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �`��R�}���h�𔭍s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

ImGuiManager::ImGuiManager() {}

ImGuiManager::~ImGuiManager()
{
	// ��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	srvHeap_.Reset();
}