#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

#define PI 3.141592653589793238f

int main( void )
{
	// Initialize GLFW
	glewExperimental = true;   // Needed for core profile
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);    // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Just for OSX, not strictly necessasry
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // We don't want the old version of OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    // GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Tutorial 07", NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel"
                "GPU, they are not 3.3 compatible. Try version 2.1\n");
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);     // Init GLEW
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -3;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if closer to the camera
    glDepthFunc(GL_LESS);
    // Cull unseen triangles
    glEnable(GL_CULL_FACE);

    // Create a Vertex Array Object and set as current one
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    std::vector<glm::vec3> verticies;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ("cube.obj", verticies, uv, normals);

    // Used to id our buffer
    GLuint vertexbuffer;
    // Generate 1 buffer put resulting identifier in `vertexbuffer`
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Put our actual verticies into the buffer
    glBufferData(
        GL_ARRAY_BUFFER,
        verticies.size() * sizeof(glm::vec3),
        &verticies[0],
        GL_STATIC_DRAW
    );

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        uv.size() * sizeof(glm::vec2),
        &uv[0],
        GL_STATIC_DRAW
    );

    // Ensure we can capture the escape key being pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set background to dark blue
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile SLSL program from the shaders
    GLuint programID = LoadShaders(
        "SimpleVertexShader.vertexshader",
        "SimpleFragmentShader.fragmentshader"
    );

    // Get a handle on our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    GLuint Texture = loadDDS("uvmap.DDS");
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // Model matrix (identity): model will be at origin
    glm::mat4 Model = glm::mat4(1.0f);
    long frame = 0;
    float cam_speed = 0.01f;
    float rad = 6.0f;
    float field_of_view = PI/4.0f;

    do{
        frame++;
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        glm::vec3 position = glm::vec3(rad * cos(frame * cam_speed), -4.0f,
            -rad * sin(frame * cam_speed));
        glm::vec3 right = glm::vec3(
            -rad * sin(frame * cam_speed), 0.0f, -rad * cos(frame * cam_speed)
        );
        glm::mat4 View = glm::lookAt(
            position, glm::vec3(0.0f, 0.0f, 0.0f), glm::cross(right, -position)
        );
        glm::mat4 Projection = glm::perspective(
            field_of_view, 4.0f/3.0f, 0.1f, 100.0f
        );

        // Our Model View Projection, remember to use reverse order
        glm::mat4 mvp = Projection * View * Model;

        // Send transformation to the currently bound shader
        // Done in the loop because each model will have a different MVP matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // Draw cube
        // 1st attribute buffer: verticies
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,          // attribute 0. Must match layout in shader
            3,          // size
            GL_FLOAT,   // type
            GL_FALSE,   // normalized?
            0,          // stride
            (void*)0    // array buffer offset
        );
        // 2nd attribute buffer: texture
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        // Actually draw cube
        // Starting from vertex 0; 12*3 verts total
        glDrawArrays(GL_TRIANGLES, 0, verticies.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if ESC pressed or window closed
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

	return 0;
}
