#include "Shader.hpp"

#include <ctime>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>


int main(void) {
    /*// Declare and create a new window
    sf::Window window(sf::VideoMode(1024, 1024), "SFML window");
    // Limit the framerate to 60 frames per second (this step is optional)
    window.setFramerateLimit(5);

    glewInit();

    sf::Image img;
    img.loadFromFile("res/maze.png");

    GLuint texture_width = img.getSize().x;
    GLuint texture_height = img.getSize().y;

    GLuint original_texture_handle;
    glGenTextures(1, &original_texture_handle);
    glBindTexture(GL_TEXTURE_2D, original_texture_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());


    GLuint fbo_handle, fbo_texture_handle;

    // generate texture

    glGenTextures(1, &fbo_texture_handle);
    glBindTexture(GL_TEXTURE_2D, fbo_texture_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // generate framebuffer

    glGenFramebuffers(1, &fbo_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture_handle, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Could not validate framebuffer\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    GLfloat quad[] = {
        -1.0f,  -1.0f,  0.0f,
        1.0f,   -1.0f,  0.0f,
        -1.0f,  1.0f,   0.0f,
        -1.0f,  1.0f,   0.0f,
        1.0f,   -1.0f,  0.0f,
        1.0f,   1.0f,   0.0f
    };

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);


    Shader shader("src/VS_Simple.glsl", "src/FS_Simple.glsl");


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


        //  computation
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, texture_width, texture_height);


        shader.useShader();
        glBindTexture(GL_TEXTURE_2D, original_texture_handle);
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_handle);
        glBindTexture(GL_TEXTURE_2D, original_texture_handle);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texture_width, texture_height);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        glPopAttrib();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        //  visualization
        shader.useShader();
        glBindTexture(GL_TEXTURE_2D, original_texture_handle);
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // End the current frame and display its contents on screen
        window.display();
    }*/

    generatePerlin();

    return 0;
}
