Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);
Texture2D texture3 : register(t3);
Texture2D textureEnv : register(t4);
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
    
    float4 color = texture0.Sample(sampler0, input.TexCoord);
    float3 normal = texture1.Sample(sampler0, input.TexCoord);
    float4 position = texture2.Sample(sampler0, input.TexCoord);
    float4 depth = texture3.Sample(sampler0, input.TexCoord);
    
    float light = saturate(0.5 - dot(normal, lightDirection) * 0.5);
    
    color.rgb *= light;
    
    float3 cameraPosition = float3(0.0, 0.0, -5.0);
    
    float3 eye = position.xyz - cameraPosition;
    float3 ref = reflect(eye, normal.xyz);
    ref = normalize(ref);
    float spec = -dot(ref, lightDirection);
    spec = saturate(spec);
    spec = pow(spec, 20.0);
    color.rgb += spec;
    
    // env
    float2 envTexCoord;
    envTexCoord.x = -ref.x * .3 + .5;
    envTexCoord.y = -ref.y * .3 + .5;
    color.rgb += textureEnv.Sample(sampler0, envTexCoord) * .5;
    
    float3 fogColor = float3(0.0, 0.0, 0.0);
    float fog = depth.x;
    color.rgb = color.rgb * (1.0 - fog) + fogColor * fog;
    return color;
}