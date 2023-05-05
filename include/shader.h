#ifndef _SHADER_H
#define _SHADER_H

struct Shader
{
	unsigned int program_id;
};
struct Uniform
{
	struct Shader* shader;
	int location;
};


void ShaderActivate(struct Shader* shader);

struct Shader* Shader(const char* vspath,const char* fspath);

unsigned int _CreateShader(const int type,const char* path);

int IsShaderValid(struct Shader* shader);

char* _GetShaderSrc(const char* path);

void ShaderFree(struct Shader* shader);

void ShaderSetSamplerIndex(struct Shader* shader , char* name,unsigned int index);

struct Uniform Uniform(struct Shader* shader,const char* name);

void UniformSet1Float(struct Uniform uniform,float x);
void UniformSet2Float(struct Uniform uniform,float x,float y);
void UniformSet3Float(struct Uniform uniform,float x,float y,float z);
void UniformSet4Float(struct Uniform uniform,float x,float y,float z,float w);

void UniformSet1Int(struct Uniform uniform,int x);
void UniformSet2Int(struct Uniform uniform,int x,int y);
void UniformSet3Int(struct Uniform uniform,int x,int y,int z);
void UniformSet4Int(struct Uniform uniform,int x,int y,int z,int w);

void UniformSet1UInt(struct Uniform uniform,unsigned int x);
void UniformSet2UInt(struct Uniform uniform,unsigned int x,unsigned int y);
void UniformSet3UInt(struct Uniform uniform,unsigned int x,unsigned int y,unsigned int z);
void UniformSet4UInt(struct Uniform uniform,unsigned int x,unsigned int y,unsigned int z,unsigned int w);

void UniformSetSampler(struct Uniform uniform,int sampler);

void UniformSetMatrix(struct Uniform uniform,void* p);

#endif
