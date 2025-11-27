#include "Common.hlsli"
#include "RayTracingStructs.hlsli"

PixelOutput main(PixelInput input)
{
    PixelOutput output;
    
    //input.position.x;
    
    //float radius = 0.5f;

    //float2 coord = float2(input.position.x / 1280, input.position.y / 720);
    //coord = coord * 2.f - 1.f;
    
    //float aspect = 720.f / 1280.f;
    
    //float3 rayorigin = float3(0, 0, 2);
    //float3 raydiraction = float3(coord.x, coord.y * aspect, -1.f);
       
    //float a = raydiraction.x * raydiraction.x + raydiraction.y * raydiraction.y + raydiraction.z * raydiraction.z;
    
    //float b = 2.f * (rayorigin.x * raydiraction.x + rayorigin.y * raydiraction.y + rayorigin.z * raydiraction.z);
    
    //float c = (rayorigin.x * rayorigin.x + rayorigin.y * rayorigin.y + rayorigin.z * rayorigin.z) - (radius * radius);
    
    //float discriminant = b * b - 4.f * a * c;
            
    //float radius1 = 0.95f;

    //float2 coord1 = float2(input.position.x / 1280, input.position.y / 720);
    //coord1 = coord1 * 2.f - 1.f;
        
    //float3 rayorigi1 = float3(2, 0, 6);
    //float3 raydiractio1 = float3(coord1.x, coord1.y * aspect, -1.f);
               
    //float a1 = raydiractio1.x * raydiractio1.x + raydiractio1.y * raydiractio1.y + raydiractio1.z * raydiractio1.z;
    
    //float b1 = 2.f * (rayorigi1.x * raydiractio1.x + rayorigi1.y * raydiractio1.y + rayorigi1.z * raydiractio1.z);
    
    //float c1 = (rayorigi1.x * rayorigi1.x + rayorigi1.y * rayorigi1.y + rayorigi1.z * rayorigi1.z) - (radius1 * radius1);
    
    //float discriminant1 = b1 * b1 - 4.f * a1 * c1;
    
    //if (discriminant1 >= 0)
    //{
    //    output.color = float4(1, 0, 0, 1);
    //}
    //else if (discriminant >= 0)
    //{
    //    output.color = float4(0, 0, 1, 1);
    //}
    //else
    //{
    //    output.color = float4(0, 0, 0, 1);
    //}
    
    int pixelIndex = int(input.position.x+ input.position.y);
    input.color = rw_ColorOutPut[pixelIndex];

    output.color = input.color;
        
    return output;
}