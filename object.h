#ifndef OBJECT_H
#define OBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "modelloader.h"
#include "shader.h"
#include <string>
#include <QString>

class Object
{

private:
    ModelLoader model;
    std::string modelPath;

    float xPos, yPos, zPos;

    QOpenGLBuffer *vbo;
    QOpenGLBuffer *ibo;

    GLfloat *vboData;
    GLuint *indexData;

    unsigned int vboLength, iboLength;

    bool hasTexCoords;

    QOpenGLTexture *texture;
    std::string textureName;
    QOpenGLShaderProgram *shader;

public:

    Object(float xPos, float yPos, float zPos, std::string modelPath, std::string textureName, QOpenGLShaderProgram* shader);

    QOpenGLTexture* getTexture();
    QOpenGLShaderProgram* getShader();
    QOpenGLBuffer* getVBO();
    QOpenGLBuffer* getIBO();
    bool hasTextureCoords();
    unsigned int getIBOLength();


};


#endif // OBJECT_H
