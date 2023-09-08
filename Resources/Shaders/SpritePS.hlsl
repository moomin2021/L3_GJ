#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    // �V�F�[�f�B���O�ɂ��F�ŕ`��
    return float4(tex.Sample(smp, input.uv)) * color;
}