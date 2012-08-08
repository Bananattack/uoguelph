// CIS*4800 Computer Graphics
// Andrew Crowell
// acrowell@uoguelph.ca
// 0545826
#pragma once

struct Vector3
{
    enum Component
    {
        X = 0,
        Y = 1,
        Z = 2,
    };

    
    typedef GLfloat Scalar;
    typedef Vector3 VectorType;
    Scalar data[3];
    
    Vector3()
    {
        data[X] = data[Y] = data[Z] = 0;
    }

    Vector3(GLfloat x, GLfloat y, GLfloat z)
    {
        data[X] = x;
        data[Y] = y;
        data[Z] = z;
    }

    
	Scalar lengthSquared()
	{
		return data[X] * data[X] + data[Y] * data[Y] + data[Z] * data[Z];
	}
	  
	Scalar length()
	{
		return sqrt(lengthSquared());
	}
	
	Scalar dot(VectorType& v)
	{
		return data[X] * v.data[X] + data[Y] * v.data[Y] + data[Z] * v.data[Z];
	}
	
	VectorType scaledBy(Scalar a)
	{
		return VectorType(data[X] * a, data[Y] * a, data[Z] * a);
	}
	
	VectorType addedWith(VectorType v)
	{
		return VectorType(data[X] + v.data[X], data[Y] + v.data[Y], data[Z] + v.data[Z]);
	}
	
	VectorType subtractedBy(VectorType v)
	{
		return VectorType(data[X] - v.data[X], data[Y] - v.data[Y], data[Z] - v.data[Z]);
	}
    
	VectorType crossedBy(VectorType v)
	{
		return VectorType(data[Y] * v.data[Z] - data[Z] * v.data[Y], data[Z] * v.data[X] - data[X] * v.data[Z], data[X] * v.data[Y] - data[Y] * v.data[X]);
	}
	
	VectorType normalized()
	{
		return scaledBy(1 / length());
	}
	
	VectorType clone()
	{
		return VectorType(data[X], data[Y], data[Z]);
	}
	
	VectorType& scale(Scalar a)
	{
		data[X] *= a;
		data[Y] *= a;
		data[Z] *= a;
		return *this;
	}
	
	VectorType& add(const VectorType& v)
	{
		data[X] += v.data[X];
		data[Y] += v.data[Y];
		data[Z] += v.data[Z];
		return *this;
	}
	
	VectorType& subtract(const VectorType& v)
	{
		data[X] -= v.data[X];
		data[Y] -= v.data[Y];
		data[Z] -= v.data[Z];
		return *this;
	}
    
	VectorType& cross(const VectorType& v)
	{
		float x, y, z;
		x = data[Y] * v.data[Z] - data[Z] * v.data[Y];
		y = data[Z] * v.data[X] - data[X] * v.data[Z];
		z = data[X] * v.data[Y] - data[Y] * v.data[X];
		setData(x, y, z);
		return *this;
	}
	
	VectorType& normalize()
	{
		scale(1 / length());
		return *this;
	}
	
	VectorType& setData(Scalar x, Scalar y, Scalar z)
	{
		data[X] = x;
		data[Y] = y;
		data[Z] = z;
		return *this;
	}
    
    void getData(Scalar& x, Scalar& y, Scalar& z)
    {
        x = data[X];
        y = data[Y];
        z = data[Z];
    }
    
    void getData(Scalar* dest)
    {
        getData(*dest, *(dest + 1), *(dest + 2));
    }
	
	void print()
	{
		print(stdout);	
	}	
	
	void print(FILE* f)
	{
		fprintf(f, "(%lf, %lf, %lf)\n", data[X], data[Y], data[Z]);
	}
};