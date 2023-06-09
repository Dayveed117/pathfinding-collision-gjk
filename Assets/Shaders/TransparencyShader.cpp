#include "TransparencyShader.hpp"

TransparencyShader::TransparencyShader(Renderer* renderer, const char* texturePath)
{
  this->renderer = renderer;
  this->texture = Texture::LoadTexture(texturePath);

  try
  {
    shader->compileShader("GLSLShaders/TransparencyShader.vert");
    shader->compileShader("GLSLShaders/TransparencyShader.frag");

    shader->link();
    shader->use();
  }
  catch (GLSLProgramException& e)
  {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }
}

void TransparencyShader::LoadShader(mat4 model, mat4 view, mat4 projection)
{
  // Uses shaderId as our shader
  GLuint shaderId = shader->getHandle();
  glUseProgram(shader->getHandle());

  // Gets the mvp position
  unsigned int m = glGetUniformLocation(shaderId, "model");
  // Passes the matrix to the shader
  glUniformMatrix4fv(m, 1, GL_FALSE, &model[0][0]);

  unsigned int v = glGetUniformLocation(shaderId, "view");
  // Passes the matrix to the shader
  glUniformMatrix4fv(v, 1, GL_FALSE, &view[0][0]);

  unsigned int p = glGetUniformLocation(shaderId, "projection");
  // Passes the matrix to the shader
  glUniformMatrix4fv(p, 1, GL_FALSE, &projection[0][0]);

  // be sure to activate shader when setting uniforms/drawing objects
  vec3 color = renderer->GetIPrimitive()->GetColor() / vec3(255.f);
  glUniform3f(glGetUniformLocation(shaderId, "objectColor"), color.x, color.y, color.z);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texture);

  renderer->Render();


  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
}

IPrimitive* TransparencyShader::GetIPrimitive()
{
  return this->renderer->GetIPrimitive();
}

void TransparencyShader::UpdateShader(const char* texturePath)
{
    this->texture = Texture::LoadTexture(texturePath);
}