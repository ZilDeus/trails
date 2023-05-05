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
  struct Point dir;
  a.x +=(b.x-a.x)*r;
  a.y +=(b.y-a.y)*r;
  return a;
}
int main()
{
  unsigned int maxCount = 100;
  struct Array* pointsArr = Array(sizeof(struct Point));
  if(!glfwInit())
    return -1;
  currTime = glfwGetTime();
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  screenWidth = mode->width;
  screenHeight = mode->height;
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED,GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING,GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);
  glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,GLFW_TRUE);
  glfwWindowHint(GLFW_AUTO_ICONIFY,GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* win;
  win = glfwCreateWindow(mode->width-1,mode->height-1, "Trails",NULL,NULL);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwMakeContextCurrent(win);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  struct Shader* mapShader = Shader("../res/map.vs", "../res/map.fs");
  struct Shader* screenShader = Shader("../res/default.vs", "../res/default.fs");
  struct Shader* subtractShader = Shader("../res/default.vs", "../res/subtract.fs");
  struct Framebuffer *mapFb = Framebuffer(screenWidth,screenHeight);
  struct Framebuffer *SubtractFb = Framebuffer(screenWidth,screenHeight);
  FramebufferAddTextures(mapFb,1);
  FramebufferAddTextures(SubtractFb,1);
  struct VertexArray* screenVa;
  screenVa = VertexArray();
  {
    float screenBuffer[] = 
    {
      1.0f,-1.0f,
      1.0f,1.0f,
      -1.0f,-1.0f,
      -1.0f,-1.0f,
      1.0f,1.0f,
      -1.0f,1.0f,
    };
    VertexArrayAddBuffer(screenVa, 0, screenBuffer,sizeof(screenBuffer),sizeof(float)*2,0);
    VertexArrayFormatBufferElement(screenVa, 0, TYPE_FLOAT, 2);
  }
  float mousePos[2];
  int imgWidth,imgHeight;
  unsigned char* imgData;
  stbi_set_flip_vertically_on_load(1);
  imgData = stbi_load("../res/tex.png",&imgWidth,&imgHeight,0,3);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glfwSetKeyCallback(win,key_callback);
  while (!glfwWindowShouldClose(win)){
    deltaTime = glfwGetTime() - currTime;
    currTime = glfwGetTime();
    FramebufferActivate(mapFb);
    //glClear(GL_COLOR_BUFFER_BIT);
    ShaderActivate(mapShader);
    VertexArrayBind(screenVa);
    getMouseScreenf(win,mousePos);
    glUniform2f(0,mousePos[0],mousePos[1]);
    glDrawArrays(GL_POINTS,0,1);
    FramebufferActivate(SubtractFb);
    ShaderActivate(subtractShader);
    TextureSetIndex(FramebufferGetTexture(mapFb,0),0);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    VertexArrayBind(screenVa);
    ShaderActivate(screenShader);
    TextureSetIndex(FramebufferGetTexture(SubtractFb,0),0);
    glDrawArrays(GL_TRIANGLES,0,6);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  ShaderFree(mapShader);
  ArrayFree(pointsArr);
  STBI_FREE(imgData);
  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
