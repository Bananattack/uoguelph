// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Cube : public Renderable
    {
		private:
			Transformation _transformation;
			Color _ambient, _specular;
        public:
			Cube()
			{
				_transformation.identity();
			}

            Cube(Transformation transformation, Color ambient = 0xff333333, Color specular = 0xffffffff)
				: _transformation(transformation), _ambient(ambient), _specular(specular)
            {
            }
            
            ~Cube()
            {
            }
            
			virtual Transformation& transformation()
			{
				return _transformation;
			}

			virtual Color& ambient()
			{
				return _ambient;
			}

			virtual Color& specular()
			{
				return _specular;
			}

            virtual void render()
            {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, _ambient.data);
				glMaterialfv(GL_FRONT, GL_SPECULAR, _specular.data);

                glPushMatrix();
                _transformation.applyToRender();
                glutSolidCube(1);
                glPopMatrix();
            }
    };
}