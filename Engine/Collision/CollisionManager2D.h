#pragma once
#include "BaseCollider2D.h"

#include <forward_list>

class CollisionManager2D
{
#pragma region �����o�ϐ�
	// �R���C�_�[
	std::forward_list<BaseCollider2D*> colliders_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	CollisionManager2D() = default;

	// �f�X�g���N�^
	~CollisionManager2D();

	// �S�Ă̏Փ˃`�F�b�N
	void CheckAllCollision();

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider"> �R���C�_�[ </param>
	inline void AddCollider(BaseCollider2D* collider) { colliders_.push_front(collider); }

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="collider"></param>
	inline void RemoveCollider(BaseCollider2D* collider) { colliders_.remove(collider); }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	// �C���X�^���X�擾
	static CollisionManager2D* GetInstance();
#pragma endregion

#pragma region ����֐�
	CollisionManager2D(const CollisionManager2D&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	CollisionManager2D& operator = (const CollisionManager2D&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};