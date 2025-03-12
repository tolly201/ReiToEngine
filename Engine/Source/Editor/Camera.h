#ifndef CAMERA_H
#define CAMERA_H
#include "Core/Math/IMath.h"

//left-handed cartesian coordinates
//default z(0,0,1) points at outside screen
namespace ReiToEngine
{
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         =  180.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    Vec3d Position;
    Vec3d Front;
    Vec3d Up;
    Vec3d Right;
    Vec3d WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(Vec3d position = Vec3d({0.0f, 0.0f, 0.0f}),
    Vec3d up = Vec3d({0.0f, 1.0f, 0.0f}), float yaw = YAW, float pitch = PITCH) :
    Front(Vec3d({0.0f, 0.0f, 1.0f})), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    Matrix4x4d GetViewMatrix()
    {
        Matrix4x4d transform =
        {
            1.0, 0.0, 0.0, -Position.x,
            0.0, 1.0, 0.0, -Position.y,
            0.0, 0.0, 1.0, -Position.z,
            0.0, 0.0, 0.0, 1.0
        };

        Matrix4x4d rotation =
        {
            Right.x, Right.y, Right.z, 0.0,
            Up.x, Up.y, Up.z, 0.0,
            Front.x, Front.y, Front.z, 0.0,
            0.0, 0.0, 0.0, 1.0
        };


        // 正确构建视图矩阵
        Matrix4x4d viewMatrix = {
            Right.x, Right.y, Right.z, -(Position*Right),
            Up.x, Up.y, Up.z, -(Position*Up),
            Front.x, Front.y, Front.z, -(Position*Front),
            0.0, 0.0, 0.0, 1.0
        };


        std::cout << "viewMatrix" << viewMatrix;
        std::cout << "counting" << rotation* transform;
        // return viewMatrix;
        return rotation * transform;
    }

    Matrix4x4d GetProjectionMatrix()
    {
    // 投影参数
    float fovY = 60.0f; // 垂直视场角（Field of View）
    float aspect = 4.0f / 3.0f; // 宽高比（Width / Height）
    float zNear = 0.1f; // 近裁剪面
    float zFar = 100.0f; // 远裁剪面

    // 计算透视投影矩阵
    float tanHalfFovY = tan(fovY / 2.0f * M_PI / 180.0f);
    printf("tan:%f, %f\n", tanHalfFovY, sqrt(3)/3);
    double ylen = 2 * tanHalfFovY * (zFar - zNear);
    double xlen = ylen * aspect;

    Matrix4x4d projectionMatrix = {
        1.0 / (tanHalfFovY), 0.0, 0.0, 0.0,
        0.0, 1.0 / tanHalfFovY, 0.0, 0.0,
        0.0, 0.0, -(zFar + zNear)/(zFar - zNear), -2.0f * zFar * zNear,
        0.0, 0.0, 1.0, 0.0
    };
        return projectionMatrix;
    }

    Matrix4x4d GetNDCMatrix(double width, double height)
    {
    // 投影参数
    float fovY = 60.0f; // 垂直视场角（Field of View）
    float aspect = 4.0f / 3.0f; // 宽高比（Width / Height）
    float zNear = 0.1f; // 近裁剪面
    float zFar = 100.0f; // 远裁剪面
    // 计算透视投影矩阵
    float tanHalfFovY = tan(fovY / 2.0f * M_PI / 180.0f);
    double ylen = 2 * tanHalfFovY * zFar;
    double xlen = ylen * aspect;

        Matrix4x4d projectionMatrix = {
            2.0 / xlen, 0.0, 0.0, 0.0,
            0.0, 2.0 / ylen, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        return projectionMatrix;
    }

    Matrix4x4d GetScreenMatrix(double width, double height)
    {
        // 屏幕空间转换矩阵
        Matrix4x4d screenMatrix = {
            0.5 * width, 0.0, 0.0, 0.5 * width,
            0.0, -0.5 * height, 0.0, 0.5 * height,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0};
        return screenMatrix;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
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
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        Vec3d front;
        std::cout << "Yaw:" << Yaw << std::endl;
        std::cout << "Pitch:" << Pitch << std::endl;
        front.x = sin(Yaw * M_PI / 180.0) * cos(Pitch * M_PI / 180.0);
        front.y = sin(Pitch * M_PI / 180.0);
        front.z = cos(Yaw * M_PI / 180.0) * cos(Pitch * M_PI / 180.0);
        std::cout << "front:" << front << std::endl;

        Front = front.normalize();
        std::cout << "Front:" << Front << std::endl;
        // also re-calculate the Right and Up vector
        Right = WorldUp.cross3D(Front).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = Front.cross3D(Right).normalize();

        std::cout << "Right:" << Right << std::endl;
        std::cout << "Up:" << Up << std::endl;
    }
};
}
#endif

