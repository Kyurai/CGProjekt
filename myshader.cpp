#include "myshader.h"


myShader::myShader()
{
    loadShaders();
}

myShader::~myShader(){
    delete this->shaders;
}

//Checks if shader can be loaded and adds it to map
std::map<std::string, QOpenGLShaderProgram *> *myShader::getShaders() const
{
    return shaders;
}

void myShader::setShaders(std::map<std::string, QOpenGLShaderProgram *> *value)
{
    shaders = value;
}

//adds Shader to map
void myShader::loadShaders(){
    QOpenGLShaderProgram *default130Shader = new QOpenGLShaderProgram();
    if(
    default130Shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/default130.vert") &&
    default130Shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/default130.frag")
    ){
        this->addShader("default130",default130Shader);
    }

    /*QOpenGLShaderProgram *defaultRayShader = new QOpenGLShaderProgram();
    if(
    defaultRayShader->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/defaultRay.vert") &&
    defaultRayShader->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/defaultRay.frag")
    ){
        this->addShader("defaultRay",defaultRayShader);
    }

    QOpenGLShaderProgram *monteCarloShader = new QOpenGLShaderProgram();
    if(
    monteCarloShader->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/monteCarlo.vert") &&
    monteCarloShader->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/monteCarlo.frag")
    ){
        this->addShader("monteCarlo",monteCarloShader);
    }*/
}

/**
 * @brief myShader::addShader
 * @param name -> key in Map
 * @param shader -> shaderprogram that key is mapped to
 */
void myShader::addShader(std::string name, QOpenGLShaderProgram* shader){
    this->shaders->at(name) = shader;
}



