#include "Terrain.hpp"

#include <stdio.h>

namespace enty
{
    Terrain::Terrain(glm::vec3 position):Entity(position, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))
    {
        this->position = position;
    }


    float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
    {
        float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
        float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
        float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
        float l3 = 1.0f - l1 - l2;
        return l1 * p1.y + l2 * p2.y + l3 * p3.y;
    }

    float Terrain::floatModulo(float a, float b)  // pretuie asta altfel are urcare si coborare in trepte
    {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        while (a >= b)
        {
            a -= b;
        }
        return a;
    }

    float Terrain::getTerrainHeight(float worldX, float worldZ)
    {
        float terrainX = worldX - this->position.x;
        float terrainZ = worldZ - this->position.z;
        float gridSquareSize = TERRAIN_SIZE / (heightsLength - 1); // cumva de cate ori e mai mare decat ar fi textura, cate texturi au trebuit sa faca terenul
        int gridX = (int) floor(terrainX / gridSquareSize);
        int gridZ = (int) floor(terrainZ / gridSquareSize);

        if (gridX >= heightsLength ||
            gridZ >= heightsLength ||
            gridX < 0 ||
            gridZ < 0)
        {
            return 0;
        }
        // coordonatele x, z in grid 
        float xCoord = floatModulo(terrainX, gridSquareSize)/ gridSquareSize;
        float zCoord = floatModulo(terrainZ, gridSquareSize) / gridSquareSize;
        // un grid ii din 2 triunghiuri, si stim inaltimile in fiecare varf
        // pentru gasire inaltime a punctului folosim Barycentric Interpolation
        float height;
        glm::vec3 p1;
        glm::vec3 p2;
        glm::vec3 p3;
        glm::vec2 pos = glm::vec2(xCoord, zCoord);

        if (xCoord <= 1 - zCoord)
        {
            p1 = glm::vec3(0, heights[gridX][gridZ], 0);
            p2 = glm::vec3(1, heights[gridX + 1][gridZ], 0);
            p3 = glm::vec3(0, heights[gridX][gridZ + 1], 1);
        }
        else  
        {
            p1 = glm::vec3(1, heights[gridX + 1][gridZ], 0);
            p2 = glm::vec3(1, heights[gridX + 1][gridZ + 1], 1);
            p3 = glm::vec3(0, heights[gridX][gridZ + 1], 1);
        }
        height = barryCentric(p1, p2, p3, pos);

        return height;
    }

    void Terrain::generate(std::vector<str::TextureInfo> texturesInfo, const char * heightMapPath, bool procedural)
    {
            int x, y, n;
            int force_channels = 4;
            unsigned char* image_data = stbi_load(heightMapPath, &x, &y, &n, force_channels);

            HeightsGenerator hGenerator;
            int vertexCount;

            if (procedural == true)
            {
                vertexCount = 128;
            }
            else
            {
                vertexCount = x;
            }
            int count = vertexCount * vertexCount;
            heightsLength = vertexCount;
            heights = (float **)malloc(sizeof(float *)  * vertexCount);
            if (NULL == heights)
            {
                fprintf(stderr, MEMORY_ALLOC_ERROR);
                return;
            }
            for (int i = 0; i < vertexCount; i++)
            {
                heights[i] = (float *)malloc(sizeof(float) * vertexCount);
                if (NULL == heights[i])
                {
                    fprintf(stderr, MEMORY_ALLOC_ERROR);
                    return;
                }
            };

            std::vector<gps::Vertex> vertices(count * 3);
            std::vector<GLuint> indices(6 * (vertexCount - 1)*(vertexCount - 1));
            
            int vertexPointer = 0;
            if (true == procedural)
            {
                for (int i = 0; i < vertexCount; i++) {
                    for (int j = 0; j < vertexCount; j++) {
                        vertices[vertexPointer].Position.x = (float)j / ((float)vertexCount - 1) * TERRAIN_SIZE;
                        heights[j][i] = getHeight(hGenerator, i, j);
                        vertices[vertexPointer].Position.y = heights[j][i];
                        vertices[vertexPointer].Position.z = (float)i / ((float)vertexCount - 1) * TERRAIN_SIZE;
                        vertices[vertexPointer].Normal = getNormal(hGenerator, i, j);
                        vertices[vertexPointer].TexCoords.x = (float)j / ((float)vertexCount - 1);
                        vertices[vertexPointer].TexCoords.y = (float)i / ((float)vertexCount - 1);
                        vertexPointer++;
                    }
                }
            }
            else
            {
                for (int i = 0; i<vertexCount; i++) {
                    for (int j = 0; j<vertexCount; j++) {
                        vertices[vertexPointer].Position.x = (float)j / ((float)vertexCount - 1) * TERRAIN_SIZE;
                        heights[j][i] = getHeight(image_data, x, y, i, j);
                        vertices[vertexPointer].Position.y = heights[j][i];
                        vertices[vertexPointer].Position.z = (float)i / ((float)vertexCount - 1) * TERRAIN_SIZE;
                        vertices[vertexPointer].Normal = getNormal(image_data, x, y, j, i);
                        vertices[vertexPointer].TexCoords.x = (float)j / ((float)vertexCount - 1);
                        vertices[vertexPointer].TexCoords.y = (float)i / ((float)vertexCount - 1);
                        vertexPointer++;
                    }
                }
            }
            int pointer = 0;
            for (int gz = 0; gz<vertexCount - 1; gz++) {
                for (int gx = 0; gx<vertexCount - 1; gx++) {
                    int topLeft = (gz*vertexCount) + gx;
                    int topRight = topLeft + 1;
                    int bottomLeft = ((gz + 1)*vertexCount) + gx;
                    int bottomRight = bottomLeft + 1;
                    indices[pointer++] = topLeft;
                    indices[pointer++] = bottomLeft;
                    indices[pointer++] = topRight;
                    indices[pointer++] = topRight;
                    indices[pointer++] = bottomLeft;
                    indices[pointer++] = bottomRight;
                }
            }

            this->setRenderObject(gps::Model3D(indices, vertices, texturesInfo));
    }

    float Terrain::getHeight(HeightsGenerator generator, int x, int y)
    {
        return generator.generateHeight(x, y);
    }

    float Terrain::getHeight(unsigned char * image_data, int xMax, int yMax, int x, int y)
    {
        if (x < 0 || x >= xMax || y < 0 || y >= yMax)
        {
            return 0;
        }
        unsigned char * row = image_data + x * 4 * xMax;
        float height = (float)*(row + (y << 2) + 1) *
            *(row + (y << 2) + 2) *
            *(row + (y << 2) + 3);
        height -= MAX_PIXEL_COLOR / 2.0f;
        height /= MAX_PIXEL_COLOR / 2.0f;
        height *= MAX_HEIGHT;

        return height + 25.0f; // 25 s-ar putea sa trebuiasca modificat daca schimb map-ul
    }

    glm::vec3 Terrain::getNormal(HeightsGenerator generator, int x, int y)
    {
        float heightL = getHeight(generator, x - 1, y);
        float heightR = getHeight(generator, x + 1, y);
        float heightD = getHeight(generator, x, y - 1);
        float heightU = getHeight(generator, x, y + 1);

        glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
        normal = glm::normalize(normal);

        return normal;
    }

    glm::vec3 Terrain::getNormal(unsigned char * image_data, int xMax, int yMax, int x, int y)
    {
        float heightL = getHeight(image_data, xMax, yMax, x - 1, y);
        float heightR = getHeight(image_data, xMax, yMax, x + 1, y);
        float heightD = getHeight(image_data, xMax, yMax, x, y - 1);
        float heightU = getHeight(image_data, xMax, yMax, x, y + 1);

        glm::vec3 normal = glm::vec3(heightL - heightR, 25.0f, heightD - heightU);
        normal = glm::normalize(normal);

        return normal;
    }
}