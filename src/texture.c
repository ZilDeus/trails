#include <texture.h>
#include <glad/glad.h>
#include <stdlib.h>

struct Texture* Texture(unsigned int width,unsigned int height,int type,int format,int levels)
{
	struct Texture *tex = malloc(sizeof(struct Texture));
	tex->type = type;
	tex->levelsCount = levels;
	tex->width = width;
	tex->height = height;
	glCreateTextures(type,1,&tex->object);
	glTextureStorage2D(tex->object,levels,format,width,height);

	return tex;
}

void TextureImage(struct Texture* tex,unsigned int width,unsigned int height,int type,int format,int level,void* data)
{
	glTextureSubImage2D(tex->object,level,0,0,width,height,format,type,data);
}
void TextureCubemap(struct Texture* tex,unsigned int width,unsigned int height,int type,int format,int level,void* data,int face)
{
	glTextureSubImage3D(tex->object,level,0,0,face,width,height,1,format,type,data);
}
void TextureSetWrap   (struct Texture* tex,unsigned int x,unsigned int y,unsigned int z)
{
	glTextureParameteri(tex->object, GL_TEXTURE_WRAP_S, x );
	glTextureParameteri(tex->object, GL_TEXTURE_WRAP_T, y );
	glTextureParameteri(tex->object, GL_TEXTURE_WRAP_R, z );
}
void TextureSetFilter (struct Texture* tex,unsigned int min,unsigned int mag)
{
	glTextureParameteri(tex->object, GL_TEXTURE_MIN_FILTER, min);
	glTextureParameteri(tex->object, GL_TEXTURE_MAG_FILTER, mag);
}
void TextureSetColor  (struct Texture* tex,float* rgb)
{
	glTextureParameterfv(tex->object,GL_TEXTURE_BORDER_COLOR,rgb);
}
void TextureSetIndex  (struct Texture* tex,int index)
{
	glBindTextureUnit(index,tex->object);
}
void TextureFree(struct Texture* tex)
{
	glDeleteTextures(1,&tex->object);
	free(tex);
}
