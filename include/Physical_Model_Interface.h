#ifndef __PHYSICAL_MODEL_INTERFACE
#define __PHYSICAL_MODEL_INTERFACE

#include "../OpenGL/GLM/vec4.hpp"
#include "../OpenGL/GLM/mat4x4.hpp"

#include "../include/Math.h"


namespace LEti {

    class Physical_Model_Interface
    {
    public:
        struct Intersection_Data
        {
            enum class Intersection_Type
            {
                none = 0,
                partly_outside,
                inside
            };
            Intersection_Type type = Intersection_Type::none;
            glm::vec3 closest_intersection_point;
	    float time_of_intersection_ratio = 1.0f;

            Intersection_Data() { }
            Intersection_Data(Intersection_Type _type) : type(_type) { }
            Intersection_Data(Intersection_Type _type, const glm::vec3& _closest_intersection_point) : type(_type), closest_intersection_point(_closest_intersection_point) { }
            Intersection_Data(const Intersection_Data& _other) : type(_other.type), closest_intersection_point(_other.closest_intersection_point) { }
            Intersection_Data(Intersection_Data&& _other) : type(_other.type), closest_intersection_point(_other.closest_intersection_point) { }
	    void operator=(const Intersection_Data& _other) { type = _other.type; closest_intersection_point = _other.closest_intersection_point; }
            operator bool() { return type != Intersection_Type::none; }
        };

    protected:
        float* m_raw_coords = nullptr;
        unsigned int m_raw_coords_count = 0;

    private:
	bool m_is_dynamic = false;

    public:
        Physical_Model_Interface() { };
        Physical_Model_Interface(const float* _raw_coords, unsigned int _raw_coords_count);
        virtual void setup(const float* _raw_coords, unsigned int _raw_coords_count) = 0;

        virtual ~Physical_Model_Interface();

	void set_is_dynamic(bool _is_dynamic);
	bool is_dynamic() const;

        virtual void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) = 0;

    public:
        virtual Intersection_Data is_intersecting_with_point(const glm::vec3& _point) const = 0;
//        virtual bool is_intersecting_with_beam(const glm::vec3& _start, const glm::vec3& _direction) const = 0;
        virtual Intersection_Data is_intersecting_with_segment(const glm::vec3& _start, const glm::vec3& _direction) const = 0;
        virtual Intersection_Data is_intersecting_with_another_model(const Physical_Model_Interface& _other) const = 0;

    };

}

#endif // __PHYSICAL_MODEL_INTERFACE
