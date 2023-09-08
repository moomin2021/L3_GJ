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
//    // xyで2aの幅で色を取得
//    float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
//    float4 col = float4(0, 0, 0, 0);

//    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
//    {
//        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
//        {
//            float2 pickUV = clamp(input.uv + float2(px, py), 0.01, 0.99);
//            float weight = Gaussian(input.uv, pickUV, _Sigma);

//            // Gaussianで取得した「重み」を色にかける
//            col += tex.Sample(smp, pickUV) * weight;

//            // かけた「重み」の合計値を控えておく
//            totalWeight += weight;
//        }
//    }
//    col.rgb = col.rgb / totalWeight; //かけた「重み」分、結果から割る
//    col.a = 1;
    
//    // 高輝度部を抽出
//    return col;
//}

//PSOutput main(VSOutput input)
//{
//    PSOutput output;
    
//    //2.抽出した画像にブラーをかける
//    float totalWeight = 0, _Sigma = 0.002, _StepWidth = 0.0002;
//    float4 col = float4(0, 0, 0, 0);

//    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
//    {
//        for (float px = -_Sigma * 1; px <= _Sigma * 1; px += _StepWidth)
//        {
//            float2 pickUV = input.uv + float2(px, py);
//            float weight = Gaussian(input.uv, pickUV, _Sigma);
//            col += tex0.Sample(smp, pickUV) * weight; //Gaussianで取得した「重み」を色にかける
//            totalWeight += weight; //かけた「重み」の合計値を控えておく
//        }
//    }
//    col.rgb = col.rgb / totalWeight; //かけた「重み」分、結果から割る
//    col.a = 1;
    
//    output.target0 = col;
//    output.target1 = tex1.Sample(smp, input.uv);
    
//    ////高輝度部を抽出
//    return output;
    
//}

float4 main(VSOutput input) : SV_TARGET
{
    //2.抽出した画像にブラーをかける
    float totalWeight = 0, _Sigma = 0.002;
    float _StepWidth = _Sigma / 10.0f;
    float4 col = float4(0, 0, 0, 0);

                // xyで2αの幅で色を取得
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            //float2 pickUV = clamp(input.uv + float2(px, py), 0.01, 0.99);
            //float weight = Gaussian(input.uv, pickUV, _Sigma);

            //             Gaussianで取得した「重み」を色にかける
            //col0 += tex0.Sample(smp, pickUV) * weight;

            //             かけた「重み」の合計値を控えておく
            //totalWeight += weight;
            
            float2 pickUV = input.uv + float2(px, py);
            
            // 画面外の色を取得しないように
            pickUV = clamp(pickUV, 0.001, 0.999);

			// ウェイト
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            totalWeight += weight;

            col += tex0.Sample(smp, pickUV) * weight;
        }
    }
    col.rgb = col.rgb / totalWeight; //かけた「重み」分、結果から割る
    col.a = 1;
    
    ////高輝度部を抽出
    return col;
    
}