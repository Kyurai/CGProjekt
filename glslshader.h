#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <map>
#include <string>
#include <qopengl.h>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>

class GLSLShader
{
public:
    GLSLShader(void);
    ~GLSLShader(void);
    void LoadFromString(GLenum whichShader, const std::string& source);
    void LoadFromFile(GLenum whichShader, const std::string& filename);
    void CreateAndLinkProgram();
    void Use();
    void UnUse();
    void AddAttribute(const std::string& attribute);
    void AddUniform(const std::string& uniform);

    //An indexer that returns the location of the attribute/uniform
    GLuint operator[](const std::string& attribute);
    GLuint operator()(const std::string& uniform);
    void DeleteShaderProgram();
    GLuint getAttribute(const std::string& attribute);
    GLuint getUniform(const std::string& uniform);
private:
    enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
    GLuint	_program;
    int _totalShaders;
    GLuint _shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
    std::map<std::string,GLuint> _attributeList;
    std::map<std::string,GLuint> _uniformLocationList;

    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();

};


#endif // GLSLSHADER_H
