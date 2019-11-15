#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum Camera_Movement{
    FORWARD, 
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera
#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define MOUSE_SENSITIVITY 0.1f

#define MIN_FOV 1.0f
#define FOV 75

#define REDNER_NEAR 0.1f
#define RENDER_DISTANCE 100.0f


/**
 * @brief Camera class
 * 
 */
class Camera {
    private:
        glm::vec3 position;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::vec3 cameraRight;
        glm::vec3 worldUp;

        glm::mat4 projectionMatrix;

        float yaw;
        float pitch;
        float perspectiveFov;

        float speed;
        float mouseSensitivity;

        GLFWwindow * camInViewPort;

    public:
        Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW);

        /**
         * @brief Updates all the vectors
         * 
         */
        void updateVectors();

        /**
         * @brief Get the View Matrix object
         * 
         * @return glm::mat4 veiw matrix
         */
        glm::mat4 getViewMatrix();

        /**
         * @brief Get the Projection Matrix object
         * 
         * @return glm::mat4 proj matrix for the current context
         */
        glm::mat4 getProjectionMatrix();

        /**
         * @brief Move camera in set direction
         * 
         * @param dir of Camera_Movement
         * @param dt is the frame delta time
         */
        void move(Camera_Movement dir, float dt);

        void rotate(float xoffset, float yoffset, GLboolean constrained = true);

        void zoom(float zoom);

        void setFov(float fov);

        void setPerspective();

        void setPerspective(float fov);

        void setOrthographic();

        void setViewPort(GLFWwindow * context);
};