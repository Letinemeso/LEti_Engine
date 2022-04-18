#ifndef __PHYSICAL_MODEL
#define __PHYSICAL_MODEL

#include "../OpenGL/GLM/vec4.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"

#include "../Debug.h"

#include "../include/Utility.h"


namespace LEti
{
	class Physical_Model
	{
	private:
		class Polygon
		{
		private:
			struct Plane_Equasion_Data { float x_part = 0.0f, y_part = 0.0f, z_part = 0.0f, constant_part = 0.0f; };

		private:
			static constexpr unsigned int m_count = 9;
			static constexpr unsigned int m_cpv = 3;

		private:
            const float* m_raw_coords = nullptr;
            glm::vec3 A, B, C;

		public:
			Polygon();
            void setup(const float* _coords);
            void update_points(const glm::mat4x4* _translation, glm::mat4x4* _rotation, glm::mat4x4* _scale);

		private:
			glm::vec3 get_normal() const;
			Plane_Equasion_Data get_equasion() const;
			bool point_belongs_to_triangle(const glm::vec3& _point) const;

		public:
			glm::vec3 get_intersection_point(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
            bool beam_intersecting_polygon(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;

		};

	private:
        float* m_raw_coords = nullptr;
        const glm::mat4x4* m_translation = nullptr, * m_rotation = nullptr, * m_scale = nullptr;

	public:
		Physical_Model(float* _raw_coords);


	};



}


#endif
