#include "glslshader.h"
#include <iostream>


GLSLShader::GLSLShader(void)
{

    _totalShaders=0;
    _shaders[VERTEX_SHADER]=0;
    _shaders[FRAGMENT_SHADER]=0;
    _shaders[GEOMETRY_SHADER]=0;
    _attributeList.clear();
    _uniformLocationList.clear();
}

GLSLShader::~GLSLShader(void)
{
    _attributeList.clear();
    _uniformLocationList.clear();
}

void GLSLShader::DeleteShaderProgram() {
    glFuncs->glDeleteProgram(_program);
}

void GLSLShader::LoadFromString(GLenum type, const std::string& source) {
    GLuint shader = glFuncs->glCreateShader(type);

    const char * ptmp = source.c_str();
    glFuncs->glShaderSource (shader, 1, &ptmp, NULL);

    //check whether the shader loads fine
    GLint status;
    glFuncs->glCompileShader (shader);
    glFuncs->glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glFuncs->glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glFuncs->glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
        std::cerr<<"Compile log: "<<infoLog<<std::endl;
        delete [] infoLog;
    }
    _shaders[_totalShaders++]=shader;
}


void GLSLShader::CreateAndLinkProgram() {
    _program = glFuncs->glCreateProgram ();
    if (_shaders[VERTEX_SHADER] != 0) {
        glFuncs->glAttachShader (_program, _shaders[VERTEX_SHADER]);
    }
    if (_shaders[FRAGMENT_SHADER] != 0) {
        glFuncs->glAttachShader (_program, _shaders[FRAGMENT_SHADER]);
    }
    if (_shaders[GEOMETRY_SHADER] != 0) {
        glFuncs->glAttachShader (_program, _shaders[GEOMETRY_SHADER]);
    }

    //link and check whether the program links fine
    GLint status;
    glFuncs->glLinkProgram (_program);
    glFuncs->glGetProgramiv (_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;

        glFuncs->glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glFuncs->glGetProgramInfoLog (_program, infoLogLength, NULL, infoLog);
        std::cerr<<"Link log: "<<infoLog<<std::endl;
        delete [] infoLog;
    }

    glFuncs->glDeleteShader(_shaders[VERTEX_SHADER]);
    glFuncs->glDeleteShader(_shaders[FRAGMENT_SHADER]);
    glFuncs->glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void GLSLShader::Use() {
    glFuncs->glUseProgram(_program);
}

void GLSLShader::UnUse() {
    glFuncs->glUseProgram(0);
}

void GLSLShader::AddAttribute(const std::string& attribute) {
    _attributeList[attribute]= glFuncs->glGetAttribLocation(_program, attribute.c_str());
}

//An indexer that returns the location of the attribute
GLuint GLSLShader::operator [](const std::string& attribute) {
    return _attributeList[attribute];
}

void GLSLShader::AddUniform(const std::string& uniform) {
    _uniformLocationList[uniform] = glFuncs->glGetUniformLocation(_program, uniform.c_str());
}

GLuint GLSLShader::operator()(const std::string& uniform){
    return _uniformLocationList[uniform];
}

GLuint GLSLShader::getUniform(const std::string &uniform)
{
    return _uniformLocationList[uniform];
}

GLuint GLSLShader::getAttribute(const std::string &attribute)
{
    return _attributeList[attribute];
}

#include <fstream>
void GLSLShader::LoadFromFile(GLenum whichShader, const std::string& filename){
    std::ifstream fp;
    fp.open(filename.c_str(), std::ios_base::in);
    if(fp) {
        std::string line, buffer;
        while(std::getline(fp, line)) {
            buffer.append(line);
            buffer.append("\r\n");
        }
        //copy to source
        LoadFromString(whichShader, buffer);
    } else {
        std::cerr<<"Error loading shader: "<<filename<<std::endl;
    }
}
