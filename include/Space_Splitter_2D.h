#ifndef __SPACE_SPLITTER_2D
#define __SPACE_SPLITTER_2D

#include "../Debug.h"

#include <list>
#include "Object.h"

namespace LEti
{
	class Space_Splitter_2D final		//	TODO: try spatial hashing instead of a quad-tree
	{
	private:
		Space_Splitter_2D() = delete;

	public:
		struct Collision_Data
		{
			const Object_2D* first = nullptr, * second = nullptr;
			Geometry::Intersection_Data collision_data;
			Collision_Data(const Object_2D* _first, const Object_2D* _second)/* : first(_first), second(_second) {}*/;
			void update_collision_data();
			bool operator==(const Collision_Data& _other) const { return (first == _other.first && second == _other.second) || (first == _other.second && second == _other.first); }
			bool operator<(const Collision_Data& _other) const { return first < _other.first ? true : second < _other.second ? true : false; }
			bool operator>(const Collision_Data& _other) const { return !(*this < _other); }
		};

	private:
		using objects_list = std::list<const Object_2D*>;
		static objects_list m_registred_models;

		static unsigned int m_precision;
		static unsigned int m_array_size;
		static objects_list** m_array;
		static unsigned int m_number_binary_length;
		struct Space_Border { float min_x = 0.0f, min_y = 0.0f, height = 0.0f, width = 0.0f; };
		static Space_Border m_space_borders;

		//map is used for fast(er) search for repeating possible collisions. "bool" as a second template parameter is some sort of a "plug"
		static std::map<Collision_Data, bool> m_possible_collisions;
		static std::list<Collision_Data> m_collisions;

	private:
		static unsigned int get_number_binary_length(unsigned int _number);

	public:
		static void set_precision(unsigned int _precision);

		static void register_object(const Object_2D* _model);
		static void unregister_object(const Object_2D* _model);

	private:
		static void update_border();
		static void reset_hash_array();
		static void hash_objects();
		static void check_for_possible_collisions();
		static void save_actual_collisions();

	public:
		static void update();

	public:
		static const std::list<Collision_Data>& get_collisions();

	};

}	/*LEti*/

#endif // __Space_Splitter_2D
