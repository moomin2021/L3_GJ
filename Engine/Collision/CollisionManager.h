#pragma once
#include "BaseCollider.h"

#include <forward_list>

class CollisionManager
{
#pragma region �����o�ϐ�
	// �R���C�_�[
	std::forward_list<BaseCollider*> colliders_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	CollisionManager() = default;

	// �f�X�g���N�^
	~CollisionManager();

	// �S�Ă̏Փ˃`�F�b�N
	void CheckAllCollision();

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="collider"> �R���C�_�[ </param>
	inline void AddCollider(BaseCollider* collider) { colliders_.push_front(collider); }

	/// <summary>
	/// �R���C�_�[�̍폜
	/// </summary>
	/// <param name="collider"></param>
	inline void RemoveCollider(BaseCollider* collider) { colliders_.remove(collider); }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	// �C���X�^���X�擾
	static CollisionManager* GetInstance();
#pragma endregion

#pragma region ����֐�
	CollisionManager(const CollisionManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	CollisionManager& operator = (const CollisionManager&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};