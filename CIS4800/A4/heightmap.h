// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

class Heightmap
{
    public:
        static const float SCALE_X = 1;
        static const float SCALE_Y = 1;
        static const float SCALE_Z_INITIAL = 70;

        static Heightmap* loadPGM(const std::string& filename);
        
    
        int width, height, depth;
        Color ambient;
        Color specular;
        float scaleZ;
        
        int* data;
       
        float* vertices;
        float* colors;
        float* normals;
        Vector3* faceNormals;
        Vector3* sharedVertexNormals;
        
        
        bool randomize;
    
        Heightmap(int width, int height, int depth);
        ~Heightmap();

        void flip();
        void refreshGeometry();
        void draw();        
};
