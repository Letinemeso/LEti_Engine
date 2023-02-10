#ifndef __DYNAMIC_NARROW_CD
#define __DYNAMIC_NARROW_CD

#include "Narrow_Phase_Interface.h"


namespace LEti
{

    class Dynamic_Narrow_CD : public Narrow_Phase_Interface
	{
	private:
		Collision_Data_List__Models m_collisions__models;
		Collision_Data_List__Points m_collisions__points;

		unsigned int m_precision = 10;

	private:
		using float_pair = std::pair<float, float>;
		float_pair find_ratio(const LEti::Object_2D &_moving_1, const LEti::Object_2D &_moving_2) const;

	private:
		Physical_Model_2D::Intersection_Data get_precise_time_ratio_of_collision(const LEti::Object_2D& _first, const LEti::Object_2D& _second, float _min_ratio, float _max_ratio) const;
		Physical_Model_2D::Intersection_Data collision__moving_vs_moving(const LEti::Object_2D& _moving_1, const LEti::Object_2D& _moving_2) const;
		Physical_Model_2D::Intersection_Data collision__moving_vs_static(const LEti::Object_2D& _moving, const LEti::Object_2D& _static) const;

		Geometry::Simple_Intersection_Data collision__static_vs_point(const LEti::Object_2D& _static, const glm::vec3& _point) const;

	public:
		Physical_Model_2D::Intersection_Data objects_collide(const LEti::Object_2D& _first, const LEti::Object_2D& _second) const;

	public:
		void update(const Broad_Phase_Interface::Colliding_Pair_List& _possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List& _possible_collisions__points) override;

		const Collision_Data_List__Models& get_collisions__models() const override;
		const Collision_Data_List__Points& get_collisions__points() const override;

		void set_precision(unsigned int _precision) override;

	};

}


#endif // __DYNAMIC_NARROW_CD
