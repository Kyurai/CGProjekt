#include "myglwidget.h"

MyGLWidget::MyGLWidget()
{

}

MyGLWidget::MyGLWidget(QWidget* parent):
    QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer),
    ibo(QOpenGLBuffer::IndexBuffer)
{
    this->setVisible(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

void MyGLWidget::resizeGL(int width, int height){

    height = (height == 0) ? 1 : height;
    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);
}

void MyGLWidget::fillBuffers(){
    //QImage img = new QImage(":/textures/earth.jpg");

    ModelLoader model;
    bool res = model.loadObjectFromFile("/home/dustin/Documents/CG_Prakt/Praktikum3/models/sphere_high.obj");
    // Wenn erfolgreich, generiere VBO und Index-Array
    if (res) {
       if(model.hasTextureCoordinates()){
           //this->hasTexCoord = true;
           vboLength = model.lengthOfVBO(0,true,true);
           iboLength = model.lengthOfIndexArray();
           vboData = new GLfloat[vboLength];
           indexData = new GLuint[iboLength];
           model.genVBO(vboData,0,true,true);
           model.genIndexArray(indexData);
           }
           else{
           //this->hasTexCoord = false;
           vboLength = model.lengthOfVBO(0,true,false);
           iboLength = model.lengthOfIndexArray();
           vboData = new GLfloat[vboLength];
           indexData = new GLuint[iboLength];
           model.genVBO(vboData,0,true,false);
           model.genIndexArray(indexData);
           }
       }
       else {}

       vbo.create();
       vbo.bind();
       vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
       vbo.allocate(vboData, sizeof(GLfloat) * vboLength);
       vbo.release();

       ibo.create();
       ibo.bind();
       ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
       ibo.allocate(indexData, sizeof(GLuint) * iboLength);
       ibo.release();
}

void MyGLWidget::initializeGL(){
      this->createShaders(); //Initialisieren der Shader
      this->fillBuffers(); //Load models

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

    defaultRay = new QOpenGLShaderProgram();
    defaultRay->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/defaultRay.vert");
    defaultRay->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/defaultRay.frag");
    defaultRay->link();

    monteCarlo = new QOpenGLShaderProgram();
    monteCarlo->addShaderFromSourceFile(QOpenGLShader::Vertex,":shader/monteCarlo.vert");
    monteCarlo->addShaderFromSourceFile(QOpenGLShader::Fragment,":shader/monteCarlo.frag");
}

void MyGLWidget::render(/*Planet *planet,*/ QMatrix4x4 perspective, QMatrix4x4 model, QMatrix4x4 view){

    /*QMatrix3x3 n = model.normalMatrix();
    QVector4D lightPosition(1.0,1.0,1.0,1.0);
    QVector3D lightIntensity(1.0,1.0,1.0);
    QVector3D kd(1.0,1.0,1.0);
    QVector3D ka(0.1,0.1,0.1);
    QVector3D ks(1.0,1.0,1.0);
    float shininess = 32.0;

    planet->shaderProgram->bind();
    this->vbo.bind();
    this->ibo.bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte
    int attrVertices = 0;
    attrVertices = planet->shaderProgram->attributeLocation("vert"); // #version 130
    int attrTexCoords = 1;
    attrTexCoords = planet->shaderProgram->attributeLocation("texCoord"); // #version 130
    int attrNorCoords = 2;
    attrNorCoords = planet->shaderProgram->attributeLocation("normCoord"); //anpassen der Location !!!!

    // Aktiviere die Verwendung der Attribute-Arrays
    planet->shaderProgram->enableAttributeArray(attrVertices);
    if(hasTexCoord){
        planet->shaderProgram->enableAttributeArray(attrTexCoords);
    }
    planet->shaderProgram->enableAttributeArray(attrNorCoords);

    planet->_texture->bind(0);
    planet->shaderProgram->setUniformValue("texture", 0);

    if(planet->planetName == "sun"){
        planet->distortionTex->bind(1);
        planet->shaderProgram->setUniformValue("distortionTex",1);
    }

    //Uniforms
    int unifMatrix = 0;
    unifMatrix = planet->shaderProgram->uniformLocation("modelMatrix");
    planet->shaderProgram->setUniformValue(unifMatrix, model);
    int unifMatrix1 = 1;
    unifMatrix1 = planet->shaderProgram->uniformLocation("perspectiveMatrix");
    planet->shaderProgram->setUniformValue(unifMatrix1, perspective);
    int unifMatrix2 = 2;
    unifMatrix2 = planet->shaderProgram->uniformLocation("viewMatrix");
    planet->shaderProgram->setUniformValue(unifMatrix2, view);
    int unifTime = 3;
    unifTime = planet->shaderProgram->uniformLocation("time");
    planet->shaderProgram->setUniformValue(unifTime, this->time);
    int unifMatrix3 = 4;
    unifMatrix3 = planet->shaderProgram->uniformLocation("normalMatrix");
    planet->shaderProgram->setUniformValue(unifMatrix3, n);
    int unifLightP = 5;
    unifLightP = planet->shaderProgram->uniformLocation("lightPosition");
    planet->shaderProgram->setUniformValue(unifLightP, lightPosition);
    int unifLightI = 6;
    unifLightI = planet->shaderProgram->uniformLocation("lightIntensity");
    planet->shaderProgram->setUniformValue(unifLightI, lightIntensity);
    int unifKd = 7;
    unifKd = planet->shaderProgram->uniformLocation("kd");
    planet->shaderProgram->setUniformValue(unifKd, kd);
    int unifKa = 8;
    unifKa = planet->shaderProgram->uniformLocation("ka");
    planet->shaderProgram->setUniformValue(unifKa, ka);
    int unifKs = 9;
    unifKs = planet->shaderProgram->uniformLocation("ks");
    planet->shaderProgram->setUniformValue(unifKs, ks);
    int unifShine = 10;
    unifShine = planet->shaderProgram->uniformLocation("shininess");
   planet->shaderProgram->setUniformValue(unifShine, shininess);



    if(this->hasTexCoord){
        int offset = 0;
        int stride = 12 * sizeof(GLfloat);
        planet->shaderProgram->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        planet->shaderProgram->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        planet->shaderProgram->setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);
    }
    else{
        int offset = 0;
        int stride = 8 * sizeof(GLfloat);
        planet->shaderProgram->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
        offset += 4 * sizeof(GLfloat);
        planet->shaderProgram->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
    }

    glDrawElements(GL_TRIANGLES, this->iboLength, GL_UNSIGNED_INT, 0);

    if(hasTexCoord){
        planet->shaderProgram->disableAttributeArray(attrTexCoords);
    }
    planet->shaderProgram->disableAttributeArray(attrVertices);
    planet->shaderProgram->disableAttributeArray(attrNorCoords);
    planet->_texture->release();

    if(planet->planetName == "sun"){
        planet->distortionTex->release();
    }
    this->vbo.release();
    this->ibo.release();

    // Löse das Shader-Programm
    planet->shaderProgram->release();*/
}
