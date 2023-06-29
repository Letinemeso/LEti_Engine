#ifndef PHYSICS_MODULE__RIGID_BODY_2D_H
#define PHYSICS_MODULE__RIGID_BODY_2D_H

#include <Object_System/Modules/Dynamic_Physics_Module_2D.h>
#include <Object_System/Modules/Default_Draw_Module_2D.h>
#include <Physics/Physical_Models/Rigid_Body_Physical_Model_2D.h>


namespace LEti
{

    class Physics_Module__Rigid_Body_2D__Stub : public Dynamic_Physics_Module_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        float* masses = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

	class Physics_Module__Rigid_Body_2D : public Dynamic_Physics_Module_2D
	{
    public:
        DECLARE_VARIABLE;

	private:
        float m_mass_multiplier = 1.0f;
        glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
        float m_angular_velocity = 0.0f;

    private:
        Physical_Model_2D* M_create_physical_model() const override;

    public:
        glm::vec3 calculate_raw_center_of_mass() const;

    public:
        void align_to_center_of_mass(Default_Draw_Module_2D* _associated_draw_module = nullptr);

    public:
        void set_masses(const float* _masses);

        inline void set_mass_multiplier(float _mass_multiplier) { m_mass_multiplier = _mass_multiplier; }
        inline void set_velocity(const glm::vec3& _v) { m_velocity = _v; }
        inline void set_angular_velocity(float _av) { m_angular_velocity = _av; }

        inline void apply_linear_impulse(const glm::vec3& _imp) { m_velocity += _imp; }
        inline void apply_rotation(float _av) { m_angular_velocity += _av; }

    public:
        float mass() const;
        float moment_of_inertia() const;

        inline float mass_multiplier() const { return m_mass_multiplier; }
        inline const glm::vec3& velocity() const { return m_velocity; }
        inline float angular_velocity() const { return m_angular_velocity; }

	};

}


#endif // PHYSICS_MODULE__RIGID_BODY_2D_H
