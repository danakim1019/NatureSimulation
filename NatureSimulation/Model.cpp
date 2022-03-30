#include"Model.h"

namespace simulation {

    Model::Model(char* path) :Object(path)
    {
        //loadModel(path);
        if (!loader.LoadFile(path))
        {
            std::cout << "\nerror : file load failed" << std::endl;
            return;
        }
        //setup buffer
        setupMesh();
    }

    //setup buffer
    void Model::setupMesh() {

        //shader object
        m_shaderProgram = new ShaderProgram();
        m_shaderProgram->initFromFiles("Phong.vert", "Phong.frag");

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

    void Model::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
        
        glm::vec4 lightPos(10, 10, 10, 1);
        glm::vec3 LightIntensity(1, 1, 1);
        glm::vec3 Ka(0.5f, 0.5f, 0.5f);
        glm::vec3 Kd(0.5f, 0.5f, 0.5f);
        glm::vec3 Ks(0.5f, 0.5f, 0.5f);

        glm::mat4 mview = view * model;
        glm::mat4 imvp = glm::inverse(mview);
        glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

        for (unsigned int i = 0; i < loader.LoadedMeshes.size(); i++) {
            
            m_shaderProgram->use();

            // draw mesh
            glBindVertexArray(vaoHandle);

            glUniform4fv(m_shaderProgram->uniform("LightPosition"), 1, glm::value_ptr(view*lightPos));
            glUniform3fv(m_shaderProgram->uniform("Ka"), 1,  glm::value_ptr(Ka));
            glUniform3fv(m_shaderProgram->uniform("Kd"), 1, glm::value_ptr(Kd));
            glUniform3fv(m_shaderProgram->uniform("Ks"), 1,  glm::value_ptr(Ks));
            glUniform3fv(m_shaderProgram->uniform("LightIntensity"), 1,  glm::value_ptr(LightIntensity));

            glUniformMatrix4fv(m_shaderProgram->uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(m_shaderProgram->uniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(m_shaderProgram->uniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(m_shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));


            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(loader.LoadedIndices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);

            m_shaderProgram->disable();
        }
    }
}