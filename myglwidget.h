#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <stack>
#include <QOpenGLTexture>
#include "modelloader.h"
#include <string>
#include <vector>
#include "myobject.h"
#include "myshader.h"

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    std::vector<MyObject*> myObjects;
    myShader *shaders;

public:
    MyGLWidget();
    MyGLWidget(QWidget *parent);

    void initializeGL();
    void loadObjects();
    void resizeGL(int width, int height);
    void paintGL();
    void createShaders();
    void render(MyObject *myObject, QMatrix4x4 perspective, QMatrix4x4 model, QMatrix4x4 view);
};

#endif // MYGLWIDGET_H
