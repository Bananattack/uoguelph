// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
	class Texture
	{
		public:
            unsigned int width, height;
            unsigned char* pixels;            
            GLuint handle;
                        
            Texture(unsigned int width, unsigned int height, unsigned char* pixels)
                : pixels(pixels)
            {
                glGenTextures(1, &handle);
                bind();
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            }
            
            ~Texture()
            {
                glDeleteTextures(1, &handle);
            }
            
            void bind()
            {
                glBindTexture(GL_TEXTURE_2D, handle);
            }
            
            static Texture* loadImage(const char* filename)
            {
                FILE* f = fopen(filename, "rb");
                if(!f)
                {
                    fprintf(stderr, "Could not find '%s'", filename);
                    exit(-1);
                }
                unsigned char widthPower;
                unsigned char heightPower;
                fread(&widthPower, 1, 1, f);
                fread(&heightPower, 1, 1, f);     
                
                unsigned int width = 1 << widthPower;
                unsigned int height = 1 << heightPower;                
                unsigned char* data = new unsigned char[width * height * 3];                
                fread(data, 1, width * height * 3, f);
                
                return new Texture(width, height, data);
            }            
	};
}