#include <framebuffer.h>
#include <shader.h>
#include <vertexarray.h>
#include <texture.h>
#include <glad/glad.h>
#include <array.h>
#include <stdlib.h>
unsigned int _attachments[10] = 
{ GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7,GL_COLOR_ATTACHMENT8,GL_COLOR_ATTACHMENT9};

struct Framebuffer* Framebuffer(unsigned int width,unsigned int height,unsigned int texCount)
{
	struct Framebuffer* fb = malloc(sizeof(struct Framebuffer));
	glCreateFramebuffers(1,&(fb->fboObject));
	glCreateRenderbuffers(1,&(fb->rboObject));
	fb->width = width;fb->height = height;
	fb->_lastActiveIndex = 0;
	fb->textures = Array(sizeof(struct Texture));
	glNamedRenderbufferStorage(fb->rboObject,GL_DEPTH24_STENCIL8,width,height);
	glNamedFramebufferRenderbuffer(fb->fboObject, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rboObject);  
  FramebufferAddTextures(fb,texCount);
  fb->va = VertexArray();
  float screenRect[] = {
    1.0f,1.0f,
    1.0f,-1.0f,
    -1.0f,-1.0f,
    1.0f,1.0f,
    -1.0,-1.0f,
    -1.0f,1.0f,
  };
  VertexArrayAddBuffer(fb->va,0,screenRect,sizeof(screenRect),sizeof(float)*2,0);
  VertexArrayFormatBufferElement(fb->va,0,TYPE_FLOAT,2);
  return fb;
}

struct Framebuffer* FramebufferUpdateSize(struct Framebuffer* fb,int width,int height)
{
  FramebufferFree(fb);
  unsigned int previousFbTexCount = ArrayGetCount(fb->textures);
  fb = Framebuffer(width,height,fb->textures->count);
  FramebufferAddTextures(fb,previousFbTexCount);
  return fb;
}
void FramebufferDrawBuffers(struct Framebuffer* fb,unsigned int buffersCount)
{
  glNamedFramebufferDrawBuffers(fb->fboObject,buffersCount,_attachments);
}

void FramebufferRender(struct Framebuffer* fb,struct Shader* shader)
{
  FramebufferActivate(fb);
  ShaderActivate(shader);
  VertexArrayBind(fb->va);
  glDrawArrays(GL_TRIANGLES,0,6);
}

void FramebufferAddTextures(struct Framebuffer* fb,unsigned int count)
{
  for(unsigned int i=0;i<count;i++)
  {
    struct Texture* fbtex;
    fbtex = Texture(fb->width,fb->height,GL_TEXTURE_2D,GL_RGB8,1);
    TextureSetFilter(fbtex,GL_NEAREST,GL_NEAREST);
    TextureSetWrap(fbtex,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    glNamedFramebufferTexture(fb->fboObject,GL_COLOR_ATTACHMENT0+(fb->_lastActiveIndex++),fbtex->object,0);
    ArrayPush(fb->textures,fbtex);
  }
  FramebufferDrawBuffers(fb,count);
}

void* FramebufferGetTexture(struct Framebuffer* fb,int index)
{
  struct Texture* fbtex = ArrayGetElement(fb->textures,index);
  return fbtex;
}

void FramebufferActivate(struct Framebuffer* fb)
{
  if(fb==NULL)
  {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    return;
  }
  glBindFramebuffer(GL_FRAMEBUFFER,fb->fboObject);
  glBindRenderbuffer(GL_RENDERBUFFER,fb->rboObject);
}

void FramebufferClear(struct Framebuffer* fb)
{
  float rgba[4] = {0.0f,0.0f,0.0f,0.0f};
  float d = 100.0f;
  if(fb==NULL)
  {
    glClearNamedFramebufferfv(0, GL_COLOR, 0, rgba);
    return;
  }
  glClearNamedFramebufferfv(fb->fboObject, GL_COLOR, 0, rgba);
  glClearNamedFramebufferfv(fb->fboObject, GL_DEPTH, 0, &d);
}
void FramebufferFree(struct Framebuffer* fb)
{
  glDeleteFramebuffers(1,&fb->fboObject);
  glDeleteRenderbuffers(1,&fb->rboObject);
  ArrayFree(fb->textures);
  VertexArrayFree(fb->va);
  free(fb);
}
