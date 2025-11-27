struct VertexInput
{
    float4 position : POSITION;
    float4 color : COLOR;    
};


struct PixelInput
{
    float4 position : SV_Position;
    float4 color : COLOR;    
};

struct PixelOutput
{
    float4 color : SV_Target;
};