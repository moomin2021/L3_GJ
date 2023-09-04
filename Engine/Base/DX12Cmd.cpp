#include "DX12Cmd.h"
#include "WinAPI.h"
#include "float4.h"
#include "Texture.h"

#include <cassert>
#include <thread>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"winmm.lib")

DX12Cmd* DX12Cmd::GetInstance()
{
	// �C���X�^���X����
	static DX12Cmd inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void DX12Cmd::Initialize()
{
	// ���ݎ��Ԃ��L�^����(FPS�Œ菈���p)
	reference_ = std::chrono::steady_clock::now();

	// �f�o�b�N���C���[�����ݒ�
	InitSetDebugLayer();

	// �A�_�v�^�̏����ݒ�
	InitSetAdapter();

	// �f�o�C�X����
	CreateDevice();

	// �G���[�ݒ�
	ErrorSet();

	// �R�}���h���X�g����
	CreateCmdList();

	// �R�}���h�L���[����
	CreateCmdQueue();

	// �X���b�v�`�F�[������
	CreateSwapChain();

	// �����_�[�^�[�Q�b�g�r���[����
	CreateRenderTargetView();

	// �t�F���X����
	CreateFence();

	// �[�x�e�X�g
	CreateDepthBuffer();
}

void DX12Cmd::PreDraw()
{
	// �C���X�^���X�擾
	WinAPI* winAPI = WinAPI::GetInstance();

#pragma region ���\�[�X�o���A�ŏ������݉\�ɕύX
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	barrierDesc_.Transition.pResource = backBuffers_[bbIndex].Get();		// �o�b�N�o�b�t�@���w��
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// �\����Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;// �`���Ԃ�
	cmdList_->ResourceBarrier(1, &barrierDesc_);
#pragma endregion

#pragma region �`���̕ύX

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	cmdList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

#pragma endregion

#pragma region ��ʃN���A R G B A
	/// �o�b�N�o�b�t�@�ɂ͑O��ɕ`�����G�����̂܂܎c���Ă���̂ŁA��U�w��F�œh��Ԃ�
	std::vector<FLOAT> clearColor = { 0.1f, 0.25, 0.5f, 0.0f }; // ���ۂ��F
	cmdList_->ClearRenderTargetView(rtvHandle, clearColor.data(), 0, nullptr);
	cmdList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion

#pragma region �r���[�|�[�g�ݒ�

	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = winAPI->GetWidth();
	viewport.Height = winAPI->GetHeight();
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList_->RSSetViewports(1, &viewport);

#pragma endregion

#pragma region �V�U�[��`

	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + winAPI->GetWidth();	// �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + winAPI->GetHeight();	// �؂蔲�����W��

	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList_->RSSetScissorRects(1, &scissorRect);

#pragma endregion
}

void DX12Cmd::PostDraw()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

#pragma region ���\�[�X�o���A��߂�

	// �o�b�N�o�b�t�@���������݉\��Ԃ����ʕ\����ԂɕύX����
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �`���Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �\����Ԃ�
	cmdList_->ResourceBarrier(1, &barrierDesc_);

	// �����܂ł��ߍ��񂾃R�}���h�����s���`�悷�鏈��
	{
		// ���߂̃N���[�Y
		result = cmdList_->Close();
		assert(SUCCEEDED(result));

		// �R�}���h���X�g�̎��s
		std::vector<ID3D12CommandList*> commandLists = { cmdList_.Get() };
		cmdQueue_->ExecuteCommandLists(1, commandLists.data());

		// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
		result = swapChain_->Present(1, 0);
		assert(SUCCEEDED(result));
	}

	// �R�}���h�̎��s������҂�
	cmdQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// FPS�Œ�
	UpdateFixFPS();

	// �L���[���N���A
	// ���̎g�p�ɔ����ăR�}���h�A���P�[�^�ƃR�}���h���X�g�����Z�b�g���Ă���
	result = cmdAllocator_->Reset();
	assert(SUCCEEDED(result));

	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = cmdList_->Reset(cmdAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result));

	Texture::GetInstance()->ReleaseIntermediateResources();

#pragma endregion

}

DX12Cmd::DX12Cmd() {}

void DX12Cmd::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));

	// 1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// ���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	// �O��L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60�b(���킸���ɒZ������)�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinCheckTime) {
		// 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	// ���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void DX12Cmd::InitSetDebugLayer()
{
#ifdef _DEBUG
	// �f�o�b�N�R���g���[��
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();				// �f�o�b�N���C���[��[ON]�ɂ���
		debugController->SetEnableGPUBasedValidation(TRUE);	// �V�F�[�_�[�`�F�b�N��[ON]�ɂ���

		// ��Debug�ŏd���Ȃ�����V�F�[�_�[�`�F�b�N��[OFF]�ɂ���Ƃ���
	}
#endif
}

void DX12Cmd::InitSetAdapter()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬
	HRESULT result;

	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// �A�_�v�^�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// �p�t�H�[�}���X���������̂��珇�ɁA���ׂẴA�_�v�^��񋓂���
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter_);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;

		// �A�_�v�^�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter_ = adapters[i];
			break;
		}
	}
}

void DX12Cmd::CreateDevice()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �Ή����x���̔z��
	std::vector<D3D_FEATURE_LEVEL> levels = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	// �O���t�B�b�N�f�o�C�X�𑀍삷��ׂ̃I�u�W�F�N�g����
	D3D_FEATURE_LEVEL featureLevel;
	// ������͕��ʁA1�Q�[����1�������Ȃ�

	for (size_t i = 0; i < levels.size(); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter_.Get(), levels[i],
			IID_PPV_ARGS(&device_));

		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
}

void DX12Cmd::ErrorSet()
{
#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	// ��΂��G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);		// �G���[�̎��~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);	// �x���̎��~�܂�
	}

	// �}������G���[
	std::vector<D3D12_MESSAGE_ID> denyIds = {
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	// �}�������\�����x��
	std::vector<D3D12_MESSAGE_SEVERITY> severities = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = static_cast<UINT>(denyIds.size());
	filter.DenyList.pIDList = denyIds.data();
	filter.DenyList.NumSeverities = static_cast<UINT>(severities.size());
	filter.DenyList.pSeverityList = severities.data();

	// �w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif
}

void DX12Cmd::CreateCmdList() {
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,	// �R�}���h�A���P�[�^�̎��
		IID_PPV_ARGS(&cmdAllocator_));	// �e�C���^�[�t�F�C�X�ŗL��GUID
	assert(SUCCEEDED(result));			// ID3D12CommandAllocator�C���^�[�t�F�C�X�̃|�C���^���i�[����ϐ��A�h���X

	// ���R�}���h���X�g�̓R�}���h�A���P�[�^���琶������̂ŁA��ɃR�}���h�A���P�[�^�����
	// ���R�}���h���X�g�Ɋi�[���閽�߂ׂ̈̃��������Ǘ�����I�u�W�F�N�g

	// �R�}���h���X�g�𐶐�
	result = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&cmdList_));
	assert(SUCCEEDED(result));
}

void DX12Cmd::CreateCmdQueue() {
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// ��{}�����邱�Ƃō\���̂̒��g��0�ŃN���A���Ă���B
	// ���l0���W���l�ɂȂ�悤��Microsoft�����߂Ă���̂ŁA���̏ꍇ�R�}���h�L���[�̐ݒ��W���ɂ��Ă��� //

	// �W���ݒ�ŃR�}���h�L���[�𐶐�
	result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));
	assert(SUCCEEDED(result));
}

void DX12Cmd::CreateSwapChain() {
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// WinAPI
	WinAPI* winAPI = WinAPI::GetInstance();

	// ��ʂ̕��w��
	swapChainDesc_.Width = winAPI->GetWidth();	// ��ʂ̉������w�肷��
	swapChainDesc_.Height = winAPI->GetHeight();// ��ʂ̏c�����w�肷��

	// �F���̏����i�\���`���j
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//��DXGI_FORMAT_R8G8B8A8_UNORM�̓A���t�@���܂ރ`�����l��������8�r�b�g���T�|�[�g

	// �s�N�Z��������̃}���`�T���v���̐����w�肷��
	swapChainDesc_.SampleDesc.Count = 1;

	// ���\�[�X�̎g�p���@���w��
	swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;// ��DXGI_USAGE_BACK_BUFFER�̓��\�[�X���o�b�N�o�b�t�@�Ƃ��Ďg�p����

	// �X���b�v�`�F�[�����̃o�b�t�@�[�̐����w�肷��
	swapChainDesc_.BufferCount = 2;

	// ��ʂ��X���b�v������̃��\�[�X�̏������@���w��
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;// ��DXGI_SWAP_EFFECT_FLIP_DISCARD�̓X���b�v������o�b�N�o�b�t�@�[�̓��e��j������ݒ�

	// �X���b�v�`�F�[������̃I�v�V�������w��
	swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// ��DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH�̓E�B���h�E�ƃt���X�N���[���̐؂�ւ�����>>
	// >>�𑜓x���E�B���h�E�T�C�Y�Ɉ�v����悤�ɕύX����

	// IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapChain1;

	// �X���b�v�`�F�[���̐���
	result = dxgiFactory_->CreateSwapChainForHwnd(
		cmdQueue_.Get(), winAPI->GetHWND(), &swapChainDesc_, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain1);
	assert(SUCCEEDED(result));

	// ��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapChain1.As(&swapChain_);
}

void DX12Cmd::CreateRenderTargetView()
{
	// �������_�[�^�[�Q�b�g�r���[�̓f�X�N���v�^�q�[�v�ɐ�������̂ŁA��Ƀf�X�N���v�^�q�[�v�����

	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount; // ���\��2��

	// �f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));

	// �o�b�N�o�b�t�@
	backBuffers_.resize(swapChainDesc_.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));

		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();

		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);

		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, rtvHandle);
	}
}

void DX12Cmd::CreateFence()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �t�F���X����
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
}

void DX12Cmd::CreateDepthBuffer()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// WinAPI
	WinAPI* winAPI = WinAPI::GetInstance();

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winAPI->GetWidth();	// �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = winAPI->GetHeight();	// �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;// �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;// �f�v�X�X�e���V��

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;		// �[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g

	// ���\�[�X����
	result = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// �[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;						// �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	// �f�v�X�X�e���V���r���[
	result = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;// �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);
}