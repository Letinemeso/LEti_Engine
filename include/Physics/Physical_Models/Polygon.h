#ifndef POLYGON_H
#define POLYGON_H

#include <Math_Stuff.h>

namespace LEti
{

    class Polygon
    {
    private:
        static constexpr unsigned int m_count = 9;
        static constexpr unsigned int m_cpv = 3;

    private:
        const float* m_raw_coords = nullptr;
        glm::vec3 m_actual_A, m_actual_B, m_actual_C;
        glm::vec3 m_center_raw, m_center;

        const bool* m_segment_can_collide = nullptr;

    public:
        Polygon();
        Polygon(const Polygon& _other);
        virtual ~Polygon();
        void setup(const float* _raw_coords, const bool* _segment_can_collide);
        virtual void setup(const Polygon& _other);
        void calculate_center();
        void update_points(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
        void update_points_with_single_matrix(const glm::mat4x4& _matrix);

    public:
        const glm::vec3& operator[](unsigned int _index) const;
        glm::vec3& operator[](unsigned int _index);
        bool segment_can_collide(unsigned int _index) const;
        const glm::vec3& center() const;
        const glm::vec3& center_raw() const;

    };

}

#endif // POLYGON_H
