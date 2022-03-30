#ifndef MODEL_H
#define MODEL_H

#include"Object.h"
#include"ObjLoader.h"

namespace simulation {
    class Model :public Object {
    public:
        Model(char* path);

        void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    private:

        void setupMesh();

        ShaderProgram* m_shaderProgram;

        GLuint vaoHandle;
        GLuint vbo_vertexPos,ebo_element, vbo_colors;

        objl::Loader loader;

        // model data
        //std::vector < objl:: Mesh > meshes;
        //string directory;

       // void loadModel(string path);
       /* void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<objl::Material> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            string typeName);*/
    };
}

#endif