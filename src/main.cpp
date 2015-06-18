#include "Shader.hpp"
#include "Terrain.hpp"
#include "ErosionSimulator.hpp"

#include <ctime>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>


int main(void) {
    // Declare and create a new window
    sf::Window window(sf::VideoMode(512, 512), "SFML window");
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(30);

    glewInit();

    Shader computeShader("src/VS_Erosion.glsl", "src/FS_Erosion.glsl");
    Shader visShader("src/VS_Texture.glsl", "src/FS_Texture.glsl");
    ErosionSimulator erosionSimulator(computeShader, 256, 256, 1, 1, "res/terrain");


    // The main loop - ends as soon as the window is closed
    while (window.isOpen())
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Activate the window for OpenGL rendering
        window.setActive();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // OpenGL drawing commands go here...

        erosionSimulator.erode(visShader);

        // End the current frame and display its contents on screen
        window.display();
    }

    //generatePerlin();

    return 0;
}
