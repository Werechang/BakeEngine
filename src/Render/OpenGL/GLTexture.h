// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#pragma once

#define TEXTURE_IMAGE 0
#define TEXTURE_DIFFUSE 1
#define TEXTURE_SPECULAR 2
#define TEXTURE_NORMAL_MAP 3

#include <string>
#include "glad/gl.h"

class GLTexture {
public:
    GLTexture(const std::string& path, int textureType, int imageType) : GLTexture(GL_LINEAR, GL_REPEAT, path, textureType, imageType, 1.0f, true) {}
    GLTexture(int interpolation, int wrapping, const std::string& path, int textureType, int imageType, float anisotropicLevel, bool mipMaps);
    GLTexture(int width, int height, int interpolation, int samples, int textureType, float anisotropicLevel, bool mipMaps);
    ~GLTexture();

    void resize(int newWidth, int newHeight);

    void activate(uint32_t slot) const;
    void bind() const;
    void unbind() const;

    uint32_t get() const;
    //std::string getTypeName() const;
private:
    uint32_t texture;
    int width, height, bpp;
    int samples;
    unsigned char* texCache;
    //uint32_t type;
    int textureType, imageType;
};
