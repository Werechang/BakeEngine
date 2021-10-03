// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#include "GLTexture.h"
#include "../../FileParser/stb_image.h"
#include "../../Util/LogHelperBE.h"
#include "../../Application/Application.h"

GLTexture::GLTexture(int interpolation, int sampling, const char* path, unsigned int type, bool sRGB, float anisotropicLevel, bool mipMaps) : type(type) {
    stbi_set_flip_vertically_on_load(1);
    texCache = stbi_load((Application::absolutePath + path).c_str(), &width, &height, &bpp, 4);

    if (anisotropicLevel < 1.0f) {
        LogHelperBE::error("Anisotropic level is too low. Minimum is 1.0f");
        anisotropicLevel = 1.0f;
    }

    glGenTextures(1, &texPtr);
    glBindTexture(GL_TEXTURE_2D, texPtr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampling);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampling);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);

    glTexImage2D(GL_TEXTURE_2D, 0, sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCache);

    if (mipMaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (anisotropicLevel > 1.0f) {
            float maxAnisotropic;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropic);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropic > anisotropicLevel ? anisotropicLevel : maxAnisotropic);
        }
    }

    if(texCache) {
        stbi_image_free(texCache);
    }
    unbind();
}

GLTexture::~GLTexture() {
    unbind();
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
        default: {
            LogHelperBE::pushName("GLTexture");
            LogHelperBE::error(("Texture type " + std::to_string(type) + " is not defined").c_str());
            LogHelperBE::popName();
            return "";
        }
    }
}
