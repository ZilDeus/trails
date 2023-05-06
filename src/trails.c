#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vertexarray.h>
#include <array.h>
#include <framebuffer.h>
#include <shader.h>
#include <texture.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stbi.h"

unsigned int screenWidth;
unsigned int screenHeight;
float deltaTime;
double currTime;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window,GLFW_TRUE);
}
struct Point{
  float x,y;
};
struct Point_UV{
  float x,y,u,v;
};
struct Curve_Point{
  float x,y;
  struct Point normal;
};
void getMouseScreenf(GLFWwindow* win,float _return[2])
{
  double x,y;
  int px,py;
  glfwGetCursorPos(win,&x,&y);
  glfwGetWindowPos(win,&px,&py);
  x+=(double)px;
  y+=(double)py;
  _return[0] = 2.0f * ((float)x/screenWidth) - 1.0f;
  _return[1] = -2.0f * ((float)y/screenHeight) + 1.0f;
}
float square(float f)
{
  return f*f;
}
float Distance(struct Point* a,struct Point* b)
{
  return square(b->x-a->x)+square(b->y-a->y);
}
struct Point MoveTo(struct Point a,struct Point b,float r)
{
  a.y +=(b.y-a.y)*r;
  a.x +=(b.x-a.x)*r;
  return a;
}
int main()
{
  unsigned int maxCount = 100;
  struct Array* pointsArr = Array(sizeof(struct Point));
  struct Array* curvePointsArr = Array(sizeof(struct Curve_Point));
  if(!glfwInit())
    return -1;
  currTime = glfwGetTime();
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  screenWidth = mode->width;
  screenHeight = mode->height-1;
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED,GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING,GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
  glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* win;
  win = glfwCreateWindow(screenWidth,screenHeight, "Trails",NULL,NULL);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwMakeContextCurrent(win);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  struct Shader* lineShader = Shader("../res/trail.vs", "../res/trail.fs");
  struct Shader* bloomShader = Shader("../res/default.vs", "../res/bloom.fs");
  struct Shader* blurShader = Shader("../res/default.vs", "../res/blur.fs");
  struct Shader* screenShader = Shader("../res/default.vs", "../res/default.fs");
  struct Framebuffer* bloomFb = Framebuffer(screenWidth,screenHeight,2);
  struct Framebuffer* renderFb = Framebuffer(screenWidth,screenHeight,1);
  struct Framebuffer* blurFbs[]={
    Framebuffer(screenWidth, screenHeight,1),
    Framebuffer(screenWidth, screenHeight,1),
  };
  float width = 0.10;
  float tempPoint[2] = {1.0f,1.0f};
  getMouseScreenf(win,(float*)tempPoint);
  ArrayPush(pointsArr,tempPoint);
  int imgWidth,imgHeight;
  unsigned char* imgData;
  stbi_set_flip_vertically_on_load(1);
  imgData = stbi_load("../res/tex.png",&imgWidth,&imgHeight,0,3);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  struct Texture* lineTex = Texture(imgWidth, imgHeight, GL_TEXTURE_2D,GL_RGB8, 1);
  TextureImage(lineTex, imgWidth,imgHeight,GL_UNSIGNED_BYTE,GL_RGB,0,imgData);
  struct VertexArray *lineVa;
  unsigned int ecount = (maxCount-1)*6;
  unsigned int elements[ecount];
  for (unsigned int i=0;i<maxCount-1;i++)
  {
    elements[6*i + 0] = i*2 + 0;
    elements[6*i + 1] = i*2 + 2;
    elements[6*i + 2] = i*2 + 3;
    elements[6*i + 3] = i*2 + 1;
    elements[6*i + 4] = i*2 + 0;
    elements[6*i + 5] = i*2 + 3;
  }
  struct Point_UV pointsBuffer[maxCount*2];

  lineVa = VertexArray();

  VertexArrayAddBuffer(lineVa, 0, elements, sizeof(elements), sizeof(int)*3, 0);
  VertexArraySetElementBuffer(lineVa, 0);
  VertexArrayAddBuffer(lineVa, 1, pointsBuffer, sizeof(pointsBuffer), sizeof(struct Point_UV), 0);
  VertexArrayFormatBufferElement(lineVa, 1, TYPE_FLOAT, 4);

  glfwSetKeyCallback(win,key_callback);
  struct Uniform hori = Uniform(blurShader,"horizontal");
  while (!glfwWindowShouldClose(win)){
    deltaTime = glfwGetTime() - currTime;
    currTime = glfwGetTime();

    ecount = (6*(pointsArr->count-1));
    FramebufferActivate(renderFb);
    glClear(GL_COLOR_BUFFER_BIT);
    VertexArrayBind(lineVa);
    TextureSetIndex(lineTex,0);
    VertexArrayUpdateBuffer(lineVa,1,sizeof(pointsBuffer),pointsBuffer);
    ShaderActivate(lineShader);
    glDrawElements(GL_TRIANGLES, ecount,GL_UNSIGNED_INT,0);
    VertexArrayBind(bloomFb->va);
    FramebufferActivate(bloomFb);
    ShaderActivate(bloomShader);
    TextureSetIndex(FramebufferGetTexture(renderFb,0),0);
    glDrawArrays(GL_TRIANGLES,0,6);
    ShaderActivate(screenShader);
    char horizontal = 1, first_iteration = 1;
    int amount = 8;
    TextureSetIndex(FramebufferGetTexture(bloomFb,1), 1);
    ShaderActivate(blurShader);
    for (unsigned int i = 0; i < amount; i++)
    {
      FramebufferActivate(blurFbs[horizontal]);
      glUniform1i(hori.location, horizontal);

      if (first_iteration)
      {
        TextureSetIndex(FramebufferGetTexture(bloomFb,1), 1);
        first_iteration = 0;
      }
      else
      {
        TextureSetIndex(FramebufferGetTexture(blurFbs[!horizontal],0),1);
      }

      glDrawArrays(GL_TRIANGLES, 0, 6);

      horizontal = !horizontal;
    }
    TextureSetIndex(FramebufferGetTexture(blurFbs[!horizontal],0),1);
    TextureSetIndex(FramebufferGetTexture(bloomFb,0),0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDrawArrays(GL_TRIANGLES,0,6);
    glfwSwapBuffers(win);
    getMouseScreenf(win,(float*)tempPoint);
    if (Distance(ArrayGetElement(pointsArr,pointsArr->count-1),(struct Point*)tempPoint)!=0.0f)
    {
      if (pointsArr->count>=maxCount) {
        ArrayRemoveAt(pointsArr,0);
      }
      struct Point* last = ArrayGetElement(pointsArr,pointsArr->count-1);
      ArrayPush(pointsArr,tempPoint);
    }
    struct Point *a;
    if(pointsArr->count >1)
    {
      a = ArrayGetElement(pointsArr,0);
      struct Point* b = ArrayGetElement(pointsArr,1);
      *a = MoveTo(*a,*b,deltaTime*20.0f);
      ArraySetElement(pointsArr,0,a);
      if(pointsArr->count>2)
        if (Distance(ArrayGetElement(pointsArr,0),ArrayGetElement(pointsArr,1)) < 0.001f)
        {
          ArrayRemoveAt(pointsArr, 0);
        }
    }
    int i;
    for (i=0;i<pointsArr->count;i++)
    {
      width = ((float)i/maxCount);
      a = ArrayGetElement(pointsArr,i);
      pointsBuffer[i*2].x = a->x+(0.01f);
      pointsBuffer[i*2].y = a->y+(0.01f);
      pointsBuffer[i*2].u = 1.0f - width;
      pointsBuffer[i*2].v = 1.0f;
      pointsBuffer[i*2 + 1].x = a->x-(0.01f);
      pointsBuffer[i*2 + 1].y = a->y-(0.01f);
      pointsBuffer[i*2 + 1].u = 1.0f - width;
      pointsBuffer[i*2 + 1].v = 0.0f;
    }
    glfwPollEvents();
  }
  VertexArrayFree(lineVa);
  TextureFree(lineTex);
  ShaderFree(lineShader);
  ArrayFree(pointsArr);
  STBI_FREE(imgData);
  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
