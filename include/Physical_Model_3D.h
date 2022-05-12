#ifndef __PHYSICAL_MODEL
#define __PHYSICAL_MODEL

#include "../include/Physical_Model_Interface.h"

#include "../Debug.h"


namespace LEti
{
    class Physical_Model_3D : public Physical_Model_Interface
	{
    private:

        //contains single pyramid which sides will be used to track collisions
        class Pyramid
        {
        public:
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

            public:
                glm::vec3 get_normal() const;
                Plane_Equasion_Data get_equasion() const;
                bool point_belongs_to_triangle(const glm::vec3& _point) const;

            public:
                glm::vec3 get_intersection_point(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
                Intersection_Data beam_intersecting_polygon(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
                Intersection_Data segment_intersecting_polygon(const glm::vec3& _beam_pos, const glm::vec3& _beam_direction) const;
                bool point_is_on_the_right(const glm::vec3& _point) const;
                bool point_is_on_the_left(const glm::vec3& _point) const;

            public:
                const glm::vec3& A() const;
                const glm::vec3& B() const;
                const glm::vec3& C() const;

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
            Intersection_Data is_intersecting_with_beam(const glm::vec3& _start, const glm::vec3& _direction) const;
            Intersection_Data is_intersecting_with_segment(const glm::vec3& _start, const glm::vec3& _direction) const;

        public:
            const Polygon& operator[](unsigned int _index) const;

        };

    private:
        Pyramid* m_pyramids = nullptr;
        unsigned int m_pyramids_count = 0;

	public:
        Physical_Model_3D();
        Physical_Model_3D(const float* _raw_coords, unsigned int _raw_coords_count);
        void setup(const float* _raw_coords, unsigned int _raw_coords_count) override;

        ~Physical_Model_3D();

        void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) override;

    public:
        Intersection_Data is_intersecting_with_point(const glm::vec3& _point) const override;
        Intersection_Data is_intersecting_with_beam(const glm::vec3& _start, const glm::vec3& _direction) const /*override*/;
        Intersection_Data is_intersecting_with_segment(const glm::vec3& _start, const glm::vec3& _direction) const override;
        Intersection_Data is_intersecting_with_another_model(const Physical_Model_Interface& _other) const override;

	};
}


#endif
