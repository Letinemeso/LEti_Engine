#ifndef __OBJECT_2D_BASE
#define __OBJECT_2D_BASE

#include "Object_Base.h"

#include <include/Math_Stuff.h>

#include <OpenGL/GLM/mat4x4.hpp>
#include <OpenGL/GLM/vec3.hpp>
#include <OpenGL/GLM/gtx/transform.hpp>


namespace TEST {

	class Object_2D_Base : public Object_Base
	{
	protected:
		Object_2D_Base* m_parent = nullptr;

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

	public:
		Object_2D_Base();
		~Object_2D_Base();

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

	public:
		Transformation_Data get_global_transformation() const;

	public:
		bool moved_since_last_frame() const;

	};

}



#endif // __OBJECT_2D_BASE
