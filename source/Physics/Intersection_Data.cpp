#include <Physics/Intersection_Data.h>

using namespace LEti;


Intersection_Data::Intersection_Data()
{

}

Intersection_Data::Intersection_Data(Type _type)
    : type(_type)
{

}

Intersection_Data::Intersection_Data(Type _type, const glm::vec3& _point)
    : type(_type), point(_point)
{

}

Intersection_Data::Intersection_Data(const Intersection_Data& _other)
    : type(_other.type), point(_other.point), normal(_other.normal), depth(_other.depth),
    first(_other.first), second(_other.second),
    first_collided_polygon_index(_other.first_collided_polygon_index), second_collided_polygon_index(_other.second_collided_polygon_index),
    time_of_intersection_ratio(_other.time_of_intersection_ratio)
{

}

Intersection_Data::Intersection_Data(Intersection_Data&& _other)
    : type(_other.type), point(_other.point), normal(_other.normal), depth(_other.depth),
    first(_other.first), second(_other.second),
    first_collided_polygon_index(_other.first_collided_polygon_index), second_collided_polygon_index(_other.second_collided_polygon_index),
    time_of_intersection_ratio(_other.time_of_intersection_ratio)
{

}

void Intersection_Data::operator=(const Intersection_Data& _other)
{
    type = _other.type;
    point = _other.point;
    time_of_intersection_ratio = _other.time_of_intersection_ratio;
    normal = _other.normal;
    depth = _other.depth;
    first = _other.first;
    second = _other.second;
    first_collided_polygon_index = _other.first_collided_polygon_index;
    second_collided_polygon_index = _other.second_collided_polygon_index;
}
