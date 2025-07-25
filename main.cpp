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
    VAO1.LinkAttrib(VBO1,2,3,GL_FLOAT, 5 * sizeof(int), (void*)(5 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    // EBO1.Unbind();

    std::vector<glm::vec3> cube_positions;
    int iterations = 1;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < iterations; j++) {
            for (int k = 0; k < iterations; k++) {
                cube_positions.emplace_back(i,j,-k);
            }
        }
    }

    // load texture to be used later
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Most images are 4 byte aligned, so if pixel data only has 3 bytes, there will be a malformed texture

    sf::Image tempImage;
    bool image_loaded = tempImage.loadFromFile("../assets/images/tilemap.png");

    int img_width = tempImage.getSize().x;
    int img_height = tempImage.getSize().y;

    const std::uint8_t *data = tempImage.getPixelsPtr();
    if (image_loaded) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0);

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

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);

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

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // render container
            ourShader.use();
            glDrawArrays(GL_TRIANGLES, 0, 36);

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