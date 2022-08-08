#ifndef __DEFAULT_NARROW_CD
#define __DEFAULT_NARROW_CD

#include "Narrow_Phase_Interface.h"


namespace LEti
{

	class Default_Narrow_CD : public Narrow_Phase_Interface
	{
	private:
		Collision_Data_List__Models m_collisions__models;
		Collision_Data_List__Points m_collisions__points;

		unsigned int m_precision = 10;

	private:
		enum class Axis_To_Use
		{
			X = 0,
			Y = 1
		};

		using float_pair = std::pair<float, float>;
		float_pair find_ratio(const Object_2D &_moving_1, const Object_2D &_moving_2, Axis_To_Use _axis) const;

	private:
		Geometry::Intersection_Data get_precise_time_ratio_of_collision(const Object_2D& _first, const Object_2D& _second, float _min_ratio, float _max_ratio) const;
		Geometry::Intersection_Data collision__moving_vs_moving(const Object_2D& _moving_1, const Object_2D& _moving_2) const;
		Geometry::Intersection_Data collision__moving_vs_static(const Object_2D& _moving, const Object_2D& _static) const;

		Geometry::Intersection_Data collision__static_vs_point(const Object_2D& _static, const glm::vec3& _point) const;

	public:
		Geometry::Intersection_Data objects_collide(const Object_2D& _first, const Object_2D& _second) const;

	public:
		void update(const Broad_Phase_Interface::Colliding_Pair_List& _possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List& _possible_collisions__points) override;

		const Collision_Data_List__Models & get_collisions__models() const override;
		const Collision_Data_List__Points & get_collisions__points() const override;

		void set_precision(unsigned int _precision) override;

	};

}


#endif // __DEFAULT_NARROW_CD
