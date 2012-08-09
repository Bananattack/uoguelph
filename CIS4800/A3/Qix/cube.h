// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Cube : public Shape
    {
		private:
            static const int PointCount = 24;
            static GLfloat vertices[PointCount * 3];
            static GLfloat normals[PointCount * 3];
            
			Transformation _transformation;
			Color _ambient, _specular;
        public:
            Texture* texture;
        
			Cube()
			{
                texture = 0;
				_transformation.identity();
                _ambient = 0xff333333;
                _specular = 0xffffffff;
			}

            Cube(Transformation transformation, Texture* texture = 0, Color ambient = 0xff333333, Color specular = 0xffffffff)
				: _transformation(transformation), _ambient(ambient), _specular(specular)
            {
                this->texture = texture;
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
                GLfloat x, y, z;
                x = _transformation.scale.data[0];
                y = _transformation.scale.data[1];
                z = _transformation.scale.data[2];
                GLfloat texCoords[] = {
                    // Front face.
                    0, 0,
                    0, x,
                    y, x,
                    y, 0,
                    // Right face.
                    0, 0,
                    0, y,
                    z, y,
                    z, 0,
                    // Top face.
                    0, 0,
                    0, z,
                    x, z,
                    x, 0,
                    // Left face.
                    0, 0,
                    0, z,
                    y, z,
                    y, 0,
                    // Bottom face.
                    0, 0,
                    0, x,
                    z, x,
                    z, 0,
                    // Back face.
                    0, 0,
                    0, x,
                    y, x,
                    y, 0,                                        
                };
            
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, _ambient.data);
				glMaterialfv(GL_FRONT, GL_SPECULAR, _specular.data);

                glPushMatrix();
                _transformation.applyToRender();
            
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                if(texture)
                {
                    glEnable(GL_TEXTURE_2D);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);                    
                    texture->bind();
                }
                
                glNormalPointer(GL_FLOAT, 0, normals);
                glVertexPointer(3, GL_FLOAT, 0, vertices);
                if(texture)
                {
                    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
                }
                glDrawArrays(GL_QUADS, 0, PointCount);                   
                 
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);              
                if(texture)
                {
                    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                    glDisable(GL_TEXTURE_2D);
                }
                
                glPopMatrix();
            }
    };
}