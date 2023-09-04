#pragma once
#include "Vector3.h"
#include "Vector2.h"

// ��
struct Sphere {
	// ���S���W
	Vector3 center = { 0.0f, 0.0f, 0.0f };

	// ���a
	float radius = 1.0f;
};

// ����
struct Plane {
	// �@���x�N�g��
	Vector3 normal = { 0.0f, 1.0f, 0.0f };

	// ���_(0.0f, 0.0f, 0.0f)����̋���
	float distance = 0.0f;
};

// �O�p�`
class Triangle {
public:
	// ���_���W3��
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;

	// �@���x�N�g��
	Vector3 normal;

	void ComputeNormal();
};

struct Ray {
	// �n�_���W
	Vector3 start = { 0.0f, 0.0f, 0.0f };

	// ����
	Vector3 dir = { 1.0f, 0.0f, 0.0f };
};

struct Point {
	// ���W
	Vector2 pos = { 0.0f, 0.0f };
};

struct Box {
	// �������W
	Vector2 center = { 0.0f, 0.0f };

	// ���a
	Vector2 radius = { 1.0f, 1.0f };
};

struct Circle {
	// �������W
	Vector2 center = { 0.0f, 0.0f };

	// ���a
	float radius = 1.0f;
};