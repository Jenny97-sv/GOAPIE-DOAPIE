struct Sphere
{
    float4x4 world;
    float radius;
    float3 position;
    float t;
    float3 color;
    bool isLightSource;
    float3 unsued;
};

struct Ray
{
    float3 origin;
    float bounces;
    float3 direction;
    float unused;    
};

cbuffer CameraBuffer : register(b2)
{
    float4 CameraPosition;
    float4 CameraForward;
    float4 CameraRight;
    float4 CameraUp;
    float NearPlane;
    float FarPlane;
    float time;
    uint numberOfObjectsInScene;
}



//struct ExternalInformation
//{
//    float numberOfObjectsInScene;
//};

StructuredBuffer<Sphere> ObjectBuffer : register(t0);
RWStructuredBuffer<Sphere> rw_ObjectBuffer : register(u1);

//RWTexture2D<float4> ColorOutPut : register(t1);
RWStructuredBuffer<float4> rw_ColorOutPut : register(u2);


