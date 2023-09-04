#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider : public BaseCollider
{
#pragma region �����o�ϐ�
private:
	// �O�p�`
	std::vector<Triangle> triangles_;

	// �Փ˂����Ƃ��̏��
	Vector3 inter_	= { 0.0f, 0.0f, 0.0f };// ��_
	Vector3 reject_ = { 0.0f, 0.0f, 0.0f };// �����o���x�N�g��
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	MeshCollider(Object3D* object);

	// �X�V����
	void Update() override;

	/// <summary>
	/// ���Ƃ̓����蔻��
	/// </summary>
	/// <param name="sphere"> �� </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);

	/// <summary>
	/// ���C�Ƃ̓����蔻��
	/// </summary>
	/// <param name="ray"> ���C </param>
	/// <param name="distance"> ����(�o�͗p) </param>
	/// <param name="inter"> ��_(�o�͗p) </param>
	/// <returns> �������Ă��邩�ۂ� </returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);

private:
	/// <summary>
	/// �O�p�`�̔z����\�z����
	/// </summary>
	/// <param name="model"> ���f�� </param>
	void ConstructTriangles(Model* model);
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// ��_��ݒ�
	inline void SetInter(const Vector3& inter) { inter_ = inter; }

	// �����o���x�N�g�������Z
	inline void AddReject(const Vector3& reject) { reject_ += reject; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ��_���擾
	inline const Vector3& GetInter() { return inter_; }

	// �����o���x�N�g�����擾
	inline const Vector3& GetReject() { return reject_; }
#pragma endregion
};