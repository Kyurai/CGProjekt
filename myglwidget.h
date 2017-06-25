#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include "modelloader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include <soil/SOIL.h>
#include "glslshader.h"
#include "camera.h"
#include "obj.h"


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public slots:
   void receiveBounces(int bounces);

private:


    int bounces = 2;

    float lastX = MyGLWidget::width() / 2.0f;
    float lastY = MyGLWidget::height() /2.0f;


    float deltaTime = 0;
    int lastFrame = 0;

    Camera camera;

    QElapsedTimer timer;


    GLSLShader* flatShader, *shader, *raytraceShader, *pathtraceShader;

    GLuint vaoID;
    GLuint vboVerticesID;
    GLuint vboIndicesID;

    //fullscreen quad vao and vbos
    GLuint quadVAOID;
    GLuint quadVBOID;
    GLuint quadIndicesID;

    //projection and modelview matrices
    glm::mat4  P = glm::mat4(1);
    glm::mat4  M = glm::mat4(1);
    glm::mat4  V = glm::mat4(1);
    glm::mat4 MV;

    //Objloader instance
    ObjLoader obj;
    vector<Mesh*> meshes;					//all meshes
    vector<Material*> materials; 			//all materials
    vector<unsigned short> indices;			//all mesh indices
    vector<Vertex> vertices; 				//all mesh vertices
    vector<GLuint> textures;				//all textures



    bool bRaytrace = false;
    bool bPathtrace = false;

    //camera transformation variables
    int state = 0, oldX=0, oldY=0;
    float rX=22, rY=116, dist = -120;

    //OBJ mesh filename to load
    const std::string mesh_filename = "/home/dustin/Documents/CG_Prakt/FinalProject/models/blocks.obj";

    //background color
    glm::vec4 bg = glm::vec4(0.5,0.5,1,1);

    //eye position
    glm::vec3 eyePos;

    //scene axially aligned bounding box
    BBox aabb;

    GLuint texVerticesID; //texture storing vertex positions
    GLuint texTrianglesID; //texture storing triangles list

    //light crosshair gizmo vetex array and buffer object IDs
    GLuint lightVAOID;
    GLuint lightVerticesVBO;
    glm::vec3 lightPosOS=glm::vec3(0, 2,0); //objectspace light position

    //spherical cooridate variables for light rotation
    float theta = 0.66f;
    float phi = -1.0f;
    float radius = 70;

    //FPS related variables
    int total_frames = 0;
    float fps = 0;
    float lastTime =0;

    //texture ID for array texture
    GLuint textureID;



protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


public:
   MyGLWidget( QWidget *parent) : QOpenGLWidget(parent)
   {


       setFocusPolicy(Qt::StrongFocus);
       //setMouseTracking(true);
   }

};

#endif // MYGLWIDGET_H
