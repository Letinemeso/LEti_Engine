#include "../../include/Physics/Default_Narrowest_CD.h"

using namespace LEti;


Default_Narrowest_CD::Default_Narrowest_CD()
{

}

Default_Narrowest_CD::~Default_Narrowest_CD()
{

}



Geometry::Simple_Intersection_Data Default_Narrowest_CD::intersection__polygon_vs_point(const Geometry::Polygon& _polygon, const glm::vec3& _point) const
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


Physical_Model_2D::Intersection_Data Default_Narrowest_CD::intersection__polygon_vs_segment(const Geometry::Polygon& _polygon, const Geometry::Segment& _segment) const
{
	unsigned int found_intersections = 0;

	Physical_Model_2D::Intersection_Data result_id;
	Geometry::Simple_Intersection_Data ids[3];

	ids[0] = Geometry_2D::segments_intersect({_polygon[0], _polygon[1]}, _segment);
	ids[1] = Geometry_2D::segments_intersect({_polygon[1], _polygon[2]}, _segment);
	ids[2] = Geometry_2D::segments_intersect({_polygon[2], _polygon[0]}, _segment);
	for(unsigned int i=0; i<3; ++i)
	{
		if(ids[i])
		{
			++found_intersections;
			std::pair<glm::vec3, glm::vec3> normals = Geometry::get_segments_normals(ids[i].first, ids[i].second);
			LOG("temp_log_level", "1: " + std::to_string(normals.first.x) + ", " + std::to_string(normals.first.y) + "\t2: " + std::to_string(normals.second.x) + ", " + std::to_string(normals.second.y));
			result_id.first_normal += normals.first;
			result_id.second_normal += normals.second;
			result_id.point += ids[i].point;
		}
	}

	if(found_intersections == 0)
		return Physical_Model_2D::Intersection_Data();

	result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
	LEti::Math::shrink_vector_to_1(result_id.first_normal);
	LEti::Math::shrink_vector_to_1(result_id.second_normal);
	result_id.point /= (float)found_intersections;

	return result_id;
}

Physical_Model_2D::Intersection_Data Default_Narrowest_CD::intersection__polygon_vs_polygon(const Geometry::Polygon& _first, const Geometry::Polygon& _second) const
{
	Geometry_2D::Rectangular_Border this_rb;
	this_rb.consider_point(_first[0]).consider_point(_first[1]).consider_point(_first[2]);
	Geometry_2D::Rectangular_Border other_rb;
	other_rb.consider_point(_second[0]).consider_point(_second[1]).consider_point(_second[2]);

	if(!(this_rb && other_rb))
		return Physical_Model_2D::Intersection_Data();

	unsigned int found_intersections = 0;
	Physical_Model_2D::Intersection_Data result_id;
	Physical_Model_2D::Intersection_Data ids[3];

	ids[0] = intersection__polygon_vs_segment(_first, {_second[0], _second[1]});
	ids[1] = intersection__polygon_vs_segment(_first, {_second[1], _second[2]});
	ids[2] = intersection__polygon_vs_segment(_first, {_second[2], _second[0]});
	for(unsigned int i=0; i<3; ++i)
	{
		if(ids[i])
		{
			++found_intersections;
			result_id.first_normal += ids[i].first_normal;
			result_id.second_normal += ids[i].second_normal;
			result_id.point += ids[i].point;
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);
		result_id.point /= (float)found_intersections;

		return result_id;
	}

	Geometry::Simple_Intersection_Data _3 = intersection__polygon_vs_point(_first, _second[0]);
	Geometry::Simple_Intersection_Data _4 = intersection__polygon_vs_point(_first, _second[1]);
	Geometry::Simple_Intersection_Data _5 = intersection__polygon_vs_point(_first, _second[2]);
	if(_3 && _4 && _5)
		return Physical_Model_2D::Intersection_Data(Physical_Model_2D::Intersection_Data::Type::intersection, glm::vec3(_3+_4+_5)/3.0f);
	else
		{ASSERT(_3 || _4 || _5);}
	return Physical_Model_2D::Intersection_Data();
}

Physical_Model_2D::Intersection_Data Default_Narrowest_CD::collision__model_vs_segment(const Physical_Model_2D& _model, const Geometry::Segment& _segment) const
{
	Physical_Model_2D::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _model.get_polygons_count(); ++i)
	{
		Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_segment(_model[i], _segment);
		if (id)
		{
			++found_intersections;
			result_id.first_normal += id.first_normal;
			result_id.second_normal += id.second_normal;
			result_id.point += id.point;
		}
	}

	if(found_intersections == 0)
		return Physical_Model_2D::Intersection_Data(Physical_Model_2D::Intersection_Data::Type::none);

	result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
	result_id.point /= (float)found_intersections;
	LEti::Math::shrink_vector_to_1(result_id.first_normal);
	LEti::Math::shrink_vector_to_1(result_id.second_normal);

	return result_id;
}



Geometry::Simple_Intersection_Data Default_Narrowest_CD::collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const
{
	for (unsigned int i = 0; i < _model.get_polygons_count(); ++i)
	{
		Geometry::Simple_Intersection_Data id = intersection__polygon_vs_point(_model[i], _point);
		if(id)
			return id;
	}
	return Geometry::Simple_Intersection_Data();
}


Physical_Model_2D::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D& _2) const
{
	Physical_Model_2D::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[i], _2[j]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[j], _2[i]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	return Physical_Model_2D::Intersection_Data();
}

Physical_Model_2D::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D::Imprint& _2) const
{
	Physical_Model_2D::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[i], _2[j]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[j], _2[i]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	return Physical_Model_2D::Intersection_Data();
}

Physical_Model_2D::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D& _2) const
{
	Physical_Model_2D::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[i], _2[j]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[j], _2[i]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	return Physical_Model_2D::Intersection_Data();
}

Physical_Model_2D::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D::Imprint& _2) const
{
	Physical_Model_2D::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[i], _2[j]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Physical_Model_2D::Intersection_Data id = intersection__polygon_vs_polygon(_1[j], _2[i]);
			if (id)
			{
				++found_intersections;
				result_id.first_normal += id.first_normal;
				result_id.second_normal += id.second_normal;
				result_id.point += id.point;
			}
		}
	}

	if(found_intersections != 0)
	{
		result_id.type = Physical_Model_2D::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		if(Math::vector_length(result_id.first_normal) < 0.01f)
			result_id.first_normal = -result_id.second_normal;
		if(Math::vector_length(result_id.second_normal) < 0.01f)
			result_id.second_normal = -result_id.first_normal;

		return result_id;
	}

	return Physical_Model_2D::Intersection_Data();
}


