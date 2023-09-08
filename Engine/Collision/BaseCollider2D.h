#pragma once
#include "CollisionShapeType.h"
#include "Sprite.h"

#include <stdint.h>

class BaseCollider2D {
public:
	friend class CollisionManager2D;

#pragma region �����o�ϐ�
protected:
	// �`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// �Փ˔��葮��
	uint16_t attribute_ = 0;

	// �C�ӂŕR�t����X�v���C�g
	Sprite* sprite_ = nullptr;

	// �Փ˃t���O
	bool isHit_ = false;

	// �Փ˂����R���C�_�[
	BaseCollider2D* hitCol_ = nullptr;

	// �^�O
	uint16_t tag_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	virtual ~BaseCollider2D() = default;

	// �X�V����
	virtual void Update() = 0;
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �`��^�C�v��ݒ�
	inline void SetShapeType(CollisionShapeType shapeType) { shapeType_ = shapeType; }

	// �Փ˔��葮����ݒ�
	inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }

	// �X�v���C�g��ݒ�
	inline void SetSprite(Sprite* sprite) { sprite_ = sprite; }

	// �Փ˃t���O��ݒ�
	inline void SetIsHit(bool flag) { isHit_ = flag; }

	// �Փ˂����R���C�_�[��ݒ�
	inline void SetHitCollider(BaseCollider2D* collider) { hitCol_ = collider; }

	// �^�O��ݒ�
	inline void SetTag(uint16_t tag) { tag_ = tag; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �`��^�C�v���擾
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	// �Փ˔��葮�����擾
	inline uint16_t GetAttribute() { return attribute_; }

	// �Փ˃t���O���擾
	inline bool GetIsHit() { return isHit_; }

	// �Փ˂����R���C�_�[���擾
	inline BaseCollider2D* GetHitCollider() { return hitCol_; }

	// ���W���擾
	Vector2 GetPosition() const;

	// �^�O���擾
	inline uint16_t GetTag() { return tag_; }
#pragma endregion
};