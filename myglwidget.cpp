#include "myglwidget.h"


//resize OpenGl-Window
void MyGLWidget::resizeGL(int width, int height)
{
    height = (height == 0) ? 1 : height;
    // Set viewport to cover the whole window
    glViewport(0, 0, width, height);
}


//Initialize OpenGL

void MyGLWidget::initializeGL(){



        QPoint p = this->mapFromGlobal(QCursor ::pos());
        lastX = p.x();
        lastY = p.y();

        shaders = new Shader();

        earthObject = new Object(0.0f, 0.0f, -10.0f, "C:/Users/lucas/Desktop/CGProjekt-master/models/sphere_high.obj", "earth", shaders->getShader("default"));
        time.start();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LEQUAL);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glClearDepth(1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);



}

void MyGLWidget::paintGL(){
        int currentFrame = time.elapsed();
        deltaTime = ((float)currentFrame - (float)lastFrame)/100.0f;
        lastFrame = currentFrame;
        std::cout << "deltaTime: " << deltaTime << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        std::stack<QMatrix4x4> matrixStack;
        QMatrix4x4 modelMatrix, perspectiveMatrix, viewMatrix;

        perspectiveMatrix.perspective(camera.Zoom, (float)MyGLWidget::width() / (float)MyGLWidget::height(), 0.1, 100.0);


        viewMatrix = camera.GetViewMatrix();

        modelMatrix.setToIdentity();
        modelMatrix.translate(0.0f,0.0f,-10.0f);

        render(earthObject,modelMatrix, perspectiveMatrix, viewMatrix);

        update();
}

void MyGLWidget::wheelEvent(QWheelEvent *event){

    camera.ProcessMouseScroll(event->angleDelta().y() / 8);



}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{


    float xOffset = (float)event->x() - lastX;
    float yOffset = lastY - (float)event->y();

    lastX = (float)event->x();
    lastY = (float)event->y();

    camera.ProcessMouseMovement(xOffset,yOffset);
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

     switch(event->key()){
     case Qt::Key_W:
     case Qt::Key_Up:
         camera.ProcessKeyboard(FORWARD,deltaTime);
         break;
     case Qt::Key_A:
     case Qt::Key_Left:
         camera.ProcessKeyboard(LEFT,deltaTime);
         break;
     case Qt::Key_D:
     case Qt::Key_Right:
         camera.ProcessKeyboard(RIGHT,deltaTime);
         break;
     case Qt::Key_S:
     case Qt::Key_Down:
         camera.ProcessKeyboard(BACKWARD,deltaTime);
         break;

     default:
         QOpenGLWidget::keyPressEvent(event);
     }
 }




void MyGLWidget::render(Object *object, QMatrix4x4 m, QMatrix4x4 p, QMatrix4x4 v)
{
        object->getShader()->bind();
        object->getVBO()->bind();
        object->getIBO()->bind();


        int attrVertices = 0;
        attrVertices = object->getShader()->attributeLocation("vert");
        int attrTexCoords = 1;
        attrTexCoords = object->getShader()->attributeLocation("texCoord");
        int attrNorCoords = 2;
        attrNorCoords = object->getShader()->attributeLocation("normCoord");

        object->getShader()->enableAttributeArray(attrVertices);
        if(object->hasTextureCoords()){
            object->getShader()->enableAttributeArray(attrTexCoords);
        }
        object->getShader()->enableAttributeArray(attrNorCoords);
        object->getTexture()->bind(0);
        object->getShader()->setUniformValue("texture", 0);



        int unifMatrix = 0;
        unifMatrix = object->getShader()->uniformLocation("modelMatrix");
        object->getShader()->setUniformValue(unifMatrix, m);
        int unifMatrix1 = 1;
        unifMatrix1 = object->getShader()->uniformLocation("perspectiveMatrix");
        object->getShader()->setUniformValue(unifMatrix1, p);
        int unifMatrix2 = 2;
        unifMatrix2 = object->getShader()->uniformLocation("viewMatrix");
        object->getShader()->setUniformValue(unifMatrix2, v);


        if(object->hasTextureCoords()){
            int offset = 0;
            int stride = 12 * sizeof(GLfloat);
            object->getShader()->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
            offset += 4 * sizeof(GLfloat);
            object->getShader()->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
            offset += 4 * sizeof(GLfloat);
            object->getShader()->setAttributeBuffer(attrTexCoords, GL_FLOAT, offset, 4, stride);
        }
        else{
            int offset = 0;
            int stride = 8 * sizeof(GLfloat);
            object->getShader()->setAttributeBuffer(attrVertices, GL_FLOAT, offset, 4, stride);
            offset += 4 * sizeof(GLfloat);
            object->getShader()->setAttributeBuffer(attrNorCoords, GL_FLOAT, offset, 4, stride);
        }

        glDrawElements(GL_TRIANGLES, object->getIBOLength(), GL_UNSIGNED_INT, 0);

        object->getShader()->disableAttributeArray(attrVertices);
        if(object->hasTextureCoords()){
            object->getShader()->disableAttributeArray(attrTexCoords);
        }
        object->getShader()->disableAttributeArray(attrNorCoords);
        object->getTexture()->release();
        object->getVBO()->release();
        object->getIBO()->release();

        object->getShader()->release();


}
