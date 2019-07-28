#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

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
    GLFWwindow* window;
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
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

    // Create a Vertex Array Object and set as current one
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // triangle verticies
    static const GLfloat triangle_verticies[] = {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        0.0, 1.0, 0.0
    };
    GLuint trianglebuffer;
    glGenBuffers(1, &trianglebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, trianglebuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(triangle_verticies),
        triangle_verticies,
        GL_STATIC_DRAW
    );

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // Used to id our buffer
    GLuint vertexbuffer;
    // Generate 1 buffer put resulting identifier in `vertexbuffer`
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Put our actual verticies into the buffer
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(g_vertex_buffer_data),
        g_vertex_buffer_data,
        GL_STATIC_DRAW
    );

    // One color for each vertex. They were generated randomly.
    static GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(g_color_buffer_data),
        g_color_buffer_data,
        GL_STREAM_DRAW
    );

    // Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(g_uv_buffer_data),
        g_uv_buffer_data,
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

    // Projection matrix: 45 deg field of view, 4:3 ratio, 0.1<->100 unit
    // distance
    glm::mat4 Projection = glm::perspective(
        glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);

    // Camera Matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4, 3, 3) in world coors
        glm::vec3(0, 0, 0), // Looks at the origin
        glm::vec3(0, 1, 0)  // Head is up
    );

    // Model matrix (identity): model will be at origin
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 TriangleModel = glm::scale(
        glm::mat4(1.0f), glm::vec3(2.6f, 2.9f, 2.0f)
    );
    TriangleModel = glm::translate(TriangleModel, glm::vec3(0.0f, 0.0f, 0.0f));
    // Our Model View Projection, remember to use reverse order
    glm::mat4 mvp = Projection * View * Model;
    glm::mat4 mvptriangle = Projection * View * TriangleModel;

    do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Change the colors
        for(int i = 0; i < 12*3*3; i++){
            g_color_buffer_data[i] += 0.01f;
            if(g_color_buffer_data[i] > 1.0f){
                g_color_buffer_data[i] = 0.0f;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(g_color_buffer_data),
            g_color_buffer_data,
            GL_STATIC_DRAW
        );

        // Send transformation to the currently bound shader
        // Done in the loop because each model will have a different MVP matrix
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

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
        // 2nd attribute buffer: colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        // Actually draw cube
        // Starting from vertex 0; 12*3 verts total
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Draw triangle now
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvptriangle[0][0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, trianglebuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
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

GLuint loadTGA_glfw(const char * imagepath){
    // Create an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    // "Bind: the newly created texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Read the file
    glfwLoadTexture2D(imagepath, 0);

    // trilinear filtering?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipMap(GL_TEXTURE_2D);

    return textureID;
}

GLuint loadBMP_custom(const char * imagepath){
    unsigned char header[54];   // BMP headers are 54 bytes
    unsigned int dataPos;       // Pos in file where data begins
    unsigned int width, height;
    unsigned int imageSize;     // width*height*3
    unsigned char *data;        // Actual RGB data
    // Open the file
    FILE * file = fopen(imagepath, "rb");
    if(!file){
        printf("Image could not be opened\n");
        return 0;
    }
    if(fread(header, 1, 54, file) != 54){
        // If we can't read 54 bytes --> problem
        printf("Not a correct BMP file\n");
        return 0;
    }
    if(header[0] != 'B' || header[1] != 'M'){
        // Header has to start with "BM"
        printf("Not a correct BMP file\n");
        return 0;
    }
    // read ints from the header
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    // Guess info if missing
    if(imageSize == 0){imageSize = width * height*3;}   // *3 for RGB
    if(dataPos == 0){dataPos = 54;}
    data = new unsigned char [imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);
    // Create an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    // "Bind" the newly created texture : all future texture functions affect
    // this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
        GL_UNSIGNED BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
