#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>
#include <QMap>
#include <string>


class Shader
{
private:
    QMap<std::string, QOpenGLShaderProgram*> shaders;
    void addShader(std::string shaderName, QOpenGLShaderProgram *shader);
    QOpenGLShaderProgram* defaultShader;

public:
    Shader();
    ~Shader();

QOpenGLShaderProgram* getShader(std::string shaderName);




};

#endif // SHADER_H
