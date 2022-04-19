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

        //contains single pyramid which sides will be used to track collisions
        class Pyramid
        {
        private:
            //contains single polygon (triangle). four of these will form a pyramid
            class Polygon
            {
            private:
                struct Plane_Equasion_Data { float x_part = 0.0f, y_part = 0.0f, z_part = 0.0f, constant_part = 0.0f; };

            private:
                static constexpr unsigned int m_count = 9;
                static constexpr unsigned int m_cpv = 3;

            private:
                const float* m_raw_coords = nullptr;
                glm::vec3 m_actual_A, m_actual_B, m_actual_C;

            public:
                Polygon();
                void setup(const float* _raw_coords);
                void update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);

            private:
                glm::vec3 get_normal() const;
                Plane_Equasion_Data get_equasion() const;
                bool point_belongs_to_triangle(const glm::vec3& _point) const;

            public:
                glm::vec3 get_intersection_point(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
                bool beam_intersecting_polygon(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
                bool point_is_on_the_right(const glm::vec3& _point) const;
                bool point_is_on_the_left(const glm::vec3& _point) const;

            };

        private:
            static constexpr unsigned int m_raw_coords_count = 12;
            const float* m_raw_coords = nullptr;

            Polygon m_polygons[4];

        public:
            Pyramid();

            void setup(const float* _raw_coords);
            void update_polygons(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);

        public:
            bool point_belongs_to_pyramid(const glm::vec3& _point) const;

        };

	private:
        float* m_raw_coords = nullptr;
        unsigned int m_raw_coords_count = 0;

        Pyramid* m_pyramids = nullptr;
        unsigned int m_pyramids_count = 0;

	public:
        Physical_Model();
        Physical_Model(const float* _raw_coords, unsigned int _raw_coords_count);
        void setup(const float* _raw_coords, unsigned int _raw_coords_count);

        ~Physical_Model();

        void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);

    public:
        bool is_intersecting_with_point(const glm::vec3& _point) const;

	};



}


#endif
