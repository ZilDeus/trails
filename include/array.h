#ifndef _ARRAY_H
#define _ARRAY_H
struct Array
{
    unsigned int count,_count;
    unsigned int size;
    void* data;
    void* end;
};

struct Buffer
{
	unsigned int size;
	void* data;
	void* end;
};

struct Buffer* Buffer(unsigned int size);

struct Array* Array(unsigned int size);

void* ArrayGetData(struct Array* arr);

unsigned int ArrayGetCount(struct Array* arr);

void* BufferGetData(struct Buffer* buff);

unsigned int BufferGetSize(struct Buffer* buff);

void ArraySetData(struct Array* arr,void* data,unsigned int count);

void ArrayPush(struct Array* arr,void* data);

void BufferPush(struct Buffer* buff,void* data,unsigned int size);

void ArrayResize(struct Array* arr,unsigned int size);

void ArrayResize_t(struct Array* arr,unsigned int size);

void ArrayRemoveAt(struct Array* arr,unsigned int i);

void ArrayFree(struct Array* arr);

void BufferFree(struct Buffer* buff);

void* ArrayGetElement(struct Array* arr,unsigned int i);

void ArraySetElement(struct Array* arr,unsigned int i,void* data);

char ArrayIsEmpty(struct Array* arr);

#endif
