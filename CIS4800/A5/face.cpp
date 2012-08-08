#include "a5.h"

Face::Face()
{

}

Face::~Face()
{
    
}

void Face::draw()
{
    glDrawElements(GL_POLYGON, vertices.size(), GL_UNSIGNED_INT, &vertices[0]);
}

// Based off Newell's Method pseudocode described in
// http://www.opengl.org/wiki/Calculating_a_Surface_Normal
void Face::refreshNormal(Polygon* polygon)
{
    std::vector<Vertex*>& vert = polygon->vertices;
    
    normal.setData(0, 0, 0);
    Vector3 current, next;
    for(size_t i = 0; i < vertices.size(); i++)
    {
        current = vert[vertices[i]]->position;
        next = vert[vertices[(i + 1) % vertices.size()]]->position;
        
        normal.add(current.cross(next));
    }

    normal.normalize();
}

void Face::addVertex(int vertex)
{
    vertices.push_back(vertex);
}

int Face::getVertexCount()
{
    return vertices.size();
}