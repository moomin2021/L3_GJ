#include "Collision.h"

#include <math.h>

bool Collision::CheckSphere2Sphere(const Sphere& sphere0, const Sphere& sphere1)
{
	float x = sphere1.center.x - sphere0.center.x;
	float y = sphere1.center.y - sphere0.center.y;
	float z = sphere1.center.z - sphere0.center.z;

	float r = sphere0.radius + sphere1.radius;

	// �^�Ȃ�Փ˂��Ă���
	if ((x * x) + (y * y) + (z * z) <= (r * r)) return true;

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = Vector3Dot(sphere.center, plane.normal);

	// ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ������S�Ƃ̋������o��
	float dist = distV - plane.distance;

	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) return false;

	// �^����_���v�Z
	if (inter) {
		// ���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = Vector3Dot(p0_p1, p0_pt);
	float d2 = Vector3Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p1;

	float d3 = Vector3Dot(p0_p1, p1_pt);
	float d4 = Vector3Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p2;

	float d5 = Vector3Dot(p0_p1, p2_pt);
	float d6 = Vector3Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter, Vector3* reject)
{
	Vector3 p;

	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);

	// �_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center;

	// �����̓������߂�
	// (�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	float d = Vector3Dot(v, v);

	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����ĂȂ�
	if (d > sphere.radius * sphere.radius) return false;

	// �^����_���v�Z
	if (inter) *inter = p;

	// �����o���x�N�g�����v�Z
	if (reject) {
		float ds = Vector3Dot(sphere.center, triangle.normal);
		float dt = Vector3Dot(triangle.p0, triangle.normal);
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	// �덷�z���p�̔����Ȓl
	const float epsilon = static_cast<float>(1.0e-5);

	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = Vector3Dot(plane.normal, ray.dir);

	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) return false;

	// �n�_�ƌ��_�̋���(���ʂ̖@������)
	// �ʖ@���ƃ��C�̎n�_���W
	float d2 = Vector3Dot(plane.normal, ray.start);

	// �n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;

	// �n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;

	// ��_���n�_�����ɂ���̂œ�����Ȃ�
	if (t < 0) return false;

	// ��������������
	if (distance) *distance = t;

	// ��_���v�Z
	if (inter) *inter = ray.start + t * ray.dir;

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = Vector3Dot(triangle.normal, triangle.p0);

	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) return false;

	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = static_cast<float>(1.0e-5);// �덷�z���ʂ̔����Ȓl

	Vector3 m;

	// ��p0_p1�ɂ���
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = Vector3Cross(pt_p0, p0_p1);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal) < -epsilon) return false;

	// ��p1_p2�ɂ���
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m = Vector3Cross(pt_p1, p1_p2);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal) < -epsilon) return false;

	// ��p2_p0�ɂ���
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	m = Vector3Cross(pt_p2, p2_p0);

	// �ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (Vector3Dot(m, triangle.normal) < -epsilon) return false;

	// �����Ȃ̂ŁA�������Ă���
	if (inter) *inter = interPlane;

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = Vector3Dot(m, ray.dir);
	float c = Vector3Dot(m, m) - sphere.radius * sphere.radius;

	// ray�̎n�_��sphere�̊O���ɂ���(c > 0)�Aray��sphere���痣��Ă����������w���Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) return false;

	float discr = b * b - c;

	// ���̔��莮�̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) return false;

	// ���C�͋��ƌ������Ă���
	// ��������ŏ��̒l���v�Z
	float t = -b - static_cast<float>(sqrt(discr));

	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) t = 0.0f;
	if (distance) *distance = t;

	if (inter) *inter = ray.start + t * ray.dir;

	return true;
}

bool Collision::CheckCircle2Circle(const Circle& circle0, const Circle& circle1)
{
	float x = circle0.center.x - circle1.center.x;
	float y = circle0.center.y - circle1.center.y;
	float c = sqrtf(x * x + y * y);

	if (c <= circle0.radius + circle1.radius) return true;
	return false;
}

bool Collision::CheckBox2Box(const Box& box0, const Box& box1)
{
	float x0 = box0.center.x - box0.radius.x;
	float x1 = box1.center.x + box1.radius.x;

	if (x0 > x1) return false;

	x0 = box0.center.x + box0.radius.x;
	x1 = box1.center.x - box1.radius.x;

	if (x0 < x1) return false;

	float y0 = box0.center.y - box0.radius.y;
	float y1 = box1.center.y + box1.radius.y;

	if (y0 > y1) return false;

	y0 = box0.center.y + box0.radius.y;
	y1 = box1.center.y - box1.radius.y;

	if (y0 < y1) return false;

	return true;
}

bool Collision::CheckCircle2Box(const Circle& circle, const Box& box)
{
	float boxX1 = box.center.x - box.radius.x;
	float boxX2 = box.center.x + box.radius.x;
	float boxY1 = box.center.y - box.radius.y;
	float boxY2 = box.center.y + box.radius.y;

	if ((boxX1 < circle.center.x) && (circle.center.x < boxX2) && (boxY1 - circle.radius < circle.center.y) && (circle.center.y < boxY2 + circle.radius)) return true;
	if ((boxX1 - circle.radius < circle.center.x) && (circle.center.x < boxX2 + circle.radius) && (boxY1 < circle.center.y) && (circle.center.y < boxY2)) return true;
	if (pow((double)boxX1 - circle.center.x, 2) + pow((double)boxY1 - circle.center.y, 2) < pow(circle.radius, 2)) return true;
	if (pow((double)boxX1 - circle.center.x, 2) + pow((double)boxY2 - circle.center.y, 2) < pow(circle.radius, 2)) return true;
	if (pow((double)boxX2 - circle.center.x, 2) + pow((double)boxY1 - circle.center.y, 2) < pow(circle.radius, 2)) return true;
	if (pow((double)boxX2 - circle.center.x, 2) + pow((double)boxY2 - circle.center.y, 2) < pow(circle.radius, 2)) return true;
	return false;
}

bool Collision::CheckPoint2Box(const Point& point, const Box& box)
{
	float x = box.center.x - box.radius.x;

	if (x > point.pos.x) return false;

	x = box.center.x + box.radius.x;

	if (x < point.pos.x) return false;

	float y = box.center.y - box.radius.y;

	if (y > point.pos.y) return false;

	y = box.center.y + box.radius.y;

	if (y < point.pos.y) return false;

	return true;
}

bool Collision::CheckPoint2Circle(const Point& point, const Circle& circle)
{
	float x = powf(point.pos.x - circle.center.x, 2);
	float y = powf(point.pos.y - circle.center.y, 2);

	if (x + y <= powf(circle.radius, 2)) return true;
	return false;
}
