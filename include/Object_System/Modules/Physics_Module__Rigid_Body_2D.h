#ifndef PHYSICS_MODULE__RIGID_BODY_2D_H
#define PHYSICS_MODULE__RIGID_BODY_2D_H

#include <Object_System/Modules/Dynamic_Physics_Module_2D.h>
#include <Object_System/Modules/Default_Draw_Module_2D.h>


namespace LEti
{

    class Physics_Module__Rigid_Body_2D__Stub : public Dynamic_Physics_Module_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        float mass = 0.0f;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

	class Physics_Module__Rigid_Body_2D : public Dynamic_Physics_Module_2D
	{
    public:
        DECLARE_VARIABLE;

	private:
		float m_mass = 1.0f;
        glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
        float m_angular_velocity = 0.0f;

    private:
        glm::vec3 calculate_raw_center_of_mass() const;

    public:
        void align_to_center_of_mass(Default_Draw_Module_2D* _associated_draw_module = nullptr);

    public:
        void set_mass(float _mass);
        void set_velocity(const glm::vec3& _v);
        void set_angular_velocity(float _av);

        void apply_linear_impulse(const glm::vec3& _imp);
        void apply_rotation(float _av);

    public:
        float mass() const;
        const glm::vec3& velocity() const;
        float angular_velocity() const;

	};

}


#endif // PHYSICS_MODULE__RIGID_BODY_2D_H
