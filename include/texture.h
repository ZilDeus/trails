#ifndef _TEXTURE_H
#define _TEXTURE_H

#define TEXTURE_FILTER_NEAREIST GL_LINEAR
#define TEXTURE_FILTER_LINEAR GL_NEAREST
#define TEXTURE_WRAP_REPEAT GL_REPEAT
#define TEXTURE_WRAP_MIRRORED GL_MIRRORED_REPEAT
#define TEXTURE_WRAP_EDGE GL_CLAMP_TO_EDGE
#define TEXTURE_WRAP_COLOR GL_CLAMP_TO_COLOR 
#define CM_SIZE 10.0f

struct Texture
{
	int width,height;
	int type;
	int levelsCount;
	unsigned int object;
};

struct Texture* Texture(unsigned int width,unsigned int height,int type,int format,int levels);
void TextureImage     (struct Texture* tex,unsigned int width,unsigned int height,int type,int format,int level,void* data);
void TextureCubemap   (struct Texture* tex,unsigned int width,unsigned int height,int type,int format,int level,void* data,int face);

void TextureSetWrap   (struct Texture* tex,unsigned int x,unsigned int y,unsigned int z);
void TextureSetFilter (struct Texture* tex,unsigned int min,unsigned int mag);
void TextureSetColor  (struct Texture* tex,float* rgb);
void TextureSetIndex  (struct Texture* tex,int index);

void TextureFree(struct Texture* tex);
#endif
