#include "myobject.h"

#define FOLDINGSTART {
QOpenGLShaderProgram *MyObject::getShader() const
{
    return shader;
}

void MyObject::setShader(QOpenGLShaderProgram *value)
{
    shader = value;
}

QOpenGLTexture *MyObject::getTex() const
{
    return tex;
}

void MyObject::setTex(QOpenGLTexture *value)
{
    tex = value;
}

std::string MyObject::getTextureName() const
{
    return textureName;
}

void MyObject::setTextureName(const std::string &value)
{
    textureName = value;
}

bool MyObject::getHasTexCoord() const
{
    return hasTexCoord;
}

void MyObject::setHasTexCoord(bool value)
{
    hasTexCoord = value;
}

unsigned int MyObject::getIboLength() const
{
    return iboLength;
}

void MyObject::setIboLength(unsigned int value)
{
    iboLength = value;
}

unsigned int MyObject::getVboLength() const
{
    return vboLength;
}

void MyObject::setVboLength(unsigned int value)
{
    vboLength = value;
}

GLuint *MyObject::getIndexData() const
{
    return indexData;
}

void MyObject::setIndexData(GLuint *value)
{
    indexData = value;
}

GLfloat *MyObject::getVboData() const
{
    return vboData;
}

void MyObject::setVboData(GLfloat *value)
{
    vboData = value;
}

double MyObject::getOpacity() const
{
    return opacity;
}

void MyObject::setOpacity(double value)
{
    opacity = value;
}

double MyObject::getRotation() const
{
    return rotation;
}

void MyObject::setRotation(double value)
{
    rotation = value;
}

double MyObject::getAngle() const
{
    return angle;
}

void MyObject::setAngle(double value)
{
    angle = value;
}

double MyObject::getZ_Coord() const
{
    return z_Coord;
}

void MyObject::setZ_Coord(double value)
{
    z_Coord = value;
}

double MyObject::getY_Coord() const
{
    return y_Coord;
}

void MyObject::setY_Coord(double value)
{
    y_Coord = value;
}

double MyObject::getX_Coord() const
{
    return x_Coord;
}

void MyObject::setX_Coord(double value)
{
    x_Coord = value;
}

std::string MyObject::getFilePath() const
{
    return filePath;
}

void MyObject::setFilePath(const std::string &value)
{
    filePath = value;
}

ModelLoader MyObject::getModel() const
{
    return model;
}

void MyObject::setModel(const ModelLoader &value)
{
    model = value;
}

QOpenGLBuffer *MyObject::getVbo() const
{
    return vbo;
}

void MyObject::setVbo(QOpenGLBuffer *value)
{
    vbo = value;
}

QOpenGLBuffer *MyObject::getIbo() const
{
    return ibo;
}

void MyObject::setIbo(QOpenGLBuffer *value)
{
    ibo = value;
}

#define FOLDINGEND }


MyObject::MyObject(){
    this->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

MyObject::MyObject(double X,double Y,double Z,double angle,double rotation,std::string Path)
{
    this->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    this->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    this->x_Coord = X;
    this->y_Coord = Y;
    this->z_Coord = Z;

    this->angle = angle;
    this->rotation = rotation;

    this->filePath = Path;
}

bool MyObject::loadAll(){
    bool isLoaded = this->loadObject();
    if(!isLoaded){
        return false;
    }
    else{
        this->loadShader();
        this->loadTexture();
        return true;
    }
}

bool MyObject::loadObject(){
    bool res = model.loadObjectFromFile(this->filePath);

    if (res) {
       if(model.hasTextureCoordinates()){
           this->hasTexCoord = true;
           vboLength = model.lengthOfVBO(0,true,true);
           iboLength = model.lengthOfIndexArray();
           vboData = new GLfloat[vboLength];
           indexData = new GLuint[iboLength];
           model.genVBO(vboData,0,true,true);
           model.genIndexArray(indexData);
           }
           else{
           this->hasTexCoord = false;
           vboLength = model.lengthOfVBO(0,true,false);
           iboLength = model.lengthOfIndexArray();
           vboData = new GLfloat[vboLength];
           indexData = new GLuint[iboLength];
           model.genVBO(vboData,0,true,false);
           model.genIndexArray(indexData);
           }
       }
    else{
        return false;
    }

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

    return true;
}

void MyObject::loadTexture(){
    QString _textureName = QString::fromStdString(this->textureName);
    this->tex = new QOpenGLTexture(QImage(":/textures/"+_textureName+".jpg").mirrored());
}

void MyObject::loadShader(){

}
