#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <GGSim/Application.h>

int main()
{			
    Window win(800, 600, "Test");

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -0.9f, -0.9f, 0.0f,
        -0.8f, -1.f, 0.0f,
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,                    // right
        0.0f, 0.5f, 0.0f                      // top

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (win.isOpen())
    {
		win.clear();

        // draw our first triangle
        win.draw();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        win.display();
		glfwPollEvents();
	}

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}