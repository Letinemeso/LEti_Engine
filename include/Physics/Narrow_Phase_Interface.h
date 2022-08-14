#ifndef __NARROW_PHASE_INTERFACE
#define __NARROW_PHASE_INTERFACE

#include "../../include/Object.h"

#include "Broad_Phase_Interface.h"
#include "Narrowest_Phase_Interface.h"

namespace LEti
{

	class Narrow_Phase_Interface
	{
	protected:
		Narrowest_Phase_Interface* m_narrowest_phase = nullptr;

	public:
		template<typename Narrowest_Phase_Implementation>
		void set_narrowest_phase();

	public:
		struct Collision_Data__Models
		{
			const Object_2D* first = nullptr, * second = nullptr;
			Geometry::Intersection_Data collision_data;
			Collision_Data__Models(const Object_2D* _first, const Object_2D* _second) : first(_first), second(_second) { ASSERT(_first == _second); }
		};
		using Collision_Data_List__Models = std::list<Collision_Data__Models>;
		struct Collision_Data__Points
		{
			const Object_2D* object = nullptr;
			const glm::vec3* point = nullptr;
			Geometry::Intersection_Data collision_data;
			Collision_Data__Points(const Object_2D* _object, const glm::vec3* _point) : object(_object), point(_point) { }
		};
		using Collision_Data_List__Points = std::list<Collision_Data__Points>;

	public:
		virtual ~Narrow_Phase_Interface();

	public:
		virtual void update(const Broad_Phase_Interface::Colliding_Pair_List& _possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List& _possible_collisions__pointsnp) = 0;

		virtual const Collision_Data_List__Models& get_collisions__models() const = 0;
		virtual const Collision_Data_List__Points& get_collisions__points() const = 0;

		virtual void set_precision(unsigned int _precision) = 0;

	};


	template<typename Narrowest_Phase_Implementation>
	void Narrow_Phase_Interface::set_narrowest_phase()
	{
		delete m_narrowest_phase;
		m_narrowest_phase = new Narrowest_Phase_Implementation;
	}

}


#endif // __NARROW_PHASE_INTERFACE
