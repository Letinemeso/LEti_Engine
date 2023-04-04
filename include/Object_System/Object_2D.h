#ifndef __OBJECT_2D
#define __OBJECT_2D

#include <Stuff/Function_Wrapper.h>

#include "Object_System/Builder_Stub.h"
#include "Object_Base.h"
#include "Modules/Default_Draw_Module_2D.h"
#include "Modules/Dynamic_Physics_Module_2D.h"
#include "Picture_Manager.h"



namespace LEti
{

    class Object_2D_Stub : public Builder_Stub
	{
	public:
		DECLARE_VARIABLE;

	public:
		glm::vec3 position, scale, rotation_axis;
        float rotation_angle;

        bool enable_draw_module = false;
        Draw_Module_Base_Stub* draw_module = nullptr;
        bool enable_physics_module = false;
        Physics_Module_Base_Stub* physics_module = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* /*_product*/) const override;

    public:
        ~Object_2D_Stub();

	};

	class Object_2D : public Object_Base
	{
	public:
		DECLARE_VARIABLE;

	protected:
		Default_Draw_Module_2D* m_draw_module = nullptr;
		Dynamic_Physics_Module_2D* m_physics_module = nullptr;

	protected:
		struct Transformation_Data
		{
			glm::mat4x4 translation_matrix, rotation_matrix, scale_matrix;
			glm::vec3 rotation_axis;
			float rotation_angle = 0.0f;
			void update(const glm::mat4x4& _translation_matrix,
						const glm::mat4x4& _rotation_matrix,
						const glm::mat4x4& _scale_matrix,
						const glm::vec3& _rotation_axis,
						float _rotation_angle)
				{ translation_matrix = _translation_matrix; rotation_matrix = _rotation_matrix; scale_matrix = _scale_matrix; rotation_axis = _rotation_axis; rotation_angle = _rotation_angle; }
		};

		Transformation_Data m_current_state;
		Transformation_Data m_previous_state;

    protected:
        LST::Function<void(float)> m_on_update;

	public:
		Object_2D();
        ~Object_2D();

    public:
        inline void set_on_update_func(const LST::Function<void(float)> _on_update) { m_on_update = _on_update; }

    public:
		void set_pos(const glm::vec3& _position);
		void move(const glm::vec3& _stride);

		void set_rotation_axis(const glm::vec3& _axis);
		void set_rotation_angle(float _angle);
		void rotate(float _angle);

		void set_scale(const glm::vec3& _scale);
		void set_scale(float _scale);

	public:
		glm::vec3 get_pos() const;
		glm::vec3 get_scale() const;
		glm::vec3 get_rotation_axis() const;
		float get_rotation_angle() const;

		glm::vec3 get_pos_prev() const;
		glm::vec3 get_scale_prev() const;
		glm::vec3 get_rotation_axis_prev() const;
		float get_rotation_angle_prev() const;

		glm::mat4x4 get_translation_matrix_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_rotation_matrix_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_scale_matrix_for_time_ratio(float _ratio) const;

		glm::mat4x4 get_translation_matrix_inversed_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_rotation_matrix_inversed_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_scale_matrix_inversed_for_time_ratio(float _ratio) const;

	public:
		bool moved_since_last_frame() const;

	public:
        void set_draw_module(Default_Draw_Module_2D* _module);
		void remove_draw_module();
		Default_Draw_Module_2D* draw_module();
		const Default_Draw_Module_2D* draw_module() const;

        void set_physics_module(Dynamic_Physics_Module_2D* _module);
		void remove_physics_module();
		Dynamic_Physics_Module_2D* physics_module();
		const Dynamic_Physics_Module_2D* physics_module() const;

	public:
		virtual void revert_to_previous_state();
		virtual void update_previous_state();
        void update(float _ratio = 1.0f) override;

	};

}


#endif // __OBJECT_2D
