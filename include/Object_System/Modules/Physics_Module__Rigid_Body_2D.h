#ifndef PHYSICS_MODULE__RIGID_BODY_2D_H
#define PHYSICS_MODULE__RIGID_BODY_2D_H

#include <Object_System/Modules/Dynamic_Physics_Module_2D.h>


namespace LEti
{

	class Physics_Module__Rigid_Body_2D : public Dynamic_Physics_Module_2D
	{
	private:
		float m_mass = 1.0f;

    public:
        void align_to_center_of_mass();
        void set_mass(float _mass);

    public:
        float mass() const;

        glm::vec3 calculate_raw_center_of_mass() const;

	};

}


#endif // PHYSICS_MODULE__RIGID_BODY_2D_H
