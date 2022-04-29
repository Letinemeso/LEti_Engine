#ifndef __PHYSICAL_MODEL_INTERFACE
#define __PHYSICAL_MODEL_INTERFACE

#include "../OpenGL/GLM/vec4.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"

#include "../include/Utility.h"


namespace LEti {

    class Physical_Model_Interface
    {
    protected:
        float* m_raw_coords = nullptr;
        unsigned int m_raw_coords_count = 0;

    public:
        Physical_Model_Interface() { };
        Physical_Model_Interface(const float* _raw_coords, unsigned int _raw_coords_count);
        virtual void setup(const float* _raw_coords, unsigned int _raw_coords_count);

        virtual ~Physical_Model_Interface();

        virtual void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) = 0;

    public:
        virtual bool is_intersecting_with_point(const glm::vec3& _point) const = 0;
        virtual bool is_intersecting_with_beam(const glm::vec3& _start, const glm::vec3& _direction) const = 0;
        virtual bool is_intersecting_with_segment(const glm::vec3& _start, const glm::vec3& _direction) const = 0;
        virtual bool is_intersecting_with_another_model(const Physical_Model_Interface& _other) const = 0;

    };

}

#endif // __PHYSICAL_MODEL_INTERFACE
