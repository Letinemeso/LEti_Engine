#ifndef DRAW_MODULE_BASE_H
#define DRAW_MODULE_BASE_H

#include <OpenGL/GLM/mat4x4.hpp>


namespace TEST {

	class Draw_Module_Base
	{
	private:


	public:
		Draw_Module_Base();
		virtual ~Draw_Module_Base();

	public:
		virtual void draw(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4 _scale) const = 0;

	};

}


#endif // DRAW_MODULE_BASE_H
