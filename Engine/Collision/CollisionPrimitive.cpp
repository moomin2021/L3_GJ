#include "CollisionPrimitive.h"

void Triangle::ComputeNormal() {
	Vector3 p0_p1 = p1 - p0;
	Vector3 p0_p2 = p2 - p0;

	// �O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
	normal = Vector3Cross(p0_p1, p0_p2);
	normal = Vector3Normalize(normal);
}