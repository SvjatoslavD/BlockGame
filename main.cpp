#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shaderClass.h"
#include "include/windowSetup.h"
#include "include/VBO.h"
#include "include/VAO.h"
#include "include/EBO.h"
#include "include/gameVariables.h"
#include "include/Texture.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

void processEventsAndInput(sf::Window& window);

int main() {
    windowSetup windowSetup;
    sf::Window window = windowSetup.start(win_width, win_height);

    // ---- build and compile our shader program ----
    Shader ourShader("../shaders/default.vert", "../shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(cube_vertices,sizeof(cube_vertices));
    // EBO EBO1(cube_indices,sizeof(cube_indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1,1,2,GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // VAO1.LinkAttrib(VBO1,2,3,GL_FLOAT, 5 * sizeof(int), (void*)(5 * sizeof(float))); //Normal isn't needed yet
    VAO1.Unbind();
    VBO1.Unbind();
    // EBO1.Unbind();

    std::vector<glm::vec3> cube_positions;
    int iterations = 20;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < iterations; j++) {
            for (int k = 0; k < iterations; k++) {
                cube_positions.emplace_back(i,j,-k);
            }
        }
    }

    Texture GrassTop("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 0, 0);
    Texture GrassSides("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE, 1, 0);
    Texture GrassBottom("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_UNSIGNED_BYTE, 2, 0);


    GrassTop.texUnit(ourShader,"textureTop",0);
    GrassBottom.texUnit(ourShader,"textureBottom",2);
    GrassSides.texUnit(ourShader,"textureSides",1);

    sf::Clock clock;

    while (window.isOpen()) {
        float currentFrame =clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processEventsAndInput(window);

        { // rendering the cubes to the screen
            // clear the buffers
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // pass projection matrix to shader (note that in this case it could change every frame)
            glm::mat4 projection = glm::perspective(glm::radians(fov), (float)win_width / (float)win_height, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection);

            // camera/view transformation
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            ourShader.setMat4("view", view);

            // render boxes
            VAO1.Bind();
            for (auto currentCubePos : cube_positions) {
                // calculate the model matrix for each object and pass it to shader before drawing
                auto model = glm::mat4(1.0f);
                model = glm::translate(model, currentCubePos);
                ourShader.setMat4("model", model);

                // Drawing the faces
                for (int i = 0; i < 6; i++) {
                    switch (i) {
                        case 0:
                            ourShader.setInt("drawnSide", 0);
                            glActiveTexture(GL_TEXTURE0);
                            GrassTop.Bind();
                            glDrawArrays(GL_TRIANGLES, 0, 6);
                            break;
                        case 1:
                            ourShader.setInt("drawnSide", 1);
                            glActiveTexture(GL_TEXTURE2);
                            GrassBottom.Bind();
                            glDrawArrays(GL_TRIANGLES, 6, 6);
                            break;
                        default:
                            ourShader.setInt("drawnSide", 2);
                            glActiveTexture(GL_TEXTURE1);
                            GrassSides.Bind();
                            glDrawArrays(GL_TRIANGLES, i*6,6);
                            break;
                    }
                }
            }

            // end the current frame (internally swaps the front and back buffers)
            window.display();
        }
    }

    VAO1.Delete();
    VBO1.Delete();
    // EBO1.Delete();
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
        float cameraSpeed = 4.f * deltaTime;
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
        if (isKeyPressed(sf::Keyboard::Key::Q)) {window.close();}
}