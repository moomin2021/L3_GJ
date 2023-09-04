#include "Util.h"

#include <Windows.h>
#include <chrono>
#include <random>

using namespace std::chrono;

int Util::Clamp(int value, int max, int min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

float Util::Clamp(float value, float max, float min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}

float Util::Radian2Degree(float radian)
{
	return radian * 180.0f / PI;
}

float Util::Degree2Radian(float degree)
{
	return degree * PI / 180.0f;
}

std::vector<wchar_t> Util::StringToWideChar(const std::string& str)
{
	// �ŏI�I�ɕԂ��p�ϐ�
	std::vector<wchar_t> wchar;
	wchar.resize(128);// -> �T�C�Y��ݒ�

	// �ϊ�
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wchar.data(), static_cast<int>(wchar.size()));

	// �߂�l��Ԃ�
	return wchar;
}

uint16_t Util::GetRandomInt(uint16_t min, uint16_t max)
{
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<uint16_t> dist(min, max);
	return dist(engine);
}

float Util::GetRandomFloat(float min, float max) {
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(engine);
}

uint64_t Util::GetTimeSec()
{
	return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

uint64_t Util::GetTimrMSec()
{
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
