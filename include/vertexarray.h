#ifndef _VERTEXARRAY_H_
#define _VERTEXARRAY_H_
#include "../include/array.h"


#define TYPE_BYTE GL_BYTE 
#define TYPE_UNSIGNED_BYTE GL_UNSIGNED_BYTE 
#define TYPE_UNSIGNED_INT GL_UNSIGNED_INT 
#define TYPE_FLOAT GL_FLOAT 
#define TYPE_DOUBLE GL_DOUBLE 
#define TYPE_INT GL_INT 
#define TYPE_SHORT GL_SHORT 
#define TYPE_UNSIGNED_SHORT GL_UNSIGNED_SHORT 


struct VertexBuffer
{
	unsigned int object;
	unsigned int index;
	unsigned int size;
};

struct VertexArray
{
	unsigned int object;
	unsigned int lastActivatedIndex;
	//unsigned int buffersCount;
	struct Array* buffers;
};

struct VertexArray* VertexArray();

void VertexArrayAddBuffer(struct VertexArray* va,unsigned int index,void* data,unsigned int size,unsigned int stride,unsigned int offset);

void VertexArrayBind(struct VertexArray* va);

void VertexArrayFormatBufferElement(struct VertexArray* va,unsigned int bufferIndex,unsigned int elementType,unsigned int elementsCount);

unsigned int VertexArrayGetSizeFromCount(unsigned int type,unsigned int count);

struct VertexBuffer* VertexArrayGetBuffer(struct VertexArray* va,unsigned int index);

void VertexArraySetElementBuffer(struct VertexArray* va,unsigned int elementBufferIndex);

void VertexArrayUpdateBuffer(struct VertexArray* va,unsigned int bufferIndex,unsigned int updatedbufferSize,void* updatedData);

void VertexArraySetBuffer(struct VertexArray* va,unsigned int index,struct VertexBuffer vb);

void VertexArrayFree(struct VertexArray* va);

#endif
