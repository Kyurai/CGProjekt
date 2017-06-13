#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <stack>
#include "camera.h"
#include <QVector3D>
#include <QTime>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include "shader.h"
#include "object.h"


class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:

    Camera camera;

    float lastX = MyGLWidget::width() / 2.0f;
    float lastY = MyGLWidget::height() /2.0f;


    float deltaTime = 0;
    int lastFrame = 0;

    QTime time;

    Shader *shaders;

    Object *earthObject;


    void render(Object *object, QMatrix4x4 m, QMatrix4x4 p, QMatrix4x4 v);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


public:
   MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
   {
       setFocusPolicy(Qt::StrongFocus);
       //setMouseTracking(true);
   }

};

#endif // MYGLWIDGET_H
