#include <shader.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

struct Shader* Shader(const char* vspath,const char* fspath)
{
	struct Shader* shader = malloc(sizeof(struct Shader));
	shader->program_id = glCreateProgram();
	unsigned int vertexshader = _CreateShader(GL_VERTEX_SHADER,vspath);
	unsigned int fragmentshader = _CreateShader(GL_FRAGMENT_SHADER,fspath);
	if(!vertexshader||!fragmentshader)
	{
		printf("you fucking idiot shader is: %s and %s\n",vspath,fspath);
		struct Shader* errorshader = 0;
    exit(1);
		return errorshader;
	}
	glAttachShader(shader->program_id,vertexshader);
	glAttachShader(shader->program_id,fragmentshader);
	glLinkProgram(shader->program_id);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	return shader;
}

int IsShaderValid(struct Shader* shader)
{
	return shader->program_id;
}

void ShaderSetSamplerIndex(struct Shader* shader , char* name,unsigned int index)
{
	ShaderActivate(shader);
	glProgramUniform1i(shader->program_id,glGetUniformLocation(shader->program_id,name),index);
}

void ShaderActivate(struct Shader* shader)
{
	glUseProgram(shader->program_id);
}

unsigned int _CreateShader(const int type,const char* path)
{
	unsigned int shader = glCreateShader(type);
	char* src = _GetShaderSrc(path);
	glShaderSource(shader,1,(const char**)&src,NULL);
	glCompileShader(shader);
	int compilestatus;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compilestatus);
	if(compilestatus)
	{
		free(src);
		return shader;
	}
	int strlen =0;
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&strlen);
	char* log = malloc(strlen);
	glGetShaderInfoLog(shader,strlen,&strlen,log);
	printf("%s/n",src);
	printf("%s",log);
	glDeleteShader(shader);
	free(log);
	free(src);
	return 0;
}

char* _GetShaderSrc(const char* path)
{
	char* pointer;
	FILE* file;
	file = fopen(path,"rb");
	if(file==NULL)
		return 0;
	//variables needed for the end of the func
	int length = 0;
	char c;
	//fgetc(file)ting the byte count of the vertexshader
	while((c=fgetc(file))>'\0')
			length++;
	rewind(file);
	pointer = (char*)calloc(length+1,1);
	fread(pointer,1,length,file);
	printf("file length is %d\n",length);
	fclose(file);
	return pointer;
}

void ShaderFree(struct Shader* shader)
{
	glDeleteProgram(shader->program_id);
}


struct Uniform Uniform(struct Shader* shader,const char* name)
{
	struct Uniform uni;
	uni.shader = shader;
	uni.location = glGetUniformLocation(uni.shader->program_id,name);
	return uni;
}

void UniformSet1Float(struct Uniform uniform,float x)
{
	ShaderActivate(uniform.shader);
	glProgramUniform1f(uniform.shader->program_id,uniform.location,x);
}

void UniformSet2Float(struct Uniform uniform,float x,float y)
{
	ShaderActivate(uniform.shader);
	glProgramUniform2f(uniform.shader->program_id,uniform.location,x,y);
}
void UniformSet3Float(struct Uniform uniform,float x,float y,float z)
{
	ShaderActivate(uniform.shader);
	glProgramUniform3f(uniform.shader->program_id,uniform.location,x,y,z);
}
void UniformSet4Float(struct Uniform uniform,float x,float y,float z,float w)
{
	ShaderActivate(uniform.shader);
	glProgramUniform4f(uniform.shader->program_id,uniform.location,x,y,z,w);
}

void UniformSet1Int(struct Uniform uniform,int x)
{
	ShaderActivate(uniform.shader);
	glProgramUniform1i(uniform.shader->program_id,uniform.location,x);
}
void UniformSet2Int(struct Uniform uniform,int x,int y)
{
	ShaderActivate(uniform.shader);
	glProgramUniform2i(uniform.shader->program_id,uniform.location,x,y);
}
void UniformSet3Int(struct Uniform uniform,int x,int y,int z)
{
	ShaderActivate(uniform.shader);
	glProgramUniform3i(uniform.shader->program_id,uniform.location,x,y,z);
}
void UniformSet4Int(struct Uniform uniform,int x,int y,int z,int w)
{
	ShaderActivate(uniform.shader);
	glProgramUniform4i(uniform.shader->program_id,uniform.location,x,y,z,w);
}


void UniformSet1UInt(struct Uniform uniform,unsigned int x)
{
	ShaderActivate(uniform.shader);
	glProgramUniform1ui(uniform.shader->program_id,uniform.location,x);
}
void UniformSet2UInt(struct Uniform uniform,unsigned int x,unsigned int y)
{
	ShaderActivate(uniform.shader);
	glProgramUniform2ui(uniform.shader->program_id,uniform.location,x,y);
}
void UniformSet3UInt(struct Uniform uniform,unsigned int x,unsigned int y,unsigned int z)
{
	ShaderActivate(uniform.shader);
	glProgramUniform3ui(uniform.shader->program_id,uniform.location,x,y,z);
}
void UniformSet4UInt(struct Uniform uniform,unsigned int x,unsigned int y,unsigned int z,unsigned int w)
{
	ShaderActivate(uniform.shader);
	glProgramUniform4ui(uniform.shader->program_id,uniform.location,x,y,z,w);
}

void UniformSetMatrix(struct Uniform uniform,void* p)
{
	ShaderActivate(uniform.shader);
	glProgramUniformMatrix4fv(uniform.shader->program_id,uniform.location,1,0,p);
}

