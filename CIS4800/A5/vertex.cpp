#include "a5.h"

Vertex::Vertex(Vector3 position)
{
    this->position = position;
}

Vertex::~Vertex()
{
}

void Vertex::addAdjacent(Face* face)
{
    adjacents.push_back(face);
}

int Vertex::getAdjacentCount()
{
    return adjacents.size();
}

void Vertex::refreshNormal()
{
    normal.setData(0, 0, 0);
	
    if(adjacents.size())
    {
        for(size_t i = 0; i < adjacents.size(); i++)
        {
            normal.add(adjacents[i]->normal);
        }
        normal.scaledBy(1 / float(adjacents.size()));
    }
}


void Vertex::refreshShadowPosition(Light& light, double planeY)
{
	// [x, y, (-Lx * x - Ly * y - Lw) / Lz]
	float x = light.position.data[Vector4::X];
	float y = light.position.data[Vector4::Y];
	float z = light.position.data[Vector4::Z];
	float w = light.position.data[Vector4::W];
	shadowPosition.setData(
		position.data[Vector3::X], 
		position.data[Vector3::Y] - planeY,
		( x * position.data[Vector3::X]
		+ y * position.data[Vector3::Y]
		- w
		) / (near(z, 0) ? 0.0001 : z)
	);
}
