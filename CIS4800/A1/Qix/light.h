// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Light
    {
        public:
			Vector4 position;

			Color ambient;
			Color diffuse;
			Color specular;

			Light()
				: ambient(0xff000000), diffuse(0xffffffff), specular(0xffffffff)
			{
			}

			Light(Vector4 position)
				: position(position), ambient(0xff000000), diffuse(0xffffffff), specular(0xffffffff)
			{
			}

			Light(Vector4 position, Color ambient, Color diffuse, Color specular)
				: position(position), ambient(ambient), diffuse(diffuse), specular(specular)
			{
			}

			void load(GLenum lightIndex)
			{
				glLightfv(lightIndex, GL_AMBIENT, ambient.data);
				glLightfv(lightIndex, GL_DIFFUSE, diffuse.data);
				glLightfv(lightIndex, GL_SPECULAR, specular.data);
				glLightfv(lightIndex, GL_POSITION, position.data);
			}
	};
}