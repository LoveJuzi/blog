#include "utils.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QDateTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

unsigned int TextureFromFile(QOpenGLFunctions_3_3_Core* glCore,
                             const char* path, const std::string& directory)
{
    std::string filename = path;
    if (!directory.empty()) filename = directory + '/' + filename;

    unsigned int textureID = 0;
    glCore->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    defer(stbi_image_free(data));
    if (data)
    {
        GLenum format;
        if(nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glCore->glBindTexture(GL_TEXTURE_2D, textureID);
        glCore->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glCore->glGenerateMipmap(GL_TEXTURE_2D);

        glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glCore->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
    }

    return textureID;
}

Shader::Shader(QOpenGLFunctions_3_3_Core* glCore)
{
    _glCore = glCore;
    if (_glCore == NULL) return;
    _ID = _glCore->glCreateProgram();
}

Shader::~Shader()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteProgram(_ID);
}

bool Shader::init(const char* vertexPath, const char* fragmentPath)
{
    if (_glCore == NULL) return false;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        defer(vShaderFile.close());
        defer(fShaderFile.close());

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ: "
                  << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    vertex = _glCore->glCreateShader(GL_VERTEX_SHADER);
    fragment = _glCore->glCreateShader(GL_FRAGMENT_SHADER);
    defer(_glCore->glDeleteShader(vertex));
    defer(_glCore->glDeleteShader(fragment));

    _glCore->glShaderSource(vertex, 1, &vShaderCode, NULL);
    _glCore->glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    _glCore->glShaderSource(fragment, 1, &fShaderCode, NULL);
    _glCore->glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    _glCore->glAttachShader(_ID, vertex);
    _glCore->glAttachShader(_ID, fragment);
    _glCore->glLinkProgram(_ID);
    checkCompileErrors(_ID, "PROGRAM");

    return true;
}

void Shader::use()
{
    _glCore->glUseProgram(_ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    _glCore->glUniform1i(_glCore->glGetUniformLocation(_ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    _glCore->glUniform1i(_glCore->glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    _glCore->glUniform1f(_glCore->glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    _glCore->glUniform3fv(_glCore->glGetUniformLocation(_ID, name.c_str()),
                          1, &value[0]);
}

void Shader::setMat4(const std::string& name, GLsizei count,
                                 GLboolean transpose, const glm::mat4& value) const
{
    _glCore->glUniformMatrix4fv(_glCore->glGetUniformLocation(_ID, name.c_str()),
                                count, transpose, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success = 0;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        _glCore->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            _glCore->glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
                      << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else
    {
        _glCore->glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            _glCore->glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: "
                      << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}

const GLfloat Camera::YAW         = 180.0f;
const GLfloat Camera::PITCH       = 0.0f;
const GLfloat Camera::SPEED       = 2.0f;
const GLfloat Camera::SENSITIVITY = 0.05f;
const GLfloat Camera::ZOOM        = 45.0f;
Camera::Camera(const glm::vec3& position, const glm::vec3& worldUp, GLfloat yaw, GLfloat pitch) :
    _position(position),
    _worldUp(worldUp),
    _yaw(yaw),
    _pitch(pitch)
{
    _front = glm::vec3(0.0f, 0.0f, -1.0f);
    _movementSpeed = Camera::SPEED;
    _mouseSensitivity = Camera::SENSITIVITY;
    _zoom = Camera::ZOOM;
    updateCameraVectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::processKeyboard(Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = _movementSpeed;
    if (deltaTime < 1.0f && deltaTime > 0.0f)
    {
        velocity *= deltaTime;
    }
    switch (direction)
    {
    case Movement::FORWARD:
        _position += _front * velocity;
        break;
    case Movement::BACKWARD:
        _position -= _front * velocity;
        break;
    case Movement::LEFT:
        _position -= _right * velocity;
        break;
    case Movement::RIGHT:
        _position += _right * velocity;
        break;
    default:
        break;
    }
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw   += xoffset;
    _pitch += yoffset;

    if (constrainPitch)
    {
        if (_pitch > 89.0f)  _pitch = 89.0f;
        if (_pitch < -89.0f) _pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yoffset)
{
    _zoom -= yoffset;
    if (_zoom < 1.0f)  _zoom = 1.0f;
    if (_zoom > 45.0f) _zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 front;
    front.x = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up    = glm::normalize(glm::cross(_right, _front));
}

Mesh::Mesh(QOpenGLFunctions_3_3_Core* glCore,
           const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<Texture>& textures)
{
    _vertices = vertices;
    _indices  = indices;
    _textures = textures;

    _glCore = glCore;

    setupMesh();
}

Mesh::~Mesh()
{
    if (_glCore == NULL) return;
    _glCore->glDeleteBuffers(1, &_EBO);
    _glCore->glDeleteBuffers(1, &_VBO);
    _glCore->glDeleteVertexArrays(1, &_VAO);
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i=0; i<_textures.size(); ++i)
    {
        std::string number;
        std::string name = _textures[i].type;
        if (name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if (name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }
        else if (name == "texture_height")
        {
            number = std::to_string(heightNr++);
        }

        _glCore->glActiveTexture(GL_TEXTURE0 + i);
        _glCore->glBindTexture(GL_TEXTURE_2D, _textures[i].id);

        shader.setInt("material." + name + number, i);
    }
    _glCore->glActiveTexture(GL_TEXTURE0);

    // draw mesh
    _glCore->glBindVertexArray(_VAO);
    _glCore->glDrawElements(
                GL_TRIANGLES,
                _indices.size(),
                GL_UNSIGNED_INT,
                0);
    _glCore->glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    if (_glCore == NULL) return;
    _glCore->glGenVertexArrays(1, &_VAO);
    _glCore->glGenBuffers(1, &_VBO);
    _glCore->glGenBuffers(1, &_EBO);

    _glCore->glBindVertexArray(_VAO);
    _glCore->glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _glCore->glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex),
                          &(_vertices[0]), GL_STATIC_DRAW);

    _glCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    _glCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int),
                          &(_indices[0]), GL_STATIC_DRAW);

    // vertex position
    _glCore->glEnableVertexAttribArray(0);
    _glCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void*)0);

    // vertex normals
    _glCore->glEnableVertexAttribArray(1);
    _glCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void*)offsetof(Vertex, Normal));

    // vertex texture corrds
    _glCore->glEnableVertexAttribArray(2);
    _glCore->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                   (void*)offsetof(Vertex, TexCoords));

    //_glCore->glEnableVertexAttribArray(3);
    //_glCore->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                               (void*)offsetof(Vertex, Tangent));

    //_glCore->glEnableVertexAttribArray(4);
    //_glCore->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                               (void*)offsetof(Vertex, Bitangent));

    //_glCore->glEnableVertexAttribArray(5);
    //_glCore->glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex),
    //                                (void*)offsetof(Vertex, BoneIDs));

    //_glCore->glEnableVertexAttribArray(6);
    //_glCore->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    //                               (void*)offsetof(Vertex, Weights));

    _glCore->glBindVertexArray(0);
}

Model::Model(QOpenGLFunctions_3_3_Core* glCore, const char* path)
{
    _glCore = glCore;
    loadModel(path);
}

Model::~Model()
{
    _SAFE_CLEAR_STL_PTR_(_meshes);
}

void Model::Draw(Shader& shader)
{
    for (unsigned int i=0; i<_meshes.size(); ++i)
    {
        _meshes[i]->Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    if (_glCore == NULL) return;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
                path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    _directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i=0; i<node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i=0; i<node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // 处理顶点
    for (unsigned int i=0; i<mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.Position = position;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 texcoords;
            texcoords.x = mesh->mTextureCoords[0][i].x;
            texcoords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texcoords;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // 处理索引
    for (unsigned int i=0; i<mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j=0; j<face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(
                    material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(
                    material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return new Mesh(_glCore, vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                 std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i=0; i<mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j=0; j< _texturesLoaded.size(); ++j)
        {
            if (std::strcmp(_texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(_texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (skip) continue;

        Texture texture;
        texture.id = TextureFromFile(_glCore, str.C_Str(), _directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        _texturesLoaded.push_back(texture);
    }
    return textures;
}
