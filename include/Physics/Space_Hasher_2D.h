#ifndef __SPACE_HASHER_2D
#define __SPACE_HASHER_2D

#include <L_Debug/L_Debug.h>
#include <Data_Structures/AVL_Tree.h>

#include <Physics/Broad_Phase_Interface.h>

namespace LEti
{

	class Space_Hasher_2D : public Broad_Phase_Interface
	{
	private:
		unsigned int m_precision = 0;
		unsigned int m_array_size = 0;
		objects_list** m_array = nullptr;
		unsigned int m_number_binary_length = 0;
		struct Space_Border { float min_x = 0.0f, min_y = 0.0f, height = 0.0f, width = 0.0f; };
		Space_Border m_space_borders;

        LDS::AVL_Tree<Colliding_Pair> m_possible_collisions__models;
        LDS::AVL_Tree<Colliding_Point_And_Object> m_possible_collisions__points;

	private:
		unsigned int get_number_binary_length(unsigned int _number);
		unsigned int construct_hash(unsigned int _x, unsigned int _y);
		void update_border(const objects_list& _registred_objects);
		void reset_hash_array();
		void hash_objects(const objects_list& _registred_objects);
		void check_for_possible_collisions__points(const points_list &_registred_points);
		void check_for_possible_collisions__models();

	public:
		Space_Hasher_2D() { }
		virtual ~Space_Hasher_2D();

	public:
		void set_precision(unsigned int _precision) override;

		void update(const objects_list &_registred_objects, const points_list &_registred_points) override;
        LDS::List<Colliding_Pair> get_possible_collisions__models() override;
        LDS::List<Colliding_Point_And_Object> get_possible_collisions__points() override;

	};

}

#endif // __SPACE_HASHER_2D
