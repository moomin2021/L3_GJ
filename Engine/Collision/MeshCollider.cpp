#include "MeshCollider.h"
#include "Collision.h"

MeshCollider::MeshCollider(Object3D* object)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_MESH;

	// �I�u�W�F�N�g��ݒ�
	object_ = object;

	// �O�p�`�̔z����\������
	ConstructTriangles(object_->GetModel());
}

void MeshCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �Փ˂����Ƃ��̏���������
	inter_ = { 0.0f, 0.0f, 0.0f };
	reject_ = { 0.0f, 0.0f, 0.0f };
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter, Vector3* reject)
{
	// �I�u�W�F�N�g�̃��[�J�����W�n�ł̋��𓾂�(���a��X�X�P�[�����Q��)
	Sphere localSphere;
	localSphere.center = sphere.center - object_->GetPosition();
	localSphere.radius = sphere.radius;

	// ���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles_.cbegin();

	for (; it != triangles_.cend(); ++it) {
		const Triangle& triangle = *it;

		// ���ƎO�p�`�̓����蔻��
		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter, reject)) {
			if (inter) {
				const Matrix4& matWorld = object_->GetMatWorld();

				// ���[���h���W�n�ł̌�_�𓾂�
				*inter = Vector3Transform(*inter, matWorld);
			}

			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector3* inter)
{
	// �I�u�W�F�N�g�̃��[�J�����W�n�ł̃��C�𓾂�
	Ray localRay;
	localRay.start = ray.start - object_->GetPosition();
	localRay.dir = ray.dir;

	// ���[�J�����W�n�Ō������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles_.cbegin();

	for (; it != triangles_.cend(); ++it) {
		const Triangle& triangle = *it;

		Vector3 tempInter;

		// ���C�ƎO�p�`�̓����蔻��
		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter)) {
			const Matrix4& matWorld = object_->GetMatWorld();

			// ���[���h���W�n�ł̌�_�𓾂�
			//tempInter = Vector3Transform(tempInter, matWorld);

			if (distance) {
				// ��_�ƃ��C�n�_�̋������v�Z
				Vector3 sub = tempInter - localRay.start;
				*distance = Vector3Dot(sub, localRay.dir);
			}

			if (inter) {
				*inter = tempInter;
			}

			return true;
		}
	}

	return false;
}

void MeshCollider::ConstructTriangles(Model* model)
{
	// �O�p�`���X�g���N���A
	triangles_.clear();

	// ���f���̎����b�V�����X�g���擾
	const std::vector<Mesh>& meshes = model->GetMesh();

	// �I�u�W�F�N�g�ɐݒ肳��Ă���X�P�[����ݒ�
	Vector3 scale = object_->GetScale();

	// ���݂̃��b�V���̊J�n�O�p�`�ԍ������Ă����ϐ�(0�ŏ�����)
	uint16_t start = 0;

	// �S���b�V���ɂ��ď��ɏ�������
	std::vector<Mesh>::const_iterator it = meshes.cbegin();

	for (; it != meshes.cend(); ++it) {
		// ���_�f�[�^
		const std::vector<Mesh::Vertex>& vertices = it._Ptr->GetVertex();

		// �C���f�b�N�X
		const std::vector<uint16_t>& indexes = it._Ptr->GetIndex();

		// �C���f�b�N�X�́A�O�p�`�̐��~3����̂ŁA�������烁�b�V�����̎O�p�`�̐����t�Z����
		size_t triangleNum = indexes.size() / 3;

		// ���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
		triangles_.resize(triangles_.size() + triangleNum);

		// �S�O�p�`�ɂ��ď��ɏ�������
		for (size_t i = 0; i < triangleNum; i++) {
			// ������v�Z����O�p�`�̎Q��
			Triangle& tri = triangles_[start + i];
			uint16_t idx0 = indexes[i * 3 + 0];
			uint16_t idx1 = indexes[i * 3 + 1];
			uint16_t idx2 = indexes[i * 3 + 2];

			// �O�p�`��3���_�̍��W����
			tri.p0 = { vertices[idx0].pos.x * scale.x, vertices[idx0].pos.y * scale.y, vertices[idx0].pos.z * scale.z };
			tri.p1 = { vertices[idx1].pos.x * scale.x, vertices[idx1].pos.y * scale.y, vertices[idx1].pos.z * scale.z };
			tri.p2 = { vertices[idx2].pos.x * scale.x, vertices[idx2].pos.y * scale.y, vertices[idx2].pos.z * scale.z };

			// 3���_����̖@�����v�Z
			tri.ComputeNormal();
		}

		// ���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
		start += static_cast<uint16_t>(triangleNum);
	}
}