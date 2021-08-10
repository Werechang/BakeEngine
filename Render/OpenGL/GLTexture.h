// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#ifndef BAKEENGINE_GLTEXTURE_H
#define BAKEENGINE_GLTEXTURE_H

#include "glad/glad.h"

class GLTexture {
private:
    unsigned int texPtr;
    int width, height, bpp;
    unsigned char* texCache;
public:
    explicit GLTexture(const char* path) : GLTexture(GL_LINEAR, GL_REPEAT, path) {}
    GLTexture(int interpolation, int sampling, const char* path);
    ~GLTexture();

    void bind(unsigned int slot) const;
    static void unbind();
};


#endif
