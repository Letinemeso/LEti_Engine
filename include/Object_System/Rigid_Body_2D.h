#ifndef RIGID_BODY_2D_H
#define RIGID_BODY_2D_H

#include <Object_System/Object_2D.h>


namespace LEti
{

	class Rigid_Body_2D : public Object_2D
	{
	private:
		glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
		float m_angular_velocity = 0.0f;
		float m_mass = 1.0f;

	public:
		void update(float _ratio = 1.0f) override;

	public:
		void set_velocity(const glm::vec3& _v);
		void set_angular_velocity(float _av);
		void set_mass(float _mass);

		void apply_linear_impulse(const glm::vec3& _imp);
		void apply_rotation(float _av);

	public:
		const glm::vec3& velocity() const;
		float angular_velocity() const;
		float mass() const;

	};

}

#endif // RIGID_BODY_2D_H
