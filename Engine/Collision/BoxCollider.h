#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class BoxCollider : public BaseCollider2D, public Box
{
#pragma region �����o�ϐ�
private:

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	BoxCollider(Vector2 center = { 0.0f, 0.0f }, Vector2 radius = {0.0f, 0.0f});

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// ���W��ݒ�
	inline void SetCenter(const Vector2& center) { Box::center = center; }

	// ���a��ݒ�
	inline void SetRadius(const Vector2& radius) { Box::radius = radius; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ���W���擾
	inline const Vector2& GetCenter() { return Box::center; }

	// ���a���擾
	inline const Vector2& GetRadius() { return Box::radius; }
#pragma endregion
};