#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    // シェーディングによる色で描画
    return float4(tex.Sample(smp, input.uv)) * color;
}