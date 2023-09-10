#include "Bloom.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    //2.���o�����摜�Ƀu���[��������
    float totalWeight = 0, _Sigma = 0.002;
    float _StepWidth = _Sigma / 2.0f;
    float4 col = float4(0, 0, 0, 0);

                // xy��2���̕��ŐF���擾
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 1; px <= _Sigma * 1; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);

			// �E�F�C�g
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            totalWeight += weight;

            col += tex0.Sample(smp, pickUV) * weight;
        }
    }

    // �������u�d�݁v���A���ʂ�������
    col.rgb = col.rgb / totalWeight;
    
    float4 col1 = tex1.Sample(smp, input.uv);
    return col + col1;
}