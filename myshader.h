#ifndef MYSHADER_H
#define MYSHADER_H

#include <QOpenGLShaderProgram>
#include <map>

class myShader
{
private:
     std::map<std::string,QOpenGLShaderProgram*> *shaders;

     void addShader(std::string name, QOpenGLShaderProgram *shader);

public:
    myShader();
    ~myShader();

    void loadShaders();

    //Getter&Setters
    std::map<std::string, QOpenGLShaderProgram *> *getShaders() const;
    void setShaders(std::map<std::string, QOpenGLShaderProgram *> *value);
};

#endif // MYSHADER_H
