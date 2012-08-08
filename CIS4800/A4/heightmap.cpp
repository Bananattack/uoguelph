// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#include <iostream>
#include <fstream>
#include "a4.h"

//#define FLAT_SHADE

Heightmap* Heightmap::loadPGM(const std::string& filename)
{
    std::ifstream f;
    
    f.open(filename.c_str(), std::ios_base::in);
     
    std::cout << "LOADING..." << std::endl;
    if(f.fail())
    {
        std::cerr << "* Error: failed to open " << filename << std::endl;
        return NULL;
    }
    
    int offset = 0;
    int width, height;
    Heightmap* result = NULL;
 
    const int BUFFER_SIZE = 64;
    char buffer[BUFFER_SIZE];
    
    bool comment = false;
    
    const int NUMBER_MAX_DIGITS = 32; 
    char numberToken[NUMBER_MAX_DIGITS];
    int numberDigits = 0;
    
    numberToken[0] = 0;
    // P2 header
    f.getline(buffer, BUFFER_SIZE);
    
    // Now read the rest of the file.
    while(!f.eof())
    {
        f.getline(buffer, BUFFER_SIZE - 2);
        {
            int l = strlen(buffer);
            
            buffer[l] = '\n';
            buffer[l + 1] = '\0';
        }
        
        for(size_t i = 0; i < strlen(buffer); i++)
        {
            char c = buffer[i];
            if(comment)
            {
                if(c == '\r' || c == '\n')
                {
                    comment = false;
                }
            }
            if(!comment)
            {
                switch(c)
                {
                    case '\t':
                    case '\r':
                    case '\n':
                    case ' ':
                        if(numberDigits)
                        {
                            switch(offset)
                            {
                                case 0:
                                    width = atoi(numberToken);
                                    break;
                                case 1:
                                    height = atoi(numberToken);
                                    break;
                                case 2:
                                    result = new Heightmap(width, height, atoi(numberToken));
                                    break;
                                default:
                                    result->data[offset - 3] = atoi(numberToken);
                                    break;
                            }
                            
                            offset++;
                            numberToken[0] = 0;
                            numberDigits = 0;
                        }
                        break;
                    case '#':
                        comment = true;
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        if(numberDigits < NUMBER_MAX_DIGITS - 1)
                        {
                            numberToken[numberDigits] = c;
                            numberToken[numberDigits + 1] = 0;
                            numberDigits++;
                        }
                        break;                    
                }
            }
        }   
    }
    
    if(comment)
    {
		if(numberDigits)
		{
			switch(offset)
			{
				case 0:
					width = atoi(numberToken);
					break;
				case 1:
					height = atoi(numberToken);
					break;
				case 2:
					result = new Heightmap(width, height, atoi(numberToken));
					break;
				default:
					result->data[offset - 3] = atoi(numberToken);
					break;
			}
			
			offset++;
			numberToken[0] = 0;
			numberDigits = 0;
		}
	}
    
    std::cout << "LOADED A " << width << "x" << height <<  " HEIGHTMAP WITH DEPTH " << result->depth << std::endl;
    
    result->flip();
    result->refreshGeometry();
    
    std::cout << "DONE..." << std::endl;
    return result;
}

Heightmap::Heightmap(int width, int height, int depth)
    : width(width), height(height), depth(depth)
{
    ambient = 0xffcccccc;
    specular = 0xffffffff;
    data = new int[width * height];
    vertices = normals = colors = NULL;
    faceNormals = sharedVertexNormals = NULL;
    randomize = true;
    scaleZ = SCALE_Z_INITIAL;
}
        
Heightmap::~Heightmap()        
{
    delete[] data;
}

static float randomFloat()
{
	return rand() / (float(RAND_MAX) + 1.0);
}


static float randomFloatRange(float low, float high)
{
	float value = randomFloat() * (high - low) + low;
	return value;
}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define ADD_POINT(p, i, x, y, z) \
    { \
        p[i++] = x; \
        p[i++] = y; \
        p[i++] = z; \
    }
    
#define ADD_VEC3(p, i, vec) \
	{ \
		Vector3& v = vec; \
		ADD_POINT(p, i, v.data[Vector3::X], v.data[Vector3::Y], v.data[Vector3::Z]) \
	}

#define ADD_COLOR(c, i, value) \
    { \
    	Color& v = value; \
    	if(!randomize) \
    	{ \
    		c[i++] = v.data[Color::RedChannel]; \
    		c[i++] = v.data[Color::GreenChannel]; \
    		c[i++] = v.data[Color::BlueChannel]; \
    	} \
    	else \
    	{ \
    		c[i++] = MIN(MAX(v.data[Color::RedChannel] + randomFloatRange(-0.2, 0.2), 0), 1); \
    		c[i++] = MIN(MAX(v.data[Color::GreenChannel] + randomFloatRange(-0.2, 0.2), 0), 1); \
    		c[i++] = MIN(MAX(v.data[Color::BlueChannel] + randomFloatRange(-0.2, 0.2), 0), 1); \
    	} \
        c[i++] = 0; \
    }                                                                
    
void Heightmap::flip()
{
	int t;
	for(int j = 0; j < height / 2; j++)
	{
		for(int i = 0; i < width; i++)
		{
			t = data[j * width + i];
			data[j * width + i] = data[(height - 1- j) * width + i];
			data[(height - 1 - j) * width + i] = t;
		}
	}
}    

void Heightmap::refreshGeometry()
{
    if(!vertices)
    {
    	vertices = new float[(width - 1) * (height - 1) * 6 * 3];
    }
    if(!colors)
    {
        colors = new float[(width - 1) * (height - 1) * 6 * 4];
    }
    if(!normals)
    {
        normals = new float[(width - 1) * (height - 1) * 6 * 3];
    }
    if(!faceNormals)
    {
    	faceNormals = new Vector3[(width - 1) * (height - 1) * 2];
    }
    if(!sharedVertexNormals)
    {
    	sharedVertexNormals = new Vector3[width * height];
    }
    //std::cout << "REFRESHING GEOMETRY ..." << std::endl;
    
    int vertexIndex = 0;
    int colorIndex = 0;
    
    int faceIndex = 0;
    Vector3 tl, tr, bl, br;
    
    Color color = ambient;
    // Build entire height map.
    //std::cout << "BUILDING VERTICES, CALCULATING FACE NORMALS, AND CALCULATING COLORS..." << std::endl;
    for(int j = 0; j < height - 1; j++)
    {
        for(int i = 0; i < width - 1; i++)
        {   
            tl.setData(i * SCALE_X, j * SCALE_Y, data[j * width + i] / double(depth) * scaleZ);
            tr.setData((i + 1) * SCALE_X, j * SCALE_Y, data[j * width + i + 1] / double(depth) * scaleZ);
            bl.setData(i * SCALE_X, (j + 1) * SCALE_Y, data[(j + 1) * width + i] / double(depth) * scaleZ);
            br.setData((i + 1) * SCALE_X, (j + 1) * SCALE_Y, data[(j + 1) * width + (i + 1)] / double(depth) * scaleZ);

            ADD_VEC3(vertices, vertexIndex, tl)
            ADD_COLOR(colors, colorIndex, color)
            ADD_VEC3(vertices, vertexIndex, tr)
            ADD_COLOR(colors, colorIndex, color)
            ADD_VEC3(vertices, vertexIndex, br)
            ADD_COLOR(colors, colorIndex, color)
            
            faceNormals[faceIndex++] = bl.subtractedBy(tl).cross(tr.subtractedBy(tl)).normalize();
            //faceNormals[faceIndex - 1].print();

            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
 
            
            ADD_VEC3(vertices, vertexIndex, tl)
            ADD_COLOR(colors, colorIndex, color)
            ADD_VEC3(vertices, vertexIndex, br)
            ADD_COLOR(colors, colorIndex, color)
            ADD_VEC3(vertices, vertexIndex, bl)
            ADD_COLOR(colors, colorIndex, color)
            
            faceNormals[faceIndex++] = tr.subtractedBy(br).cross(bl.subtractedBy(br)).normalize();
            //faceNormals[faceIndex - 1].print();
            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
            //ADD_VEC3(normals, normalIndex, faceNormals[faceIndex - 1])
        }
    }
    
    
    //std::cout << "CALCULATING CORNER NORMALS..." << std::endl;
    // Corner normals.
    // Top-left
    //std::cout << "    TOP LEFT..." << std::endl;
    sharedVertexNormals[0] = 
    	faceNormals[0]
			.addedWith(faceNormals[1])
			.scale(1.0 / 2.0);		
    // Top-right
    //std::cout << "    TOP RIGHT..." << std::endl;
    sharedVertexNormals[width - 1] =
    	faceNormals[2 * width - 2];
    // Bottom-left
    //std::cout << "    BOTTOM LEFT..." << std::endl;
    sharedVertexNormals[(height - 1) * width] = faceNormals[(height - 1) * 2 * (width - 1) + 1];
    // Bottom-right
    //std::cout << "    BOTTOM RIGHT..." << std::endl;
    sharedVertexNormals[(height - 1) * width + width - 1] =
    	faceNormals[(height - 2) * 2 * (width - 1) - 2]
    		.addedWith(faceNormals[(height - 2) * 2 * (width - 1) - 1])
    		.scale(1.0 / 2.0);
    
    //std::cout << "CALCULATING LEFT AND RIGHT EDGE NORMALS..." << std::endl;
    // Left and right edge normals
    for(int y = 1; y < height - 2; y++)
    {
    	// Left edge
    	sharedVertexNormals[y * width] =
    		faceNormals[(y - 1) * 2 * (width - 1) - 1]
    			.addedWith(faceNormals[y * 2 * (width - 1)])
    			.add(faceNormals[y * 2 * (width - 1) + 1])
    			.scale(1.0 / 3.0);
    	// Right edge
    	sharedVertexNormals[y * width + width - 1] = Vector3();
    }
    
    //std::cout << "CALCULATING TOP AND BOTTOM EDGE NORMALS..." << std::endl;
    // Top and bottom edge normals
    for(int x = 1; x < width - 2; x++)
    {
    	// Top edge
    	sharedVertexNormals[x] =
    		faceNormals[2 * x - 2]
    			.addedWith(faceNormals[2 * x])
    			.addedWith(faceNormals[2 * x + 1])
    			.scaledBy(1.0 / 3.0);
    	// bottom edge
    	sharedVertexNormals[(height - 2) * width + x] =
    		faceNormals[(height - 2) * 2 * (width - 1) + 2 * x - 2]
    			.addedWith(faceNormals[(height - 2) * 2 * (width - 1) + 2 * x - 1])
    			.add(faceNormals[(height - 2) * 2 * (width - 1) + 2 * x + 1])
    			.scale(1.0 / 3.0);
    }    
    
    //std::cout << "CALCULATING INNER NORMALS..." << std::endl;
    // Smooth all inner vertex normals.
    for(int y = 1; y < height - 2; y++)
    {
		for(int x = 1; x < width - 2; x++)
		{
			sharedVertexNormals[y * width + x] =
    			faceNormals[(y - 1) * 2 * (width - 1) + 2 * x - 2]
    				.addedWith(faceNormals[(y - 1) * 2 * (width - 1) + 2 * x - 1])
    				.add(faceNormals[(y - 1) * 2 * (width - 1) + 2 * x + 1])
    				.add(faceNormals[y * 2 * (width - 1) + 2 * x - 2])
    				.add(faceNormals[y * 2 * (width - 1) + 2 * x])
    				.add(faceNormals[y * 2 * (width - 1) + 2 * x + 1])
    				.scale(1.0 / 6.0);
		}
    }
    
    //std::cout << "SENDING NORMALS TO GL..." << std::endl;
    faceIndex = 0;
    int normalIndex = 0;
    // Setup normals for GL now.
    for(int j = 0; j < height - 1; j++)
    {
        for(int i = 0; i < width - 1; i++)
        {  
#ifdef FLAT_SHADE
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	faceIndex++;
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	ADD_VEC3(normals, normalIndex, faceNormals[faceIndex])
        	faceIndex++;
#else        	
        	// tl, tr, br triangle
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[j * width + i])
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[j * width + (i + 1)])
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[(j + 1) * width + (i + 1)])
            
            // tl, br, bl triangle
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[j * width + i])
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[(j + 1) * width + (i + 1)])
            ADD_VEC3(normals, normalIndex, sharedVertexNormals[(j + 1) * width + i])    
#endif
        }
    }
}

void Heightmap::draw()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient.data);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular.data);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glPushMatrix();
    glTranslatef(-width / 2.0 * SCALE_X, -height / 2.0 * SCALE_Y, 0);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glNormalPointer(GL_FLOAT, 0, normals);
    
    glDrawArrays(GL_TRIANGLES, 0, (width - 1) * (height - 1) * 6);
    glPopMatrix();
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
