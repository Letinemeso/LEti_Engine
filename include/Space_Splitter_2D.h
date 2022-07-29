#ifndef __SPACE_SPLITTER_2D
#define __SPACE_SPLITTER_2D

#include <list>
#include <map>

#include "Tree.h"
#include "Object.h"

#include "Timer.h"


namespace LEti
{
	class Space_Splitter_2D final		//	TODO: try spatial hashing instead of a quad-tree
	{
	private:
		Space_Splitter_2D() = delete;

	private:
		struct Border
		{
			bool inf = true;
			float value = 0.0f;

			Border() { }
			Border(float _value) : inf(false), value(_value) { }
			void operator=(float _value) { inf = false; value = _value; }
			Border(const Border& _other) : inf(_other.inf), value(_other.value) { }
			void operator=(const Border& _other) { inf = _other.inf; value = _other.value; }

			bool operator<(float _compared_to) const { return value < _compared_to; }
			bool operator>(float _compared_to) const { return value > _compared_to; }
		};

		struct Area
		{
			Border left, right, top, bottom;
			std::list<const Object_2D*> objects;

			Area(const Border& _left, const Border& _right, const Border& _top, const Border& _bottom)
				: left(_left), right(_right), top(_top), bottom(_bottom) { }

			bool rectangle_is_inside(const Physical_Model_2D::Rectangular_Border& _rectangle) const;

			void split(LEti::Tree<Area, 4>::Iterator _it);
		};

	public:
		struct Collision_Data
		{
			const Object_2D* first = nullptr, * second = nullptr;
			Geometry::Intersection_Data collision_data;
			Collision_Data(const Object_2D* _first, const Object_2D* _second) : first(_first), second(_second) {};
			void update_collision_data();
			bool operator==(const Collision_Data& _other) const { return (first == _other.first && second == _other.second) || (first == _other.second && second == _other.first); }
			bool operator<(const Collision_Data& _other) const { return first < _other.first ? true : second < _other.second ? true : false; }
			bool operator>(const Collision_Data& _other) const { return !(*this < _other); }
		};

	private:
		static std::list<const Object_2D*> m_registred_models;

		static unsigned int m_max_tree_depth;
		static LEti::Tree<Area, 4> m_quad_tree;

		static std::list<Collision_Data> m_collisions;

		//map is used for quick search for repeating possible collisions. "bool" as a second template parameter is some sort of a "plug"
		static std::map<Collision_Data, bool> m_possible_collisions;

	private:
		static Timer m_timer;

	public:
//		static void set_update_rate(float _rate); // times per second, aka 1.0f / 60.0f
		static void set_max_tree_depth(unsigned int _max_depth);

		static void register_object(const Object_2D* _model);
		static void unregister_object(const Object_2D* _model);

	private:
		static void split_space_recursive(LEti::Tree<Area, 4>::Iterator _it, unsigned int _level);
		static void check_for_possible_collisions(LEti::Tree<Area, 4>::Iterator _it);
		static bool possible_collision_is_already_saved(const Collision_Data& _cd);
		static void save_possible_collision(const Object_2D* _first, const Object_2D* _second);
		static void save_actual_collisions();

	public:
		static void update();

	public:
		static const std::list<Collision_Data>& get_collisions();

	};

}	/*LEti*/


#endif // __SPACE_SPLITTER_2D
