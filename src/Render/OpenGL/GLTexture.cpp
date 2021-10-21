// Jedem Anfang wohnt ein Zauber inne. Der uns beschützt und der uns hilft, zu leben.
#include "GLTexture.h"
#include "../../FileParser/stb_image.h"
#include "../../Util/LogHelperBE.h"
#include "../../Application/Application.h"

GLTexture::GLTexture(int interpolation, int wrapping, const std::string& path, int textureType, int imageType, float anisotropicLevel, bool mipMaps) : textureType(textureType), imageType(imageType), samples(1) {
    stbi_set_flip_vertically_on_load(1);
    if (path.empty()) {
        LogHelperBE::pushName("GLTexture");
        LogHelperBE::error("Path is not defined!");
        LogHelperBE::popName();
        return;
    }
    texCache = stbi_load((Application::absolutePath + path).c_str(), &width, &height, &bpp, 4);

    if (anisotropicLevel < 1.0f) {
        LogHelperBE::error("Anisotropic level is too low. Minimum is 1.0f");
        anisotropicLevel = 1.0f;
    }
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);

    glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, imageType, GL_UNSIGNED_BYTE, texCache);

    if (mipMaps) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        if (anisotropicLevel > 1.0f) {
            float maxAnisotropic;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic > anisotropicLevel ? anisotropicLevel : maxAnisotropic);
        }
    }

    if(texCache) {
        stbi_image_free(texCache);
    }
    unbind();
}

GLTexture::GLTexture(int width, int height, int interpolation, int samples, int textureType,
                     float anisotropicLevel, bool mipMaps) : textureType(textureType), texCache(
        nullptr), width(width), height(height), samples(samples), bpp(-1), imageType(textureType) {
    if (anisotropicLevel < 1.0f) {
        LogHelperBE::error("Anisotropic level is too low. Minimum is 1.0f");
        anisotropicLevel = 1.0f;
    }
    int multisampling = samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    glGenTextures(1, &texture);
    glBindTexture(multisampling, texture);

    if (samples < 2) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    }

    if (samples > 1) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, textureType, width, height, true);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType, GL_UNSIGNED_BYTE, nullptr);
    }

    if (mipMaps) {
        glTexParameteri(multisampling, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(multisampling);
        if (anisotropicLevel > 1.0f) {
            float maxAnisotropic;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropic);
            glTexParameterf(multisampling, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropic > anisotropicLevel ? anisotropicLevel : maxAnisotropic);
        }
    }
    unbind();
}

GLTexture::~GLTexture() {
    unbind();
    glDeleteTextures(1, &texture);
}

void GLTexture::resize(int newWidth, int newHeight) {
    if (texCache) {
        // TODO Is it ok to resize this?
        return;
    }
    bind();
    width = newWidth;
    height = newHeight;
    if (samples > 1) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, textureType, newWidth, newHeight, GL_TRUE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, textureType, newWidth, newHeight, 0, imageType, GL_UNSIGNED_BYTE, nullptr);
    }
}

void GLTexture::activate(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    bind();
}

void GLTexture::bind() const {
    glBindTexture(samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture);
}

void GLTexture::unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t GLTexture::get() const {
    return texture;
}

/*std::string GLTexture::getTypeName() const {
    switch (type) {
        case TEXTURE_IMAGE: return "textureImage";
        case TEXTURE_DIFFUSE: return "textureDiffuse";
        case TEXTURE_SPECULAR: return "textureSpecular";
        case TEXTURE_NORMAL_MAP: return "textureNormal";
        default: {
            LogHelperBE::pushName("GLTexture");
            LogHelperBE::error("Texture type " + std::to_string(type) + " is not defined");
            LogHelperBE::popName();
            return "";
        }
    }
}*/
