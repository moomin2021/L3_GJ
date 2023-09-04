#include "PipelineManager.h"
#include "DX12Cmd.h"
#include "Texture.h"

std::map<std::string, std::unique_ptr<PipelineObj>> PipelineManager::pipelineObj_ = {};

PipelineManager* PipelineManager::GetInstance()
{
	// �C���X�^���X����
	static PipelineManager inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void PipelineManager::PreDraw(std::string pipelineName, D3D_PRIMITIVE_TOPOLOGY primitiveType, bool isDescHeap)
{
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// SRV�q�[�v�擾
	ID3D12DescriptorHeap* srvHeap = Texture::GetInstance()->GetSRVHeap();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineObj_[pipelineName]->GetPipelineState());

	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipelineObj_[pipelineName]->GetRootSignature());

	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(primitiveType);

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	if (isDescHeap) {
		std::vector<ID3D12DescriptorHeap*> ppHeaps = { srvHeap };
		cmdList->SetDescriptorHeaps(1, ppHeaps.data());
	}
}

PipelineManager::PipelineManager() {
	AddPipeline("Object3D");
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DPS.hlsl", PS);
	pipelineObj_["Object3D"]->LoadShader("Resources/Shaders/Object3DVS.hlsl", VS);
	pipelineObj_["Object3D"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Object3D"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Object3D"]->CreateRootParams(1, 3);
	pipelineObj_["Object3D"]->CreatePipeline(2);

	AddPipeline("Particle");
	pipelineObj_["Particle"]->LoadShader("Resources/Shaders/ParticlePS.hlsl", PS);
	pipelineObj_["Particle"]->LoadShader("Resources/Shaders/ParticleVS.hlsl", VS);
	pipelineObj_["Particle"]->LoadShader("Resources/Shaders/ParticleGS.hlsl", GS);
	pipelineObj_["Particle"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	pipelineObj_["Particle"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32_FLOAT);
	pipelineObj_["Particle"]->CreateRootParams(1, 1);
	pipelineObj_["Particle"]->CreatePipeline(2, PipelineObj::ADD,  D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, false);

	AddPipeline("Sprite");
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpritePS.hlsl", PS);
	pipelineObj_["Sprite"]->LoadShader("Resources/Shaders/SpriteVS.hlsl", VS);
	pipelineObj_["Sprite"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Sprite"]->CreateRootParams(1, 1);
	pipelineObj_["Sprite"]->CreatePipeline(1);

	AddPipeline("Gaussian");
	pipelineObj_["Gaussian"]->LoadShader("Resources/Shaders/GaussianPS.hlsl", PS);
	pipelineObj_["Gaussian"]->LoadShader("Resources/Shaders/GaussianVS.hlsl", VS);
	pipelineObj_["Gaussian"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Gaussian"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Gaussian"]->CreateRootParams(2, 1);
	pipelineObj_["Gaussian"]->CreatePipeline(2);

	AddPipeline("HighLumi");
	pipelineObj_["HighLumi"]->LoadShader("Resources/Shaders/HighLumiPS.hlsl", PS);
	pipelineObj_["HighLumi"]->LoadShader("Resources/Shaders/HighLumiVS.hlsl", VS);
	pipelineObj_["HighLumi"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["HighLumi"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["HighLumi"]->CreateRootParams(2, 1);
	pipelineObj_["HighLumi"]->CreatePipeline(2);

	AddPipeline("Bloom");
	pipelineObj_["Bloom"]->LoadShader("Resources/Shaders/BloomPS.hlsl", PS);
	pipelineObj_["Bloom"]->LoadShader("Resources/Shaders/BloomVS.hlsl", VS);
	pipelineObj_["Bloom"]->AddInputLayout("POSITION", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Bloom"]->AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	pipelineObj_["Bloom"]->CreateRootParams(2, 1);
	pipelineObj_["Bloom"]->CreatePipeline(1);
}

void PipelineManager::AddPipeline(std::string pipelineName)
{
	std::unique_ptr<PipelineObj> obj = std::make_unique<PipelineObj>();
	pipelineObj_.insert(std::make_pair(pipelineName, std::move(obj)));
}