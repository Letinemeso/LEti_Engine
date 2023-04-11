#ifndef COLLISION_RESOLUTION__RIGID_BODY_2D_H
#define COLLISION_RESOLUTION__RIGID_BODY_2D_H

#include <Physics/Collision_Resolver.h>
#include <Object_System/Rigid_Body_2D_Stub.h>


namespace LEti
{

	class Collision_Resolution__Rigid_Body_2D : public Collision_Resolution_Interface
	{
	private:
        glm::vec3 M_calculate_center_of_mass(const Physical_Model_2D& _model) const;
        float M_calculate_moment_of_inertia(const Physical_Model_2D& _model, float _mass) const;

        float M_calculate_kinetic_energy(const glm::vec3& _velocity, float _angular_velocity, float _mass, float _moment_of_inertia) const;

	public:
		bool resolve(const Physical_Model_2D::Intersection_Data &_id) override;

	};

}


#endif // COLLISION_RESOLUTION__RIGID_BODY_2D_H
