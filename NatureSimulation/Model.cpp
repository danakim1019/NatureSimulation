#include"Model.h"

namespace simulation {

    Model::Model(char* path) :Object(path)
    {
        //loadModel(path);
        loader.LoadFile(path);
        //setup buffer
        setupMesh();
    }

    //setup buffer
    void Model::setupMesh() {

        //shader object
        m_shaderProgram = new ShaderProgram();
        m_shaderProgram->initFromFiles("Phong.vert", "Phong.frag");

        //add attributes and uniform vars
        m_shaderProgram->addUniform("mvp");

        m_shaderProgram->addUniform("LightPosition");
        m_shaderProgram->addUniform("Ka");
        m_shaderProgram->addUniform("Kd");
        m_shaderProgram->addUniform("Ks");
        m_shaderProgram->addUniform("LightIntensity");

        m_shaderProgram->addUniform("modelMatrix");
        m_shaderProgram->addUniform("viewMatrix");
        m_shaderProgram->addUniform("projectionMatrix");
        m_shaderProgram->addUniform("NormalMatrix");

        // create buffers/arrays
        glGenVertexArrays(1, &vaoHandle);
        glBindVertexArray(vaoHandle);

        glGenBuffers(1, &vbo_vertexPos);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexPos);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, loader.LoadedVertices.size() * sizeof(objl::Vertex), &loader.LoadedVertices[0], GL_STATIC_DRAW);
        // set the vertex attribute pointers
       // vertex Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &ebo_element);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_element);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.LoadedIndices.size() * sizeof(unsigned int), &loader.LoadedIndices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)(sizeof(float) * 3));
        // vertex normals
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)(sizeof(float) * 6));
        // vertex texture coords
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void Model::draw() {
        for (unsigned int i = 0; i < loader.LoadedMeshes.size(); i++) {
            
            m_shaderProgram->use();

            // draw mesh
            glBindVertexArray(vaoHandle);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(loader.LoadedIndices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);

            m_shaderProgram->disable();
        }
    }
}