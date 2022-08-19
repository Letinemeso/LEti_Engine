#ifndef __BROAD_PHASE_INTERFACE
#define __BROAD_PHASE_INTERFACE

#include <list>

//#include "../../include/Object.h"
#include <include/Object_System/Object_2D.h>


namespace LEti
{

	class Broad_Phase_Interface
	{
	public:
		using objects_list = std::list<const LEti::Object_2D*>;
		using points_list = std::list<const glm::vec3*>;

		struct Colliding_Pair
		{
			const LEti::Object_2D* first = nullptr, * second = nullptr;
			Colliding_Pair(const LEti::Object_2D* _first, const LEti::Object_2D* _second) : first(_first), second(_second) { ASSERT(first == second); if(second > first) { const LEti::Object_2D* temp = first; first = second; second = temp; } }
			bool operator==(const Colliding_Pair& _other) const { return (first == _other.first && second == _other.second) || (first == _other.second && second == _other.first); }
			bool operator<(const Colliding_Pair& _other) const { return first < _other.first ? true : second < _other.second ? true : false; }
			bool operator>(const Colliding_Pair& _other) const { return !(*this < _other); }
		};
		using Colliding_Pair_List = std::list<Colliding_Pair>;

		struct Colliding_Point_And_Object
		{
			const LEti::Object_2D* object = nullptr;
			const glm::vec3* point = nullptr;
			Colliding_Point_And_Object(const LEti::Object_2D* _object, const glm::vec3* _point) : object(_object), point(_point) {  }
			bool operator==(const Colliding_Point_And_Object& _other) const { return object == _other.object && point == _other.point; }
			bool operator<(const Colliding_Point_And_Object& _other) const { return object < _other.object; }
			bool operator>(const Colliding_Point_And_Object& _other) const { return !(*this < _other); }
		};
		using Colliding_Point_And_Object_List = std::list<Colliding_Point_And_Object>;

	public:
		virtual ~Broad_Phase_Interface();

	public:
		virtual void update(const objects_list& _registred_objects, const points_list& _registred_points) = 0;
		virtual std::list<Colliding_Pair> get_possible_collisions__models() = 0;
		virtual std::list<Colliding_Point_And_Object> get_possible_collisions__points() = 0;

		virtual void set_precision(unsigned int _precision) = 0;

	};

}

#endif // __BROAD_PHASE_INTERFACE
