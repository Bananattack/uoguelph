// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    struct Color
    {
        enum ColorChannel
        {
            RedChannel = 0,
            GreenChannel = 1,
            BlueChannel = 2,
            AlphaChannel = 3,
        };
        
        GLfloat data[4];
        
		Color()
		{
			memset(data, 0, sizeof(data));
		}

        Color(unsigned int value)
        {
            data[RedChannel] = ((value >> 16) & 255) / 255.0; 
            data[GreenChannel] = ((value >> 8) & 255) / 255.0;
            data[BlueChannel] = (value & 255) / 255.0;
            data[AlphaChannel] = ((value >> 24) & 255) / 255.0;
        }
    };
}