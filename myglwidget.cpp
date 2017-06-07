#include "myglwidget.h"

MyGLWidget::MyGLWidget()
{

}

MyGLWidget::MyGLWidget(QWidget* parent):QOpenGLWidget(parent)
{
    this->setVisible(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

//resize OpenGl-Window
void MyGLWidget::resizeGL(int width, int height){
    height = (height == 0) ? 1 : height;
    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);
}

 //load each MyObject here
void MyGLWidget::loadObjects(){

    //resize Vector to fit number of Objects
    this->myObjects.resize(3);

    //Create Objects
    MyObject *sphere = new MyObject(0.0,0.0,0.0,0.0,0.0,"/home/dustin/Documents/CG_Prakt/FinalProject/models/sphere_high.obj");
    //MyObject *cube = new MyObject();
    //MyObject *cylinder = new MyObject();

    //add Object to myObjects-Vector
    myObjects.at(0) = sphere;
    //myObjects.at(1) = cube;
    //myObjects.at(2) = cylinder;

}

//Initialize OpenGL
void MyGLWidget::initializeGL(){
      this->createShaders(); //Initialisieren der Shader
      this->loadObjects();

      glEnable(GL_DEPTH_TEST);
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
      glDepthFunc(GL_LEQUAL);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
      glClearDepth(1.0f);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//Set bgColor black
}

void MyGLWidget::paintGL(){
    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    std::stack<QMatrix4x4> matrixStack;
    QMatrix4x4 perspectiveMatrix, modelMatrix, viewMatrix;
    modelMatrix.setToIdentity();
    perspectiveMatrix.setToIdentity();
    perspectiveMatrix.perspective(45.0, 16/9, 0.01, 100.0);

    viewMatrix.setToIdentity();
    viewMatrix.translate(0,0,-1);
    viewMatrix.rotate(0.0,1.0,0.0,0.0);
    viewMatrix.rotate(0.0,0.0,1.0,0.0);
    viewMatrix.rotate(0.0,0.0,1.0,0.0);
    viewMatrix.scale(0.0,0.0,0.0);

}

void MyGLWidget::createShaders(){

    /*default130 = new QOpenGLShaderProgram();
    default130->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/default130.vert");
    default130->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/default130.frag");
    default130->link();*/

    /*defaultRay = new QOpenGLShaderProgram();
    defaultRay->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/defaultRay.vert");
    defaultRay->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/defaultRay.frag");
    defaultRay->link();

    monteCarlo = new QOpenGLShaderProgram();
    monteCarlo->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/monteCarlo.vert");
    monteCarlo->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/monteCarlo.frag");*/
}

void MyGLWidget::render(MyObject *myObject, QMatrix4x4 perspective, QMatrix4x4 model, QMatrix4x4 view){

    QMatrix3x3 n = model.normalMatrix();
    QVector4D lightPosition(1.0,1.0,1.0,1.0);
    QVector3D lightIntensity(1.0,1.0,1.0);
    QVector3D kd(1.0,1.0,1.0);
    QVector3D ka(0.1,0.1,0.1);
    QVector3D ks(1.0,1.0,1.0);
    float shininess = 32.0;

    myObject->getShader()->bind();
    myObject->getVbo()->bind();
    myObject->getIbo()->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
    int attrVertices = 0;
    attrVertices = myObject->getShader()->attributeLocation("vert"); // #version 130
    int attrTexCoords = 1;
    attrTexCoords = myObject->getShader()->attributeLocation("texCoord"); // #version 130
    int attrNorCoords = 2;
    attrNorCoords = myObject->getShader()->attributeLocation("normCoord"); //anpassen der Location !!!!

    // Aktiviere die Verwendung der Attribute-Arrays
    myObject->getShader()->enableAttributeArray(attrVertices);
    if(myObject->getHasTexCoord()){
        myObject->getShader()->enableAttributeArray(attrTexCoords);
    }
    myObject->getShader()->enableAttributeArray(attrNorCoords);

    myObject->getTex()->bind(0);
    myObject->getShader()->setUniformValue("texture", 0);

    //Uniforms
    int unifMatrix = 0;
    unifMatrix = myObject->getShader()->uniformLocation("modelMatrix");
    myObject->getShader()->setUniformValue(unifMatrix, model);
    int unifMatrix1 = 1;
    unifMatrix1 = myObject->getShader()->uniformLocation("perspectiveMatrix");
    myObject->getShader()->setUniformValue(unifMatrix1, perspective);
    int unifMatrix2 = 2;
    unifMatrix2 = myObject->getShader()->uniformLocation("viewMatrix");
    myObject->getShader()->setUniformValue(unifMatrix2, view);
    //int unifTime = 3;
    //unifTime = myObject->getShader()->uniformLocation("time");
    //myObject->getShader()->setUniformValue(unifTime, this->time);
    int unifMatrix3 = 4;
    unifMatrix3 = myObject->getShader()->uniformLocation("normalMatrix");
    myObject->getShader()->setUniformValue(unifMatrix3, n);
    int unifLightP = 5;
    unifLightP = myObject->getShader()->uniformLocation("lightPosition");
    myObject->getShader()->setUniformValue(unifLightP, lightPosition);
    int unifLightI = 6;
    unifLightI = myObject->getShader()->uniformLocation("lightIntensity");
    myObject->getShader()->setUniformValue(unifLightI, lightIntensity);
    int unifKd = 7;
    unifKd = myObject->getShader()->uniformLocation("kd");
    myObject->getShader()->setUniformValue(unifKd, kd);
    int unifKa = 8;
    unifKa = myObject->getShader()->uniformLocation("ka");
    myObject->getShader()->setUniformValue(unifKa, ka);
    int unifKs = 9;
    unifKs = myObject->getShader()->uniformLocation("ks");
    myObject->getShader()->setUniformValue(unifKs, ks);
    int unifShine = 10;
    unifShine = myObject->getShader()->uniformLocation("shininess");
   myObject->getShader()->setUniformValue(unifShine, shininess);



    if(myObject->getHasTexCoord()){
        int offset = 0;
        int stride = 12 * sizeof(GLfloat);
        myObject->getShader()->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        myObject->getShader()->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        myObject->getShader()->setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);
    }
    else{
        int offset = 0;
        int stride = 8 * sizeof(GLfloat);
        myObject->getShader()->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        myObject->getShader()->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
    }

    glDrawElements(GL_TRIANGLES, myObject->getIboLength(), GL_UNSIGNED_INT, 0);

    if(myObject->getHasTexCoord()){
        myObject->getShader()->disableAttributeArray(attrTexCoords);
    }
    myObject->getShader()->disableAttributeArray(attrVertices);
    myObject->getShader()->disableAttributeArray(attrNorCoords);
    myObject->getTex()->release();

    myObject->getVbo()->release();
    myObject->getIbo()->release();

    // Löse das Shader-Programm
    myObject->getShader()->release();
}
