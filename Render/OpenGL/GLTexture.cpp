// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#include <iostream>
#include "GLTexture.h"
#include "../../FileParser/stb_image.h"

GLTexture::GLTexture(int interpolation, int sampling, const char* path, unsigned int type) : type(type) {
    stbi_set_flip_vertically_on_load(1);
    texCache = stbi_load(path, &width, &height, &bpp, 4);

    glGenTextures(1, &texPtr);
    glBindTexture(GL_TEXTURE_2D, texPtr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampling);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampling);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCache);

    // Load image first!
    glGenerateMipmap(GL_TEXTURE_2D);
    if(texCache) {
        stbi_image_free(texCache);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &texPtr);
}

void GLTexture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texPtr);
}

void GLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

std::string GLTexture::getTypeName() const {
    switch (type) {
        case TEXTURE_IMAGE: return "textureImage";
        case TEXTURE_DIFFUSE: return "textureDiffuse";
        case TEXTURE_SPECULAR: return "textureSpecular";
        case TEXTURE_NORMAL_MAP: return "textureNormal";
        default: std::cerr << "[BakeEngine]:[Warning] Texture type " << type << " is not defined" << std::endl;
            return "";
    }
}
