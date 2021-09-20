// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#ifndef BAKEENGINE_GLTEXTURE_H
#define BAKEENGINE_GLTEXTURE_H
#define TEXTURE_IMAGE 0
#define TEXTURE_DIFFUSE 1
#define TEXTURE_SPECULAR 2
#define TEXTURE_NORMAL_MAP 3

#include <string>
#include "glad/glad.h"

class GLTexture {
private:
    unsigned int texPtr;
    int width, height, bpp;
    unsigned char* texCache;
    unsigned int type;
public:
    GLTexture(const char* path, unsigned int type, bool sRGB) : GLTexture(GL_LINEAR, GL_REPEAT, path, type, sRGB) {}
    GLTexture(int interpolation, int sampling, const char* path, unsigned int type, bool sRGB);
    ~GLTexture();

    void bind(unsigned int slot) const;
    static void unbind();

    std::string getTypeName() const;
};


#endif
