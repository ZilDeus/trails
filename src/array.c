#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <array.h>

struct Array* Array(unsigned int size)
{
	struct Array *array = malloc(sizeof(struct Array));
	array->count=0;
	array->_count=0;
	array->size = size;
	array->data = 0;
	array->end = 0;
	return array;
}

struct Buffer* Buffer(unsigned int size)
{
	struct Buffer *buff = malloc(sizeof(struct Buffer));
	buff->size = size;
	buff->data = malloc(size);
	buff->end=buff->data; 
	return buff;
}

void ArraySetData(struct Array* arr,void* data,unsigned int count)
{
	arr->data = data;
	arr->count = count;
	arr->_count = count;
}

char ArrayIsEmpty(struct Array* arr)
{
	return arr->data==0;
}

void ArrayFree(struct Array* arr)
{
	free(arr->data);
	arr->data = 0;
	free(arr);
}

void BufferFree(struct Buffer* buff)
{
	free(buff->data);
	buff->data = 0;
	free(buff);
}
void ArrayResize_t(struct Array* arr,unsigned int size)
{
  arr->count = size;
}
void ArrayResize(struct Array* arr,unsigned int size)
{
	void* temp = malloc(arr->count * arr->size);
	memcpy(temp,arr->data,arr->count * arr->size);
	free(arr->data);
	arr->data = malloc(size *arr->size);
	memcpy(arr->data,temp,arr->count*arr->size);
	free(temp);
}
void ArrayPush(struct Array* arr,void* data)
{
	if(arr->count+1>arr->_count)
	{
		arr->_count = (arr->count+1)*2;
		ArrayResize(arr,arr->_count);
		arr->end = (char*)arr->data + (arr->count * arr->size);
	}
	memcpy(arr->end,data,arr->size);
	arr->end =((char*)(arr->end))+arr->size;
	arr->count++;
}
void ArrayRemoveAt(struct Array* arr,unsigned int removeAt)
{
  if (removeAt>=arr->count){
    fprintf(stderr,"ERROR:invalid index");
    exit(1);
  }
  for (unsigned int i = removeAt;i<arr->count-1;i++)
  {
    ArraySetElement(arr,i,ArrayGetElement(arr,i+1));
  }
  arr->count--;
  arr->end-=arr->size;
}
void BufferPush(struct Buffer* buff,void* data,unsigned int size)
{
	if(buff->end-buff->data==buff->size)
	{
		printf("crossed array boundries\narray start at %p and must end at %p\ninstead the is at %p\n",buff->data,buff->data+buff->size,buff->end);
	}
	memcpy(buff->end,data,size);
	buff->end =(char*)buff->end + size;
}

void* ArrayGetData(struct Array* arr)
{
	return arr->data;
}

unsigned int ArrayGetCount(struct Array* arr)
{
	return arr->count;
}

void* BufferGetData(struct Buffer* buff)
{
	return buff->data;
}

unsigned int BufferGetSize(struct Buffer* buff)
{
	return buff->size;
}

void* ArrayGetElement(struct Array* arr,unsigned int i)
{
	if(i<arr->count)
		return ((char*)arr->data)+(i*arr->size);
	printf("i value (%d) must in the range of 0 and the array Count(%d)\n",i,arr->count);
	exit(1);
}
void ArraySetElement(struct Array* arr,unsigned int i,void* data)
{
	if(i<arr->count)
	{
		memcpy((arr->data)+(i*arr->size),data,arr->size);
		return;
	}
	printf("i value (%d) must in the range of 0 and the array Count(%d)\n",i,arr->count);
	exit(1);

}
