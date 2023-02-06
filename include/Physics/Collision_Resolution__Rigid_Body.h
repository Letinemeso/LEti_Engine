#ifndef COLLISION_RESOLUTION__RIGID_BODY_H
#define COLLISION_RESOLUTION__RIGID_BODY_H

#include <Physics/Collision_Resolver.h>
#include <Object_System/Rigid_Body_2D.h>


namespace LEti
{

	class Collision_Resolution__Rigid_Body : public Collision_Resolution_Interface
	{
	public:
		bool resolve(const Physical_Model_2D::Intersection_Data &_id) override;

	};

}


#endif // COLLISION_RESOLUTION__RIGID_BODY_H
