#include "ParticleTexture.hpp"

namespace parti
{
    ParticleTexture::ParticleTexture() {};

    ParticleTexture::ParticleTexture(const char * texturePath, int numberOfRows)
    {
        this->textureID = gps::Model3D::ReadTextureFromFile(texturePath);
        this->numberOfRows = numberOfRows;
    }

    GLuint ParticleTexture::getTextureID()
    {
        return textureID;
    }

    int ParticleTexture::getNumberOfRows()
    {
        return numberOfRows;
    }
}