#include "Gaussian.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

//float4 main(VSOutput input) : SV_TARGET
//{
//    // xy��2a�̕��ŐF���擾
//    float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
//    float4 col = float4(0, 0, 0, 0);

//    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
//    {
//        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
//        {
//            float2 pickUV = clamp(input.uv + float2(px, py), 0.01, 0.99);
//            float weight = Gaussian(input.uv, pickUV, _Sigma);

//            // Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
//            col += tex.Sample(smp, pickUV) * weight;

//            // �������u�d�݁v�̍��v�l���T���Ă���
//            totalWeight += weight;
//        }
//    }
//    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
//    col.a = 1;
    
//    // ���P�x���𒊏o
//    return col;
//}

//PSOutput main(VSOutput input)
//{
//    PSOutput output;
    
//    //2.���o�����摜�Ƀu���[��������
//    float totalWeight = 0, _Sigma = 0.002, _StepWidth = 0.0002;
//    float4 col = float4(0, 0, 0, 0);

//    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
//    {
//        for (float px = -_Sigma * 1; px <= _Sigma * 1; px += _StepWidth)
//        {
//            float2 pickUV = input.uv + float2(px, py);
//            float weight = Gaussian(input.uv, pickUV, _Sigma);
//            col += tex0.Sample(smp, pickUV) * weight; //Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
//            totalWeight += weight; //�������u�d�݁v�̍��v�l���T���Ă���
//        }
//    }
//    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
//    col.a = 1;
    
//    output.target0 = col;
//    output.target1 = tex1.Sample(smp, input.uv);
    
//    ////���P�x���𒊏o
//    return output;
    
//}

float4 main(VSOutput input) : SV_TARGET
{
    //2.���o�����摜�Ƀu���[��������
    float totalWeight = 0, _Sigma = 0.002;
    float _StepWidth = _Sigma / 10.0f;
    float4 col = float4(0, 0, 0, 0);

                // xy��2���̕��ŐF���擾
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            //float2 pickUV = clamp(input.uv + float2(px, py), 0.01, 0.99);
            //float weight = Gaussian(input.uv, pickUV, _Sigma);

            //             Gaussian�Ŏ擾�����u�d�݁v��F�ɂ�����
            //col0 += tex0.Sample(smp, pickUV) * weight;

            //             �������u�d�݁v�̍��v�l���T���Ă���
            //totalWeight += weight;
            
            float2 pickUV = input.uv + float2(px, py);
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);

			// �E�F�C�g
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            totalWeight += weight;

            col += tex0.Sample(smp, pickUV) * weight;
        }
    }
    col.rgb = col.rgb / totalWeight; //�������u�d�݁v���A���ʂ��犄��
    col.a = 1;
    
    ////���P�x���𒊏o
    return col;
    
}