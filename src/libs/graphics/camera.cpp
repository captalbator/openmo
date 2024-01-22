#include "camera.hpp"

namespace graphics
{

Camera::Camera(const glm::vec3 &position, float fov, float aspect, float nearClip, float farClip)
{
    _position = position;
    _aspect = aspect;

    _near = nearClip;
    _far = farClip;

    setFOV(fov);

    update();
}

Camera::~Camera()
{
}

void Camera::update()
{
    glm::vec3 look;
    look.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    look.y = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    look.z = sin(glm::radians(_pitch));
    _lookVector = look;

    _rightVector = -glm::normalize(glm::cross(kWorldUp, _lookVector));
    _upVector = glm::normalize(glm::cross(_rightVector, _lookVector));

    _viewMatrix = glm::lookAt(_position, _position + _lookVector, _upVector);

    updateFrustum();
}

void Camera::updateProjectionMatrix()
{
    _projectionMatrix = glm::perspective(glm::radians(_fieldOfView), _aspect, _near, _far);
    updateFrustum();
}

void Camera::setFOV(float newFov)
{
    _fieldOfView = newFov;
    float vertical_fov = 2.0f * atan(tan(glm::radians(_fieldOfView) * 0.5f) * _aspect);
    _verticalFieldOfView = vertical_fov;
    updateProjectionMatrix();
}

void Camera::setViewportSize(double w, double h)
{
    _aspect = w / h;
    updateProjectionMatrix();
}

void Camera::processMouseMovement(int dx, int dy, bool isHeld)
{
    if (isHeld)
    {
        _yaw -= dx * _sensitivity;
        _pitch -= dy * _sensitivity;

        if (_pitch > 89.9f)
            _pitch = 89.9f;

        if (_pitch < -89.9f)
            _pitch = -89.9f;
    }

    update();
}

void Camera::setSpeed(float newSpeed)
{
    _speed = newSpeed;
    if (_speed < 1.0f)
        _speed = 1.0f;
}

void Camera::changeSpeed(float delta)
{
    setSpeed(_speed + (_speed * 0.1f) * delta);
}

void Camera::processMovement(float delta)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W])
    {
        _position += _lookVector * _speed * delta;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        _position -= _lookVector * _speed * delta;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        _position -= glm::normalize(glm::cross(_lookVector, _upVector)) * _speed * delta;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        _position += glm::normalize(glm::cross(_lookVector, _upVector)) * _speed * delta;
    }
}

void Camera::updateFrustum()
{
    Frustum frustum;

    const float halfVSide = _far * tanf(_verticalFieldOfView * 0.5f);
    const float halfHSide = halfVSide * _aspect;
    const glm::vec3 frontMultFar = _far * _lookVector;

    frustum.nearFace = {_position + _near * _lookVector, _lookVector};
    frustum.farFace = {_position + frontMultFar, -_lookVector};
    frustum.rightFace = {_position, glm::cross(frontMultFar + _rightVector * halfHSide, _upVector)};
    frustum.leftFace = {_position, glm::cross(_upVector, frontMultFar - _rightVector * halfHSide)};
    frustum.topFace = {_position, glm::cross(-_rightVector, frontMultFar - _upVector * halfVSide)};
    frustum.bottomFace = {_position,
                          glm::cross(frontMultFar + _upVector * halfVSide, -_rightVector)};

    _frustum = frustum;
}

} // namespace graphics
