#ifndef __PHYSICS_MODULE_BASE
#define __PHYSICS_MODULE_BASE

#include <mat4x4.hpp>


namespace LEti {

	class Physics_Module_Base
	{
	private:


	public:
		Physics_Module_Base();
		virtual ~Physics_Module_Base();

	public:
		virtual void update_previous_state() = 0;
		virtual void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) = 0;

	};

}


#endif // __PHYSICS_MODULE_BASE
