#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/Camera.h"
#include "header/Chunk.h"
#include "header/EBO.h"
#include "header/Shader.h"
#include "header/Texture.h"
#include "header/VAO.h"
#include "header/VBO.h"
#include "header/WindowSetup.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

int win_width = 1000;
int win_height = 800;

float deltaTime = 0.0f; // time between current frame and last frame

int main() {
    WindowSetup windowSetup;
    sf::Window window = windowSetup.start(win_width, win_height);

    Shader ourShader("../resources/shaders/default.vert", "../resources/shaders/default.frag");

    Chunk ourChunk(16);

    Texture grassTop("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 0, 0);
    Texture grassSides("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE, 1, 0);
    Texture dirt("../assets/images/tilemap.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_UNSIGNED_BYTE, 2, 0);

    grassTop.texUnit(ourShader, "textureTop", 0);
    grassSides.texUnit(ourShader, "textureSides", 1);
    dirt.texUnit(ourShader, "textureBottom", 2);

    sf::Vector2i windowCenter = (window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
    Camera camera(win_width, win_height, glm::vec3(0.f, 0.f, 20.f), windowCenter);

    sf::Clock clock;
    float lastFrame = 0.0f;

    while (window.isOpen()) {
        float currentFrame = clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.Inputs(window, deltaTime);
        camera.Matrix(45.0f, 0.1f, 300.f, ourShader, "cameraMatrix");

        glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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