// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#pragma once

#define TEXTURE_IMAGE 0
#define TEXTURE_DIFFUSE 1
#define TEXTURE_SPECULAR 2
#define TEXTURE_NORMAL_MAP 3

#include <string>
#include "glad/gl.h"

class GLTexture {
private:
    unsigned int texPtr;
    int width, height, bpp;
    unsigned char* texCache;
    unsigned int type;
public:
    GLTexture(const std::string& path, unsigned int type, bool sRGB) : GLTexture(GL_LINEAR, GL_REPEAT, path, type, sRGB, 1, true) {}
    GLTexture(int interpolation, int sampling, const std::string& path, unsigned int type, bool sRGB, float anisotropicLevel, bool mipMaps);
    ~GLTexture();

    void bind(unsigned int slot) const;
    void unbind() const;

    std::string getTypeName() const;
};
