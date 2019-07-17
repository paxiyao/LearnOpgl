#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"

class SpriteRenderer
{
public:
	// Constructor
	SpriteRenderer(const Shader& shader);
	// Destructor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Shader state
	Shader shader;
	GLuint quadVAO;
	// Initializes and configures the quad's buffer and vertex attribute
	void initRenderData();
};


#endif // !SPRITE_RENDERER_H

