#pragma once

class Vertex;
class Polygon;
class Face
{
    public:
        std::vector<GLuint> vertices;
        
        Vector3 normal;
        Vector3 shadowNormal;
        
        Face();
        ~Face();
        
        void draw();
        void refreshNormal(Polygon* polygon);
        
        void addVertex(int index);
        int getVertexCount();
};
