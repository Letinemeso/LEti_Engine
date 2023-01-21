#include "Physics/SAT_Narrowest_CD.h"

using namespace LEti;


SAT_Narrowest_CD::mm_pair SAT_Narrowest_CD::M_get_minmax_projections(const glm::vec3 &_axis, const Geometry::Polygon &_pol) const
{
	float min = 0.0f, max = 0.0f;

	min = Geometry_2D::point_to_axis_projection(_pol[0], _axis);
	max = Geometry_2D::point_to_axis_projection(_pol[0], _axis);

	for(unsigned int i=1; i<3; ++i)
	{
		float proj = Geometry_2D::point_to_axis_projection(_pol[i], _axis);
		if(proj < min)
			min = proj;
		if(proj > max)
			max = proj;
	}

	return {min, max};
}


SAT_Narrowest_CD::Intersection_Data SAT_Narrowest_CD::M_polygons_collision(const Geometry::Polygon &_first, const Geometry::Polygon &_second) const
{
	SAT_Narrowest_CD::Intersection_Data result;

	for(unsigned int i=0; i<3; ++i)
	{
		glm::vec3 axis = _first[i + 1] - _first[i];
		Math::shrink_vector_to_1(axis);
		Geometry_2D::rotate_perpendicular_ccw(axis);

		mm_pair f = M_get_minmax_projections(axis, _first);
		mm_pair s = M_get_minmax_projections(axis, _second);

		if(f.first > s.second || s.first > s.second)
			return Intersection_Data();

		result.intersection = true;

		float dist_1 = fabs(f.second - s.first);
		float dist_2 = fabs(s.second - f.first);

		float min = dist_1 < dist_2 ? dist_1 : dist_2;

		if(result.min_dist < 0.0f || result.min_dist > min)
		{
			result.min_dist = min;
			result.min_dist_axis = axis;
		}
	}

	return result;
}


float SAT_Narrowest_CD::M_smallest_point_to_polygon_distance(const glm::vec3 &_point, const Pol &_pol) const
{
	float min_dist = -1.0f;

	for(unsigned int i=0; i<3; ++i)
	{
		float min = Geometry_2D::point_to_segment_distance(_point, _pol[i], _pol[i + 1]);

		if(min < 0.0f)
			continue;

		if(min_dist < 0.0f || min_dist > min)
			min_dist = min;
	}

	return min_dist;
}

LDS::List<glm::vec3> SAT_Narrowest_CD::M_points_of_contact(const Geometry::Polygon *_f_pols, unsigned int _f_count, const Geometry::Polygon *_s_pols, unsigned int _s_count) const
{
	float min_dist = -1.0f;

	LDS::List<glm::vec3> result;

	auto point_already_counted = [&](const glm::vec3& _point)->bool
	{
		for(LDS::List<glm::vec3>::Iterator it = result.begin(); !it.end_reached(); ++it)
			if(Math::vecs_are_equal(*it, _point))
				return true;
		return false;
	};

	for(unsigned int i=0; i<_f_count; ++i)
	{
		const Pol& cur_pol = _f_pols[i];

		for(unsigned int j=0; j<3; ++j)
		{
			const glm::vec3& cur_point = cur_pol[j];

			for(unsigned int s_pol_i = 0; s_pol_i < _s_count; ++s_pol_i)
			{
				float min = M_smallest_point_to_polygon_distance(cur_point, _s_pols[s_pol_i]);

				if(min < 0.0f)
					continue;

				if(min_dist < 0.0f || min_dist > min)
				{
					result.clear();
					min_dist = min;
					result.push_back(cur_point);
				}
				else if(Math::floats_are_equal(min_dist, min))
				{
					if(point_already_counted(cur_point))
						continue;
					result.push_back(cur_point);
				}
			}
		}
	}

	for(unsigned int i=0; i<_s_count; ++i)
	{
		const Pol& cur_pol = _s_pols[i];

		for(unsigned int j=0; j<3; ++j)
		{
			const glm::vec3& cur_point = cur_pol[j];

			for(unsigned int f_pol_i = 0; f_pol_i < _f_count; ++f_pol_i)
			{
				float min = M_smallest_point_to_polygon_distance(cur_point, _f_pols[f_pol_i]);

				if(min < 0.0f)
					continue;

				if(min_dist < 0.0f || min_dist > min)
				{
					result.clear();
					min_dist = min;
					result.push_back(cur_point);
				}
				else if(Math::floats_are_equal(min_dist, min))
				{
					if(point_already_counted(cur_point))
						continue;
					result.push_back(cur_point);
				}
			}
		}
	}

	return result;
}


Geometry::Simple_Intersection_Data SAT_Narrowest_CD::intersection__polygon_vs_point(const Geometry::Polygon& _polygon, const glm::vec3& _point) const
{
	Geometry_2D::Equasion_Data AB_eq(_polygon[0], _polygon[1]);
	Geometry_2D::Equasion_Data BC_eq(_polygon[1], _polygon[2]);
	Geometry_2D::Equasion_Data CA_eq(_polygon[2], _polygon[0]);

	float AB_y_proj = AB_eq.solve_by_x(_point.x);
	float BC_y_proj = BC_eq.solve_by_x(_point.x);
	float CA_y_proj = CA_eq.solve_by_x(_point.x);

	bool AB_right_side = AB_eq.is_vertical() ? ( _polygon[1].y < _polygon[0].y ? _point.x >= _polygon[0].x : _point.x <= _polygon[0].x ) : ( AB_eq.goes_left() ? AB_y_proj > _point.y : AB_y_proj < _point.y );
	bool BC_right_side = BC_eq.is_vertical() ? ( _polygon[2].y < _polygon[1].y ? _point.x >= _polygon[1].x : _point.x <= _polygon[1].x ) : ( BC_eq.goes_left() ? BC_y_proj > _point.y : BC_y_proj < _point.y );
	bool CA_right_side = CA_eq.is_vertical() ? ( _polygon[0].y < _polygon[2].y ? _point.x >= _polygon[2].x : _point.x <= _polygon[2].x ) : ( CA_eq.goes_left() ? CA_y_proj > _point.y : CA_y_proj < _point.y );

	if (AB_right_side && BC_right_side && CA_right_side)
		return Geometry::Simple_Intersection_Data(Geometry::Simple_Intersection_Data::Type::intersection, _point);
	return Geometry::Simple_Intersection_Data();
}



Geometry::Simple_Intersection_Data SAT_Narrowest_CD::collision__model_vs_point(const Physical_Model_2D &_model, const glm::vec3 &_point) const
{
	for (unsigned int i = 0; i < _model.get_polygons_count(); ++i)
	{
		Geometry::Simple_Intersection_Data id = intersection__polygon_vs_point(_model[i], _point);
		if(id)
			return id;
	}
	return Geometry::Simple_Intersection_Data();
}


Physical_Model_2D::Intersection_Data SAT_Narrowest_CD::collision__model_vs_model(const Geometry::Polygon* _pols_1, unsigned int _pols_amount_1, const Geometry::Polygon* _pols_2, unsigned int _pols_amount_2) const
{
	Physical_Model_2D::Intersection_Data result(Physical_Model_2D::Intersection_Data::Type::intersection);

	Intersection_Data f_id;
	for(unsigned int i=0; i<_pols_amount_1; ++i)
	{
		for(unsigned int j=0; j<_pols_amount_2; ++j)
		{
			Intersection_Data id = M_polygons_collision(_pols_1[i], _pols_2[j]);

			if(!id.intersection)
				continue;

			if(f_id.min_dist < 0.0f || f_id.min_dist > id.min_dist)
				f_id = id;
		}
	}

	if(!f_id.intersection)
		return {};

	Intersection_Data s_id;
	for(unsigned int i=0; i<_pols_amount_2; ++i)
	{
		for(unsigned int j=0; j<_pols_amount_1; ++j)
		{
			Intersection_Data id = M_polygons_collision(_pols_2[i], _pols_1[j]);

			if(!id.intersection)
				continue;

			if(s_id.min_dist < 0.0f || s_id.min_dist > id.min_dist)
				s_id = id;
		}
	}

	if(!s_id.intersection)
		return {};

	if(s_id.min_dist < f_id.min_dist)
		f_id = s_id;

	result.normal = f_id.min_dist_axis;
	LEti::Math::shrink_vector_to_1(result.normal);

	result.points = M_points_of_contact(_pols_1, _pols_amount_1, _pols_2, _pols_amount_2);
	if(result.points.size() == 0)
		return {};

	return result;
}






























































