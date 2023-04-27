#ifndef INTERSECTION_DATA_H
#define INTERSECTION_DATA_H

#include <vec3.hpp>

namespace LEti
{

    class Object_2D;

    struct Intersection_Data
    {
    public:
        enum class Type
        {
            none = 0,
            intersection,
            same_line
        };

    public:
        Type type = Type::none;
        glm::vec3 point{0.0f, 0.0f, 0.0f};
        glm::vec3 normal{0.0f, 0.0f, 0.0f};
        float depth = 0.0f;
        const Object_2D* first = nullptr, *second = nullptr;
        float time_of_intersection_ratio = 1.0f;

    public:
        Intersection_Data();
        Intersection_Data(Type _type);
        Intersection_Data(Type _type, const glm::vec3& _point);
        Intersection_Data(const Intersection_Data& _other);
        Intersection_Data(Intersection_Data&& _other);
        void operator=(const Intersection_Data& _other);

    public:
        inline operator bool() { return type != Type::none; }
    };

}

#endif // INTERSECTION_DATA_H
