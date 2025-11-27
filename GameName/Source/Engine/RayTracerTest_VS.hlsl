#include "Common.hlsli"

PixelInput main(VertexInput input)
{
    PixelInput tempInput;
    
    tempInput.position = input.position;
    
    //int2 pixelIndex = int2(input.position.x, input.position.y);
    //input.color = rw_ColorOutPut[pixelIndex];
    
    tempInput.color = input.color;
    
    return tempInput;
}