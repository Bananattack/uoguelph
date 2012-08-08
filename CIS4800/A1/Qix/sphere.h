// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Sphere : public Renderable
    {
        private:
            static const int Subdivisions = 30;

			Transformation _transformation;
			Color _ambient, _specular;
        public:
            Sphere(Transformation transformation, Color ambient = 0xff333333, Color specular = 0xffffffff)
				: _transformation(transformation), _ambient(ambient), _specular(specular)
            {
            }
            
            ~Sphere()
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
				glutSolidSphere(1, Subdivisions, Subdivisions);
                glPopMatrix();
            }
    };
}