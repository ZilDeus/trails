#include <vertexarray.h>
#include <array.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

struct VertexArray* VertexArray()
{
	struct VertexArray* va = malloc(sizeof(struct VertexArray));
	glCreateVertexArrays(1,&va->object);
	va->buffers = Array(sizeof(struct VertexBuffer));
	va->lastActivatedIndex = 0;
	return va;
}
void VertexArrayBind(struct VertexArray* va)
{
	glBindVertexArray(va->object);
}
void VertexArrayAddBuffer(struct VertexArray* va,unsigned int index,void* data,unsigned int size,unsigned int stride,unsigned int offset)
{
	unsigned int tempBuffer = 0;
	glCreateBuffers(1,&tempBuffer);
	glNamedBufferData(tempBuffer,size,data,GL_DYNAMIC_DRAW);
	glVertexArrayVertexBuffer(va->object,index,tempBuffer,offset,stride);
	struct VertexBuffer vb;vb.index = index;vb.object = tempBuffer;vb.size = 0;
	ArrayPush(va->buffers,&vb);
}
void VertexArrayUpdateBuffer(struct VertexArray* va,unsigned int bufferIndex,unsigned int updatedbufferSize,void* updatedData)
{
  glNamedBufferData(VertexArrayGetBuffer(va,bufferIndex)->object,updatedbufferSize,updatedData,GL_DYNAMIC_DRAW);
}
void VertexArraySetElementBuffer(struct VertexArray* va,unsigned int elementBufferIndex)
{
  if(elementBufferIndex>=va->buffers->count)
  {
    fprintf(stderr,"ERROR:provided index is more than the vertex array added buffers\n");
    exit(1);
  }
  glVertexArrayElementBuffer(va->object,VertexArrayGetBuffer(va,elementBufferIndex)->object);
}
void VertexArrayFormatBufferElement(struct VertexArray* va,unsigned int bufferIndex,unsigned int elementType,unsigned int elementsCount)
{
	unsigned int size = VertexArrayGetSizeFromCount(elementType,elementsCount);

	struct VertexBuffer* currentBuffer = VertexArrayGetBuffer(va,bufferIndex);
	glEnableVertexArrayAttrib(va->object,va->lastActivatedIndex);
	glVertexArrayAttribFormat(va->object,va->lastActivatedIndex,elementsCount,elementType,GL_FALSE,currentBuffer->size);
	glVertexArrayAttribBinding(va->object,va->lastActivatedIndex++,currentBuffer->index);
	currentBuffer->size+=size;
	VertexArraySetBuffer(va,bufferIndex,*currentBuffer);
}
unsigned int VertexArrayGetSizeFromCount(unsigned int type,unsigned int count)
{
	if(type == TYPE_UNSIGNED_BYTE||type == TYPE_BYTE)
		return count * sizeof(char);
	else if (type == TYPE_UNSIGNED_SHORT || type == TYPE_SHORT)
		return count * sizeof(float);
	else if (type == TYPE_UNSIGNED_INT || type == TYPE_INT || type == TYPE_FLOAT)
		return count * sizeof(float);
	else if (type == TYPE_DOUBLE)
		return count * sizeof(double);
}

struct VertexBuffer* VertexArrayGetBuffer(struct VertexArray* va,unsigned int index)
{
	return ArrayGetElement(va->buffers,index);
}

void VertexArraySetBuffer(struct VertexArray* va,unsigned int index,struct VertexBuffer vb)
{
	ArraySetElement(va->buffers,index,&vb);
}

void VertexArrayFree(struct VertexArray* va)
{
	unsigned int buffersCount = ArrayGetCount(va->buffers);
	for(int i=0;i<buffersCount;i++)
	{
		struct VertexBuffer* buffer = ArrayGetElement(va->buffers,i);
		glDeleteBuffers(1,&buffer->object);
	}
	ArrayFree(va->buffers);
	glDeleteVertexArrays(1,&va->object);
	free(va);
}
