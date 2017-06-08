#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "modelloader.h"
#include "myshader.h"


class MyObject
{
private:
    ModelLoader model;
    std::string filePath;

    double x_Coord;
    double y_Coord;
    double z_Coord;

    double angle;
    double rotation;
    double scale;

    double opacity;

    QOpenGLBuffer *vbo; //Vertex Buffer Object
    QOpenGLBuffer *ibo; //Indices Buffer Object

    GLfloat *vboData;
    GLuint *indexData; // GLuint statt GLubyte, da viele Modelle groß sind
    unsigned int vboLength;
    unsigned int iboLength;

    bool hasTexCoord;

    QOpenGLTexture *tex;
    std::string textureName;
    QOpenGLShaderProgram *shader;

public:
    MyObject();
    MyObject(double X, double Y, double Z, double angle, double rotation, double scale, std::string modelPath);
    ~MyObject();

    bool loadAll();
    bool loadObject(); //load model/object into vbo
    void loadTexture();

    //Getter&Setter
    QOpenGLTexture *getTex() const;
    void setTex(QOpenGLTexture *value);
    std::string getTextureName() const;
    void setTextureName(const std::string &value);
    bool getHasTexCoord() const;
    void setHasTexCoord(bool value);
    unsigned int getIboLength() const;
    void setIboLength(unsigned int value);
    unsigned int getVboLength() const;
    void setVboLength(unsigned int value);
    GLuint *getIndexData() const;
    void setIndexData(GLuint *value);
    GLfloat *getVboData() const;
    void setVboData(GLfloat *value);
    double getOpacity() const;
    void setOpacity(double value);
    double getRotation() const;
    void setRotation(double value);
    double getAngle() const;
    void setAngle(double value);
    double getZ_Coord() const;
    void setZ_Coord(double value);
    double getY_Coord() const;
    void setY_Coord(double value);
    double getX_Coord() const;
    void setX_Coord(double value);
    std::string getFilePath() const;
    void setFilePath(const std::string &value);
    ModelLoader getModel() const;
    void setModel(const ModelLoader &value);
    QOpenGLBuffer *getVbo() const;
    void setVbo(QOpenGLBuffer *value);
    QOpenGLBuffer *getIbo() const;
    void setIbo(QOpenGLBuffer *value);
    double getScale() const;
    void setScale(double value);
    QOpenGLShaderProgram *getShader() const;
    void setShader(QOpenGLShaderProgram *value);
};

#endif // MYOBJECT_H
