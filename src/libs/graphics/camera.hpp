#pragma once

namespace graphics
{

const glm::vec3 kWorldUp = glm::vec3(0.0f, 0.0f, 1.0f);

struct BoundingSphere
{
    glm::vec3 center;
    float radius;
};

struct Plane
{
    Plane() {}

    Plane(const glm::vec3 &p1, const glm::vec3 &norm)
        : normal(glm::normalize(norm)), distance(glm::dot(normal, p1))
    {
    }

    float getSignedDistanceToPlane(const glm::vec3 &point) const
    {
        return glm::dot(normal, point) - distance;
    }

    glm::vec3 normal = {0.0f, 1.0f, 0.0f};
    float distance = 0.0f;
};

struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
};

class Camera
{
public:
    Camera(const glm::vec3 &position, float fov, float aspect, float near, float far);
    ~Camera();

    void setPosition(glm::vec3 &position) { _position = position; }

    void setPitch(float pitch) { _pitch = pitch; }
    void setYaw(float yaw) { _yaw = yaw; }

    void updateProjectionMatrix();

    void setFOV(float newFOV);
    void setViewportSize(double w, double h);

    void setSpeed(float newSpeed);
    void changeSpeed(float delta);

    void setNear(float newNear)
    {
        _near = newNear;
        this->updateProjectionMatrix();
    }

    void setFar(float newFar)
    {
        _far = newFar;
        this->updateProjectionMatrix();
    }

    void processMouseMovement(int dx, int dy, bool held);
    void processMovement(float delta);

    void updateFrustum();
    void update();
    void drawInfo();

    bool isInFrustum(glm::vec3 &point) const
    {
        return (_frustum.leftFace.getSignedDistanceToPlane(point) >= 0.0f) &&
               (_frustum.rightFace.getSignedDistanceToPlane(point) >= 0.0f) &&
               (_frustum.farFace.getSignedDistanceToPlane(point) >= 0.0f) &&
               (_frustum.nearFace.getSignedDistanceToPlane(point) >= 0.0f) &&
               (_frustum.topFace.getSignedDistanceToPlane(point) >= 0.0f) &&
               (_frustum.bottomFace.getSignedDistanceToPlane(point) >= 0.0f);
    }

    bool isInFrustum(BoundingSphere &sphere) const
    {
        if (isInFrustum(sphere.center))
        {
            return true;
        }

        return (_frustum.leftFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius) &&
               (_frustum.rightFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius) &&
               (_frustum.farFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius) &&
               (_frustum.nearFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius) &&
               (_frustum.topFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius) &&
               (_frustum.bottomFace.getSignedDistanceToPlane(sphere.center) > -sphere.radius);
    }

    const glm::vec3 &getPosition() const { return _position; }
    const glm::vec3 &getLookVector() const { return _lookVector; }
    const glm::vec3 &getUpVector() const { return _upVector; }
    const glm::vec3 &getRightVector() const { return _rightVector; }
    const float &getPitch() const { return _pitch; }
    const float &getYaw() const { return _yaw; }

    const float &getFOV() const { return _fieldOfView; }

    const float &getSpeed() const { return _speed; }

    const Frustum &getCameraFrustum() const { return _frustum; }

    const glm::mat4 &getViewMatrix() const { return _viewMatrix; }
    const glm::mat4 &getProjectionMatrix() const { return _projectionMatrix; }

private:
    glm::vec3 _position = {0.0f, 0.0f, 0.0f};

    glm::vec3 _lookVector{0.0f, -1.0f, 0.0f};
    glm::vec3 _upVector{0.0f, 0.0f, 1.0f};
    glm::vec3 _rightVector;

    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

    Frustum _frustum;

    float _speed{100.0f};
    float _sensitivity{0.25f};

    float _fieldOfView{90.0f};
    float _verticalFieldOfView{0.0f};

    float _aspect;

    float _near{0.01f};
    float _far{100.0f};

    float _pitch{0.0f};
    float _yaw{-90.0f};
};

} // namespace graphics
