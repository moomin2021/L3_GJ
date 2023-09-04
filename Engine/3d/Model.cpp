#include "Model.h"
#include "Texture.h"
#include "DX12Cmd.h"

#include <fstream>
#include <sstream>

using namespace std;

Model::Model(string fileName) :
#pragma region ���������X�g
	// ���b�V���f�[�^
	meshes_{},

	// �}�e���A���f�[�^
	materials_{}
#pragma endregion
{
	// ���f���ǂݍ���
	LoadModel(fileName);
}

void Model::Draw() {
	for (auto& i : meshes_) {
		materials_[i.GetMaterialName()].Draw();
		i.Draw();
	}
}

void Model::LoadModel(string name)
{
	// �t�@�C���X�g���[��
	std::ifstream file;

	// .obj�t�@�C�����J��
	const string modelName = name;
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + fileName);

	// �t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());

	vector<Vector3> positions;	// ���_���W
	vector<Vector3> normals;		// �@���x�N�g��
	vector<Vector2> texcoords;	// �e�N�X�`��UV

	// ���b�V���𐶐�������J�E���g
	uint16_t meshCount = 0;
	uint16_t loopCount = 0;

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪������[o]�Ȃ璸�_�f�[�^�����Z�b�g
		if (key == "o") {
			// ���b�V������
			meshes_.emplace_back();
			meshCount++;
			loopCount = 0;
		}

		// �擪������[v]�Ȃ璸�_���W
		else if (key == "v") {
			// X, Y, Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// ���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}

		// �擪������[vt]�Ȃ�e�N�X�`��
		else if (key == "vt") {
			// U, V�����ǂݍ���
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			// V�������]
			texcoord.y = 1.0f - texcoord.y;

			// �e�N�X�`�����W�f�[�^�ǉ�
			texcoords.emplace_back(texcoord);
		}

		// �擪������[vn]�Ȃ�@���x�N�g��
		else if (key == "vn") {
			// X, Y, Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		// �擪������[f]�Ȃ�|���S��(�O�p�`)
		else if (key == "f") {
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {

				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);// -> �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);// -> �X���b�V�����΂�
				index_stream >> indexNormal;

				// ���_�f�[�^�̒ǉ�
				Mesh::Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				meshes_[meshCount - 1].AddVertex(vertex);

				// ���_�C���f�b�N�X�ɒǉ�
				meshes_[meshCount - 1].AddIndex(loopCount);
				loopCount++;
			}
		}

		// �擪������[usemtl]�Ȃ�}�e���A��
		else if (key == "usemtl") {
			std::string materialName;
			line_stream >> materialName;

			// �}�e���A���̖��O�����b�V���ɐݒ�
			meshes_[meshCount - 1].SetMaterialName(materialName);
		}

		// �擪������[mtllib]�Ȃ�}�e���A��
		else if (key == "mtllib") {
			string fileName;
			line_stream >> fileName;

			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, fileName);
		}
	}

	// �����������ׂẴ��b�V���̒��_�o�b�t�@�ƃC���f�b�N�X�f�[�^�̐���
	for (size_t i = 0; i < meshes_.size(); i++) {
		meshes_[i].CreateVertexBuff();
		meshes_[i].CreateIndexBuff();
	}

	// �t�@�C�������
	file.close();
}

void Model::LoadMaterial(const string& directoryPath, const string& fileName) {
	// �t�@�C���X�g���[��
	ifstream file;

	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + fileName);

	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) assert(0);

	std::string materialName;

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {
		// 1�s���̕�������X�g���[���ɕϊ�
		istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		// �擪������[newmtl]�Ȃ�}�e���A����
		if (key == "newmtl") {
			// �}�e���A�����ǂݍ���
			line_stream >> materialName;

			// �}�e���A���ǉ�
			materials_.emplace(materialName, Material());
		}

		// �擪������[Ka]�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> materials_[materialName].ambient_.x;
			line_stream >> materials_[materialName].ambient_.y;
			line_stream >> materials_[materialName].ambient_.z;
		}

		// �擪������[Kd]�Ȃ�f�B�t�F�[�Y�E
		if (key == "Kd") {
			line_stream >> materials_[materialName].diffuse_.x;
			line_stream >> materials_[materialName].diffuse_.y;
			line_stream >> materials_[materialName].diffuse_.z;
		}

		// �擪������[Ks]�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> materials_[materialName].specular_.x;
			line_stream >> materials_[materialName].specular_.y;
			line_stream >> materials_[materialName].specular_.z;
		}

		// �擪������[map_Kd]�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			std::string texName;
			line_stream >> texName;

			// �e�N�X�`���ǂݍ���
			materials_[materialName].texHandle_ = LoadTexture(directoryPath + texName);
		}
	}

	// �S�Ẵ}�e���A���f�[�^�̃o�b�t�@�𐶐�
	for (auto it = materials_.begin(); it != materials_.end(); ++it) {
		materials_[it->first].CreateMaterialBuff();
	}

	// �t�@�C�������
	file.close();
}