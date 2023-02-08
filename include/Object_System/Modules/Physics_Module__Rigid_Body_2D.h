#ifndef PHYSICS_MODULE__RIGID_BODY_2D_H
#define PHYSICS_MODULE__RIGID_BODY_2D_H

#include <Object_System/Modules/Default_Physics_Module_2D.h>


namespace LEti
{

	class Physics_Module__Rigid_Body_2D : public Default_Physics_Module_2D
	{
	private:
		//	move mass, moment of inertia and other stuff here

	public:
		void align_to_center_of_mass();

	};

}


#endif // PHYSICS_MODULE__RIGID_BODY_2D_H
