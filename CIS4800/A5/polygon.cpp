#include <stddef.h>
#include "ply.h"
#include "a5.h"

Polygon::Polygon()
{
    fillColor = 0xffff0000;
    shadowColor = 0xff000000;
    specularOn = 0xffffffff;
	specularOff = 0xff000000;
}

Polygon::~Polygon()
{
    for(size_t i = 0; i < vertices.size(); i++)
    {
        delete vertices[i];
    }
    for(size_t i = 0; i < faces.size(); i++)
    {
        delete faces[i];
    }
}

// Taken from http://developer.nvidia.com/attach/6641 Appendix B.
void shadowMatrix(float m[4][4], float plane[4], float light[4])
{
	light[0] *= -1;
	light[1] *= -1;
	light[2] *= -1;
	GLfloat dot = plane[0] * light[0] + plane[1] * light[1] + plane[2] * light[2] + plane[3] * light[3];
	m[0][0] = dot - light[0] * plane[0];
	m[1][0] = -light[0] * plane[1];
	m[2][0] = -light[0] * plane[2];
	m[3][0] = -light[0] * plane[3];
	m[0][1] = -light[1] * plane[0];
	m[1][1] = dot - light[1] * plane[1];
	m[2][1] = -light[1] * plane[2];
	m[3][1] = -light[1] * plane[3];
	m[0][2] = -light[2] * plane[0];
	m[1][2] = -light[2] * plane[1];
	m[2][2] = dot - light[2] * plane[2];
	m[3][2] = -light[2] * plane[3];
	m[0][3] = -light[3] * plane[0];
	m[1][3] = -light[3] * plane[1];
	m[2][3] = -light[3] * plane[2];
	m[3][3] = dot - light[3]*plane[3];
}


void Polygon::draw(Light& lightSource)
{
	float PlaneVertex[4][3] = {
		{ -planeSize, planeY, -planeSize },
		{ planeSize, planeY, -planeSize },
		{ planeSize, planeY, planeSize },
		{ -planeSize, planeY, planeSize },
	};

	float PlaneNormal[] = {
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};

	float PlaneColor[] = {
		0.5, 0.5, 0.5, 1,
		0.5, 0.5, 0.5, 1,
		0.5, 0.5, 0.5, 1,
		0.5, 0.5, 0.5, 1,
	};

	GLfloat PlaneVector[4] = {
		0, 1, 0, -planeY - 0.03
	};


	// Plane underneath imade
	glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &PlaneVertex[0][0]);
	glNormalPointer(GL_FLOAT, 0, PlaneNormal);
	glColorPointer(4, GL_FLOAT, 0, PlaneColor);

    glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	// Fake shadows.
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularOff.data);
	glPushMatrix();

	GLfloat matrix[4][4];
	Light stabstabstab = lightSource;
	shadowMatrix(matrix, PlaneVector, stabstabstab.position.data);
	glMultMatrixf(&matrix[0][0]);

	/*printf("%lf %lf %lf %lf\n", PlaneVector[0], PlaneVector[1], PlaneVector[2], PlaneVector[3]);
	printf("BEGIN\n");
	printf("%lf, %lf, %lf, %lf\n", matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3]); 
	printf("%lf, %lf, %lf, %lf\n", matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3]);
	printf("%lf, %lf, %lf, %lf\n", matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3]);
	printf("%lf, %lf, %lf, %lf\n", matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
	printf("END\n\n\n");*/

    glVertexPointer(3, GL_FLOAT, 0, &vertexArray[0]);
    shadowColor.applyToRender();
    // Draw every face of the polygon.
    for(int i = 0; i < faces.size(); i++)
    {
        faces[i]->draw();
    }
	glPopMatrix();
    

	// Polygon pieces.
	glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fillColor.data);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularOn.data);
    
    glVertexPointer(3, GL_FLOAT, 0, &vertexArray[0]);
    glNormalPointer(GL_FLOAT, 0, &normalArray[0]);
    glColorPointer(4, GL_FLOAT, 0, &colorArray[0]);
    
    // Draw every face of the polygon.
    for(size_t i = 0; i < faces.size(); i++)
    {
        faces[i]->draw();
    }

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Polygon::refreshGeometry()
{
    // Recalculate face normals.
    for(size_t i = 0; i < faces.size(); i++)
    {
        faces[i]->refreshNormal(this);
    }
    

    // Recalculate vertex normals, which require face normals.
    for(size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i]->refreshNormal();
        
        vertices[i]->position.getData(&vertexArray[i * 3]);
        vertices[i]->normal.getData(&normalArray[i * 3]);
    }
    // refreshShadow();
}

void Polygon::refreshShadow(Light& lightSource)
{
    // Recalculate shadow positions.
    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i]->refreshShadowPosition(lightSource, planeY);

		vertices[i]->shadowPosition.getData(&shadowVertexArray[i * 3]);
    }
}

void Polygon::refreshColors()
{
    // Recalculate vertex normals, which require face normals.
    for(size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i]->color = fillColor;
        
        vertices[i]->color.getData(&colorArray[i * 4]);
    }
}

void Polygon::addVertex(Vertex* vertex)
{
    vertices.push_back(vertex);
    for(size_t i = 0; i < 3; i++)
    {
        vertexArray.push_back(0);
        normalArray.push_back(0);
        
        shadowVertexArray.push_back(0);
    }
	shadowNormalArray.push_back(0); shadowNormalArray.push_back(-1); shadowNormalArray.push_back(0);
    for(size_t i = 0; i < 4; i++)
    {
        colorArray.push_back(0);
    }
}

void Polygon::addFace(Face* face)
{
    faces.push_back(face);
}


// Code below is mostly adapted from plytest.c
// Ugly ugly ugly. ugly. ugly.
// I have converted it to C++-ish style, and added my loading at the end of this.
// Did I mention this code is an ugly abomination to mankind?
struct PlyVertex
{
    float x, y, z;			// The usual 3-space position of a vertex
};

struct PlyFace
{
    unsigned char intensity; // This user attaches intensity to faces
    unsigned char nverts;    // Number of vertex indices in list
    int *verts;              // Vertex index list
};

// List of property information for a vertex
PlyProperty VERTEX_PROPERTIES[] = {
    {"x", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex, x), 0, 0, 0, 0},
    {"y", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex, y), 0, 0, 0, 0},
    {"z", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVertex, z), 0, 0, 0, 0},
};

// List of property information for a vertex
PlyProperty FACE_PROPERTIES[] = {
    { "intensity", PLY_UCHAR, PLY_UCHAR, offsetof(PlyFace, intensity), 0, 0, 0, 0 },
    { "vertex_indices", PLY_INT, PLY_INT, offsetof(PlyFace, verts), 1, PLY_UCHAR, PLY_UCHAR, offsetof(PlyFace, nverts) },
};


Polygon* Polygon::loadPolygon(const std::string& filename)
{   
    int elementCount;
    char** elementNames;
    int fileType;
    float version;
    // Open a PLY file for reading
    char* name = strdup(filename.c_str());
    PlyFile* ply = ply_open_for_reading(name, &elementCount, &elementNames, &fileType, &version);
    free(name);
    if(!ply)
    {
        printf("FAILED\n");
        return NULL;
    }
    
    // Print what we found out about the file
    //printf ("version %f\n", version);
    //printf ("type %d\n", fileType);

    std::vector<PlyVertex*> plyVertices;
    std::vector<PlyFace*> plyFaces;
    
    // Go through each kind of element that we learned is in the file and read them
    for (int i = 0; i < elementCount; i++)
    {
        // Get the description of the first element
        char* elementName = elementNames[i];
        
        int entryCount, propertyCount;
        PlyProperty** properties = ply_get_element_description(ply, elementName, &entryCount, &propertyCount);

        // Print the name of the element, for debugging
        //printf("element %s %d\n", elementName, entryCount);

        // If we're on vertex elements, read them in
        if (!strcmp("vertex", elementName))
        {   
            // Set up for getting vertex elements
            ply_get_property(ply, elementName, &VERTEX_PROPERTIES[0]);
            ply_get_property(ply, elementName, &VERTEX_PROPERTIES[1]);
            ply_get_property(ply, elementName, &VERTEX_PROPERTIES[2]);

            plyVertices.reserve(entryCount);
            // Grab all the vertex elements
            for (int j = 0; j < entryCount; j++)
            {
                PlyVertex* v = new PlyVertex();
             
                // Grab an element from the file
                ply_get_element(ply, v);
                
                plyVertices.push_back(v);

                // Print out vertex x,y,z for debugging
                //printf("vertex: %d %lf %lf %lf\n", plyVertices.size(), v->x, v->y, v->z);
            }
        }

        // If we're on face elements, read them in
        if (!strcmp("face", elementName))
        {   
            // Set up for getting face elements
            ply_get_property(ply, elementName, &FACE_PROPERTIES[0]);
            ply_get_property(ply, elementName, &FACE_PROPERTIES[1]);

            plyFaces.reserve(entryCount);
            // Grab all the face elements
            for (int j = 0; j < entryCount; j++)
            {
                PlyFace* f = new PlyFace();
                
                // Grab an element from the file
                ply_get_element(ply, f);
                
                plyFaces.push_back(f);

                // Print out face info, for debugging
                /*printf("face: %d, list = ", f->intensity);
                for (int k = 0; k < f->nverts; k++)
                {
                    printf("%d ", f->verts[k]);
                }
                printf("\n");*/
            }
        }
        
        // Print out the properties we got, for debugging
        /*for (int j = 0; j < propertyCount; j++)
        {
            printf ("property %s\n", properties[j]->name);
        }*/
    }

    Polygon* polygon = new Polygon();    
    polygon->vertices.reserve(plyVertices.size());
    polygon->vertexArray.reserve(plyVertices.size() * 3);
    polygon->normalArray.reserve(plyVertices.size() * 3);
    polygon->colorArray.reserve(plyVertices.size() * 4);
    polygon->shadowVertexArray.reserve(plyVertices.size() * 3);
    polygon->shadowNormalArray.reserve(plyVertices.size() * 3);
    polygon->faces.reserve(plyFaces.size());
    
    Vector3 vec3;
    polygon->minX = plyVertices[0]->x;
    polygon->maxX = plyVertices[0]->x;
    polygon->minY = plyVertices[0]->y;
    polygon->maxY = plyVertices[0]->y;
    polygon->minZ = plyVertices[0]->z;
    polygon->maxZ = plyVertices[0]->z;
    for(size_t i = 0; i < plyVertices.size(); i++)
    {
        vec3.setData(plyVertices[i]->x, plyVertices[i]->y, plyVertices[i]->z);
        Vertex* v = new Vertex(vec3);
        polygon->addVertex(v); 
        
        polygon->minX = MIN(plyVertices[i]->x, polygon->minX);
        polygon->maxX = MAX(plyVertices[i]->x, polygon->maxX);
        polygon->minY = MIN(plyVertices[i]->y, polygon->minY);
        polygon->maxY = MAX(plyVertices[i]->y, polygon->maxY);
        polygon->minZ = MIN(plyVertices[i]->z, polygon->minZ);
        polygon->maxZ = MAX(plyVertices[i]->z, polygon->maxZ);
        
        delete plyVertices[i];
    }
    
    polygon->width = polygon->maxX - polygon->minX;
    polygon->height = polygon->maxY - polygon->minY;
    polygon->depth = polygon->maxZ - polygon->minZ;
    printf("(%lf, %lf, %lf) -> (%lf, %lf, %lf) = (%lf, %lf, %lf)\n", polygon->minX, polygon->minY, polygon->minZ, polygon->maxX, polygon->maxY, polygon->maxZ, polygon->width, polygon->height, polygon->depth);
    
    for(size_t i = 0; i < plyFaces.size(); i++)
    {   
        Face* face = new Face();
        for(size_t j = 0; j < plyFaces[i]->nverts; j++)
        {
            int idx = plyFaces[i]->verts[j];
            face->addVertex(idx);
            polygon->vertices[idx]->addAdjacent(face);
        }
        polygon->addFace(face);
        
        delete plyFaces[i];
    }
    
    polygon->planeSize = -MAX(polygon->width, polygon->depth) * 1.5;
    polygon->planeY = polygon->minY - 0.06;

    // Close the PLY file.
    ply_close (ply);
    
    return polygon;
}
