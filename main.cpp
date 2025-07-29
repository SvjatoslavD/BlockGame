#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/Chunk.h"
#include "include/Shader.h"
#include "include/WindowSetup.h"
#include "include/VBO.h"
#include "include/VAO.h"
#include "include/EBO.h"
#include "include/GameVariables.h"
#include "include/Texture.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

void processEventsAndInput(sf::Window& window);

int main() {
    WindowSetup windowSetup;
    sf::Window window = windowSetup.start(win_width, win_height);

    // ---- build and compile our shader program ----
    Shader ourShader("../shaders/default.vert", "../shaders/default.frag");

    Chunk ourChunk(1);

    Texture grassTop("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 0, 0);
    Texture grassSides("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE, 1, 0);
    Texture dirt("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_UNSIGNED_BYTE, 2, 0);

    grassTop.texUnit(ourShader,"textureTop",0);
    grassSides.texUnit(ourShader,"textureSides",1);
    dirt.texUnit(ourShader,"textureBottom",2);

    sf::Clock clock;

    while (window.isOpen()) {
        // Enable some options
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        float currentFrame =clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processEventsAndInput(window);

        // rendering the cubes to the screen
        // clear the buffers
        glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)win_width / (float)win_height, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // calculate the model matrix for each object and pass it to shader before drawing
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f,0.f,0.f));
        ourShader.setMat4("model", model);

        // Drawing the faces
        ourShader.use();
        ourShader.setInt("drawnSide", 2);
        glActiveTexture(GL_TEXTURE1);
        grassSides.Bind();
        ourChunk.RenderChunk();

        window.display();
    }

    ourShader.Delete();

    return 0;
}

void processEventsAndInput(sf::Window& window) {
    // handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {window.close();}
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                // adjust the viewport when the window is resized
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if (isButtonPressed(sf::Mouse::Button::Left)) {
                    float xpos = static_cast<float>(mouseMoved->position.x);
                    float ypos = static_cast<float>(mouseMoved->position.y);

                    if (firstMouse) {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                    }

                    float xoffset = xpos - lastX;
                    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
                    lastX = xpos;
                    lastY = ypos;

                    float sensitivity = 0.1f; // change this value to your liking
                    xoffset *= sensitivity;
                    yoffset *= sensitivity;

                    yaw += xoffset;
                    pitch += yoffset;

                    // make sure that when pitch is out of bounds, screen doesn't get flipped
                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    if (pitch < -89.0f)
                        pitch = -89.0f;

                    glm::vec3 front;
                    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front.y = sin(glm::radians(pitch));
                    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    cameraFront = glm::normalize(front);
                }
            }
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    firstMouse = true;
                }
            }
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->delta > 0) {
                    fov -= mouseScrollSpeed * deltaTime;
                    if (fov < 1.0f) {fov = 1.0f;}
                }
                if (mouseWheel->delta < 0) {
                    fov += mouseScrollSpeed * deltaTime;
                    if (fov > 55.0f) {fov = 55.0f;}
                }
            }
        }

        // process input
        float cameraSpeed = 6.f * deltaTime;
        if (isKeyPressed(sf::Keyboard::Key::W)) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (isKeyPressed(sf::Keyboard::Key::S)) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (isKeyPressed(sf::Keyboard::Key::A)) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::D)) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::Space)) {
            cameraPos += cameraUp * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::LShift)) {
            cameraPos -= cameraUp * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::Q)) {window.close();}
}