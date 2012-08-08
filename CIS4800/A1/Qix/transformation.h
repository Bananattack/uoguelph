// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
	class Transformation
    {
		public:
			Vector3 translation;
			Vector3 scale;
			Vector3 rotation;
	        
			Transformation()
			{
				identity();
			}

			Transformation(Vector3 translation, Vector3 scale, Vector3 rotation)
				: translation(translation), scale(scale), rotation(rotation)
			{
			}

			~Transformation()
			{
			}

			void identity()
			{
				translation = Vector3(0, 0, 0);
				scale = Vector3(1, 1, 1);
				rotation = Vector3(0, 0, 0);
			}

			void applyToRender()
			{
				glTranslatef(translation.data[0], translation.data[1], translation.data[2]);
				glScalef(scale.data[0], scale.data[1], scale.data[2]);
				glRotatef(rotation.data[0], 1, 0, 0);
				glRotatef(rotation.data[1], 0, 1, 0);
				glRotatef(rotation.data[2], 0, 0, 1);
			}
    };
}