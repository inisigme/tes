#ifndef __TEXTURES__H__
#define __TEXTURES__H__

#include <GL/glew.h>
bool LoadTexture(const char *fileName, GLenum target, GLint level = 0);
bool LoadTextures(const char **fileNames, GLenum target, GLint layers);
#endif 
