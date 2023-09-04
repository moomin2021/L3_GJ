#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class CircleCollider : public BaseCollider2D, public Circle
{
#pragma region �����o�ϐ�
private:

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	CircleCollider(Vector2 center = { 0.0f, 0.0f }, float radius = 1.0f);

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// ���W��ݒ�
	inline void SetCenter(const Vector2& center) { Circle::center = center; }

	// ���a��ݒ�
	inline void SetRadius(float radius) { Circle::radius = radius; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ���W���擾
	inline const Vector2& GetCenter() { return Circle::center; }

	// ���a���擾
	inline float GetRadius() { return Circle::radius; }
#pragma endregion
};