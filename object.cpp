#include "object.h"

Object::Object(float xPos, float yPos, float zPos, std::string modelPath, std::string textureName, QOpenGLShaderProgram* shader)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->modelPath = modelPath;

    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);




        bool res = model.loadObjectFromFile(this->modelPath);
            if (res) {
            if(model.hasTextureCoordinates()){
                hasTexCoords = true;
                vboLength = model.lengthOfVBO(0,true,true);
                iboLength = model.lengthOfIndexArray();
                vboData = new GLfloat[vboLength];
                indexData = new GLuint[iboLength];
                model.genVBO(vboData,0,true,true);
                model.genIndexArray(indexData);
                }
             else{
                hasTexCoords = false;
                vboLength = model.lengthOfVBO(0,true,false);
                iboLength = model.lengthOfIndexArray();
                vboData = new GLfloat[vboLength];
                indexData = new GLuint[iboLength];
                model.genVBO(vboData,0,true,false);
                model.genIndexArray(indexData);
                }
            }
            else {}

            vbo->create();
            vbo->bind();
            vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            vbo->allocate(vboData, sizeof(GLfloat) * vboLength);
            vbo->release();

            ibo->create();
            ibo->bind();
            ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
            ibo->allocate(indexData, sizeof(GLuint) * iboLength);
            ibo->release();

            glEnable(GL_TEXTURE_2D);

            QString texturePath = ":/textures/" + QString::fromStdString(textureName) + ".jpg";
            texture = new QOpenGLTexture(QImage(texturePath).mirrored());

            this->shader = shader;


}

QOpenGLTexture *Object::getTexture()
{
    return texture;
}

QOpenGLShaderProgram *Object::getShader()
{
    return shader;
}

QOpenGLBuffer *Object::getVBO()
{
    return vbo;
}

QOpenGLBuffer *Object::getIBO()
{
    return ibo;
}

bool Object::hasTextureCoords()
{
    return hasTexCoords;
}

unsigned int Object::getIBOLength()
{
    return iboLength;
}
