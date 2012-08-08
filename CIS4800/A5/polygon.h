#pragma once

#include <string>

class Polygon
{
    public:
        std::vector<Vertex*> vertices;
        std::vector<Face*> faces;
        Color fillColor;
        Color shadowColor;
        Color specularOn;
		Color specularOff;
        
        std::vector<GLfloat> vertexArray;
        std::vector<GLfloat> normalArray;
        std::vector<GLfloat> colorArray;

        std::vector<GLfloat> shadowVertexArray;
        std::vector<GLfloat> shadowNormalArray;
        
        double minX, maxX, minY, maxY, minZ, maxZ;
        double width, height, depth;
        double planeSize, planeY;
        
        Polygon();
        ~Polygon();
        
        void draw(Light& lightSource);
        void refreshGeometry();
        void refreshShadow(Light& lightSource);
        void refreshColors();
        
        void addVertex(Vertex* vertex);
        void addFace(Face* face);
        
        static Polygon* loadPolygon(const std::string& filename);
};
