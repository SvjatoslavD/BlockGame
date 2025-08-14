#include "core/application/Application.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

int main() {
    // Shader ourShader("../resources/shaders/default.vert", "../resources/shaders/default.frag");
    //
    // World our_world{};
    //
    // Texture test("../assets/images/tile-map.png", GL_TEXTURE_2D_ARRAY, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, 16);
    //
    // test.TexUnit(ourShader, "TextureArray1", 0);
    //
    // glm::fvec2 atlas_tile_size = glm::vec2(1.f/5.f,1.f/5.f);
    // ourShader.setVec2("atlasTileSize", atlas_tile_size);
    //
    // sf::Vector2i windowCenter = (window.getPosition() + sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
    // Camera camera(win_width, win_height, glm::vec3(0.f, 110.f, 0.f), windowCenter);
    //
    //
    // while (window.isOpen()) {
    //
    //     camera.HandleInputs(window, deltaTime);
    //     camera.Matrix(50.0f, 0.1f, 300.f, ourShader, "cameraMatrix");
    //
    //     glClearColor(0.5f, 0.8f, 0.92f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    //     ourShader.Activate();
    //     glActiveTexture(GL_TEXTURE0);
    //     test.Bind();
    //     our_world.Update(camera.getChunkCoordinates());
    //     our_world.RenderChunks(ourShader, camera);
    //
    //     window.display();
    // }
    //
    // ourShader.Delete();

	Application app(1000,800, 60);

    return 0;
}