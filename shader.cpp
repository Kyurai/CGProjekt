#include "shader.h"


void Shader::addShader(std::string shaderName, QOpenGLShaderProgram *shader)
{
    shaders.insert(shaderName,shader);
}

Shader::Shader()
{
    QOpenGLShaderProgram *default130Shader = new QOpenGLShaderProgram();

    if (default130Shader->addShaderFromSourceFile(QOpenGLShader::Vertex,":shaders/default130.vert") &&
        default130Shader->addShaderFromSourceFile(QOpenGLShader::Fragment,":shaders/default130.frag"))
        {
            this->addShader("default130",default130Shader);
            defaultShader = default130Shader;
        }
}

Shader::~Shader()
{
    for(QMap<std::string, QOpenGLShaderProgram*>::iterator itr = shaders.begin(); itr != shaders.end(); itr++)
    {
        delete itr.value();

    }

    defaultShader = nullptr;
}

QOpenGLShaderProgram *Shader::getShader(std::string shaderName)
{
    return this->shaders.value(shaderName,defaultShader);
}
