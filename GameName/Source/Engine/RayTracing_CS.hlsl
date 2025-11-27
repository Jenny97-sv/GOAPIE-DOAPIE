#include "RayTracingStructs.hlsli"


//float radius = 0.5f;

//float2 coord = float2(input.position.x / 1280, input.position.y / 720);
//    coord = coord * 2.f - 1.f;
    
//float aspect = 720.f / 1280.f;
    
//float3 rayOrigin = float3(0, 0, 2);
//float3 rayDiraction = float3(coord.x, coord.y * aspect, -1.f);
       
//float a = rayDiraction.x * rayDiraction.x + rayDiraction.y * rayDiraction.y + rayDiraction.z * rayDiraction.z;
    
//float b = 2.f * (rayOrigin.x * rayDiraction.x + rayOrigin.y * rayDiraction.y + rayOrigin.z * rayDiraction.z);
    
//float c = (rayOrigin.x * rayOrigin.x + rayOrigin.y * rayOrigin.y + rayOrigin.z * rayOrigin.z) - (radius * radius);
    
//float discriminant = b * b - 4.f * a * c;


float DotProduct(float3 aVec, float3 aOtherVec)
{
    float resualt = aVec.x * aOtherVec.x + aVec.y * aOtherVec.y + aVec.z * aOtherVec.z;
    
    return resualt;
}

float LengthSqr(float3 aVec)
{
    float resualt;
    resualt = aVec.x * aVec.x + aVec.y * aVec.y + aVec.z * aVec.z;
    
    resualt = resualt;
    
    return resualt;
}

bool IntersectSphere(Ray aRay, Sphere aSphere)
{
    float a = DotProduct(aRay.direction, aRay.direction);
    
    float b = 2.f * DotProduct(aRay.origin, aRay.direction);
    
    float c = DotProduct(aRay.origin, aRay.origin) - aSphere.radius * aSphere.radius;
    
    float discriminant = b * b - 4.f * a * c;
          
    return discriminant >= 0;
}

float4 TraceRay(Ray aRay)
{
    float4 resualtColor = float4(0, 0, 0, 0);
    
    for (int i = 0; i < numberOfObjectsInScene; i++)
    {
        Sphere tempSphere = ObjectBuffer[i];
        
        float t;
        if (IntersectSphere(aRay, tempSphere))
        {
            
            resualtColor.xyz = tempSphere.color;

        }
    }
    
    
    return resualtColor;
}

#define NTHREADS 26
[numthreads(NTHREADS, NTHREADS, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int x = DTid.x + DTid.y;
    //int y = DTid.y;
    int pixelIndex = x;
    
    Ray tempRay;
    
    tempRay.bounces = 1;
    tempRay.origin = CameraPosition.xyz;
    tempRay.direction = CameraForward + DTid.x * CameraRight + DTid.y * CameraUp;
    
    float4 tempResualt = TraceRay(tempRay);
    
    rw_ColorOutPut[pixelIndex] = tempResualt;
    
    GroupMemoryBarrierWithGroupSync();
}