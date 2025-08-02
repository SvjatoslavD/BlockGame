#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "header/Camera.h"
#include "header/Chunk.h"
#include "header/Shader.h"
#include "header/Texture.h"
#include "header/Window-Setup.h"
#include "header/World.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

int win_width = 1000;
int win_height = 800;

float deltaTime = 0.0f; // time between current frame and last frame

int main() {
    WindowSetup windowSetup;
    sf::Window window = windowSetup.Start(win_width, win_height);

    Shader ourShader("../resources/shaders/default.vert", "../resources/shaders/default.frag");

    World our_world{};

    Texture grassTop("../assets/images/tile-map.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 0, 0);
    Texture grassSides("../assets/images/tile-map.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE, 1, 0);
    Texture dirt("../assets/images/tile-map.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_UNSIGNED_BYTE, 2, 0);

    grassTop.TexUnit(ourShader, "textureTop", 0);
    grassSides.TexUnit(ourShader, "textureSides", 1);
    dirt.TexUnit(ourShader, "textureBottom", 2);

    sf::Vector2i windowCenter = (window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
    Camera camera(win_width, win_height, glm::vec3(0.f, 520.f, 0.f), windowCenter);

    sf::Clock clock;
    float lastFrame = 0.0f;

    while (window.isOpen()) {
        float currentFrame = clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.HandleInputs(window, deltaTime);
        camera.Matrix(45.0f, 0.1f, 200.f, ourShader, "cameraMatrix");

        glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Activate();
        ourShader.setInt("drawnSide", 2);
        glActiveTexture(GL_TEXTURE1);
        grassSides.Bind();
        our_world.Update(camera.getChunkCoordinates());
        our_world.RenderChunks(ourShader, camera);

        window.display();
    }

    ourShader.Delete();

    return 0;
}