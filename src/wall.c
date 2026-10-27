//as cordenadas sao xx yy e zz

float vertices[] = {
  -0.5f, 0.0f, -0.5f, 
   0.5f, 0.0f, -0.5f,
   0.5f, 0.0f, 0.5f,
   -0.5f, 0.0f, 0.5f
};


unsigned int VBO;
glGenBuffers(1, &VBO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

const char *vertexShaderSource  = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos; \n"
  "void main()\n"
  "{\n}"
  "  gl_Position = vec4(aPos.x, aPos.y, 1.0);"
  "}/0";