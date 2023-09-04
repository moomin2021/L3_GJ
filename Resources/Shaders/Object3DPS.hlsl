#include "Object3D.hlsli"

Texture2D<float4> tex : register(t0); // -> 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // ------> 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input)
{
    PSOutput output;
    
    // �e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv) * input.color;
    
    // ����x
    const float shininess = 20.0f;
    
    // ���_���王�_�ւ̕����x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    
    // �V�F�[�f�B���O�ɂ��F
    float4 shadecolor = float4(0.0f, 0.0f, 0.0f, m_alpha);
    
    // ���s����
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            // ���ˌ��x�N�g��
            float3 reflect = normalize(-dirLights[i].lightv + 2.0f * input.normal * dot(input.normal, dirLights[i].lightv));
            
            // ���̖̂ʂ̖@���Ƒ��z�̈ʒu�������x�N�g���ǂ̂��炢�d�Ȃ��Ă��邩���v�Z
			// �d�Ȃ��Ă���Ώd�Ȃ��Ă���قǖ��邢�I
			// saturate�͒l��0-1�ɃN�����v����
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
            
            // �g�U���ˌ�
            float3 diffuse = texcolor.rgb * intensity * dirLights[i].lightcolor;
            
            // ���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * dirLights[i].lightcolor * m_specular;
            
            // ���� = �f�ނ̐F �~ ���C�g�̐F * �Â����邽�߂�
            float3 ambient = texcolor.rgb * dirLights[i].lightcolor * 0.3;
            
            // �S�ĉ��Z����
            shadecolor.rgb += diffuse + specular + ambient;
        }
    }
    
    // �_����
    for (i = 0; i < POINTLIGHT_NUM; i++)
    {
        if (pointLights[i].active)
        {
            // ���C�g�ւ̃x�N�g��
            float3 lightv = pointLights[i].lightpos - input.worldPos.xyz;
            
            // �x�N�g���̒���
            float d = length(lightv);
            
            // ���K�����A�P�ʃx�N�g���ɂ���
            lightv = normalize(lightv);
            
            // ���������W��
            float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
            
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
            
            // ���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2.0f * dotlightnormal * input.normal);
            
            // �g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
            
            // ���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            //�@���ׂĉ��Z����
            shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
        }
    }
    
    // �X�|�b�g���C�g
    for (i = 0; i < SPOTLIGHT_NUM; i++)
    {
        if (spotLights[i].active)
        {
            // ���C�g�ւ̕����x�N�g��
            float3 lightv = spotLights[i].lightpos - input.worldPos.xyz;
            float d = length(lightv);
            lightv = normalize(lightv);
        
            // ���������W��
            float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
        
            // �p�x����
            float cos = dot(lightv, spotLights[i].lightv);
        
            // �����J�n�p�x����A�����I���p�x�ɂ����Č���
            // �����J�n�p�x�̓�����1�{�����I���p�x�̊O����0�{�̋P�x
            float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
        
            // �p�x��������Z
            atten *= angleatten;
        
            // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
            float3 dotlightnormal = dot(lightv, input.normal);
            
            // ���ˌ��x�N�g��
            float3 reflect = normalize(-lightv + 2.0f * dotlightnormal * input.normal);
            
            // �g�U���ˌ�
            float3 diffuse = dotlightnormal * m_diffuse;
            
            // ���ʔ��ˌ�
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

            // ���ׂĂ����Z����
            shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
        }
    }
    
    // �ۉe
    for (i = 0; i < CIRCLESHADOW_NUM; i++)
    {
        if (circleShadows[i].active)
        {
            //�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
            float3 casterv = circleShadows[i].casterPos - input.worldPos.xyz;
            //���e�����ł̋���
            float d = dot(casterv, circleShadows[i].dir);
            //���������W��
            float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
            //�������}�C�i�X�Ȃ�0�ɂ���
            atten *= step(0, d);
            //���z���C�g�̍��W
            float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
            //�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
            float3 lightv = normalize(lightpos - input.worldPos.xyz);
            //�p�x����
            float cos = dot(lightv, circleShadows[i].dir);
            //�����J�n�e������A�����I���p�x�ɂ����Č���
            //�����J�n�p�x�̓�����1�{�A�����I���p�x�̊O����0�{�̋P�x
            float angleatten =
            smoothstep(circleShadows[i].factorAngleCos.y,
            circleShadows[i].factorAngleCos.x, cos);
            //�p�x��������Z
            atten *= angleatten;
            //���ׂČ��Z����
            shadecolor.rgb -= atten;
        }

    }

    // �V�F�[�f�B���O�ɂ��F�ŕ`��
    output.target0 = shadecolor;
    output.target1 = shadecolor;
    return output;
}