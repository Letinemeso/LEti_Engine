#ifndef __DEFAULT_PHYSICS_MODULE_2D
#define __DEFAULT_PHYSICS_MODULE_2D

#include "Physics_Module_Base.h"

#include "../../Physics/Physical_Model_2D.h"


namespace LEti {

	class Default_Physics_Module_2D : public Physics_Module_Base
	{
	private:
		bool m_can_collide = false;
		LEti::Physical_Model_2D* m_physical_model = nullptr;
		LEti::Physical_Model_2D::Imprint* m_physical_model_prev_state = nullptr;
		LEti::Geometry_2D::Rectangular_Border m_dynamic_rb;
		bool m_is_dynamic = false;

	public:
		Default_Physics_Module_2D();
		~Default_Physics_Module_2D();

		void init(const float* _coords, unsigned int _coords_count);

	public:
		void update_previous_state() override;
		void update(const glm::mat4x4 &_translation, const glm::mat4x4 &_rotation, const glm::mat4x4 &_scale) override;

	public:
		void set_can_collide(bool _can_collide);
		bool can_collide() const;
		void set_is_dynamic(bool _is_dynamic);
		bool is_dynamic() const;

	public:
		LEti::Physical_Model_2D* get_physical_model();
		LEti::Physical_Model_2D::Imprint* get_physical_model_prev_state();
		const LEti::Physical_Model_2D* get_physical_model() const;
		const LEti::Physical_Model_2D::Imprint* get_physical_model_prev_state() const;
		const LEti::Geometry_2D::Rectangular_Border& get_dynamic_rb() const;

	};

}


#endif // __DEFAULT_PHYSICS_MODULE_2D
