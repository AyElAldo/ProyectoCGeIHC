#pragma once
/* para windows */
#include <glew.h>
#include <glm.hpp>
/*pa linux*/
//#include <GL/glew.h>
//#include<glm/glm.hpp>
class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

