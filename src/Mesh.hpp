//
//  Mesh.hpp
//  Lab4
//
//  Created by CGIS on 27/10/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include "glm/glm.hpp"
#include "GLEW/glew.h"
#include <string>
#include <vector>
#include "Shader.hpp"
#include "structs.hpp"

namespace gps {
    


class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	void Draw(gps::Shader shader);

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

	// Initializes all the buffer objects/arrays
	void setupMesh();

};

}
#endif /* Mesh_hpp */
