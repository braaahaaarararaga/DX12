Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D textureEnv : register(t4);
Texture2D textureIBL : register(t5);
SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Diffuse : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 lightDirection = float3(1.0, -1.0, 1.0);
    lightDirection = normalize(lightDirection);
    
    float4 albedo = texture0.Sample(sampler0, input.TexCoord);
    float3 normal = texture1.Sample(sampler0, input.TexCoord);
    float4 position = texture2.Sample(sampler0, input.TexCoord);
    float4 depth = texture3.Sample(sampler0, input.TexCoord);
    
    float light = saturate(0.5 - dot(normal, lightDirection) * 0.5);
    
    albedo.rgb *= light;
    
    float3 cameraPosition = float3(0.0, 0.0, -5.0);
    
    float3 eye = position.xyz - cameraPosition;
    float3 ref = reflect(eye, normal.xyz);
    ref = normalize(ref);
    //float spec = -dot(ref, lightDirection);
    //spec = saturate(spec);
    //spec = pow(spec, 20.0);
    //color.rgb += spec;
    
    float3 diffuse, specular;
    
    
    float roughness = .0;
    // env
    float2 envTexCoord;
    envTexCoord.x = -ref.x * .3 + .5;
    envTexCoord.y = -ref.y * .3 + .5;
    specular = textureEnv.Sample(sampler0, envTexCoord);
    
        //IBL
    float2 iblTexCoord;
    iblTexCoord.x = -normal.x * 0.3 + 0.5;
    iblTexCoord.y = -normal.y * 0.3 + 0.5;
    diffuse = albedo * textureIBL.Sample(sampler0, iblTexCoord);
  
    //メタリック係数
    float metallic = 0.99;
    //スペキュラ係数
    float spec = 0.99;
    //フレネル係数
    float3 f0 = lerp(0.08 * spec, albedo.rgb, metallic);
    
    //フレネル反射率
    float3 f = f0 + (1.0 - f0) * pow(1.0 - dot(normal.xyz, ref), 5);
    
    float4 color;
    color.rgb = diffuse * (1.0 - metallic) * (1.0 - f) + specular * f;
    color.a = 1.0;
    
    //明度
    color.rgb *= 1.5;
    
    return color;
}