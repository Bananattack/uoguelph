#pragma once

class Face;
class Vertex
{
    public:
        std::vector<Face*> adjacents;
        
        Vector3 position;
        Vector3 normal;
        Color color;
        
        Vector3 shadowPosition;
        
        Vertex(Vector3 position);        
        ~Vertex();
        
        
        void addAdjacent(Face* face);
        int getAdjacentCount();
        void refreshNormal();
        void refreshShadowPosition(Light& light, double planeY);
};
