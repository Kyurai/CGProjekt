#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  2.5f;
const float SENSITIVTY =  0.1f;
const float ZOOM       =  45.0f;



class Camera
{
public:

    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = QVector3D(posX, posY, posZ);
        WorldUp = QVector3D(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }


    QMatrix4x4 GetViewMatrix()
    {
        QMatrix4x4 tmp;
        tmp.lookAt(Position, Position + Front, Up);
        return tmp;
    }


    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;


    }


    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;


        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }


        updateCameraVectors();
    }


    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:

    void updateCameraVectors()
    {


        QVector3D front;
        front.setX(qCos(qDegreesToRadians(Yaw)) * qCos(qDegreesToRadians(Pitch)));
        front.setY(qSin(qDegreesToRadians(Pitch)));
        front.setZ(qSin(qDegreesToRadians(Yaw)) * qCos(qDegreesToRadians(Pitch)));
        Front = front.normalized();

        Right = (QVector3D::crossProduct(Front, WorldUp)).normalized();
        Up    = (QVector3D::crossProduct(Right, Front)).normalized();

    }



};
#endif
