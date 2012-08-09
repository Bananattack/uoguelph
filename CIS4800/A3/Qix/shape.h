// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

namespace Qix
{
    class Shape
    {
        public:
			virtual ~Shape()
			{
			}

			virtual void render() = 0;
            virtual Transformation& transformation() = 0;
			virtual Color& ambient() = 0;
			virtual Color& specular() = 0;
    };
}