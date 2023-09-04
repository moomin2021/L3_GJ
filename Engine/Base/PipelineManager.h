#pragma once
#include "PipelineObj.h"

#include <map>
#include <memory>
#include <string>

class PipelineManager
{
#pragma region �����o�ϐ�
private:
	static std::map<std::string, std::unique_ptr<PipelineObj>> pipelineObj_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static PipelineManager* GetInstance();


	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="pipelineName"> �p�C�v���C�����O </param>
	static void PreDraw(std::string pipelineName, D3D_PRIMITIVE_TOPOLOGY primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, bool isDescHeap = true);

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PipelineManager();

	/// <summary>
	/// �p�C�v���C����ǉ�
	/// </summary>
	/// <param name="pipelineName"> �p�C�v���C���̖��O </param>
	void AddPipeline(std::string pipelineName);
#pragma endregion

#pragma region ����֐�
	// �֎~
	PipelineManager(const PipelineManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	PipelineManager& operator = (const PipelineManager&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};