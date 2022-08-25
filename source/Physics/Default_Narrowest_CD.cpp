#include "../../include/Physics/Default_Narrowest_CD.h"

using namespace LEti;


Default_Narrowest_CD::Default_Narrowest_CD()
{

}

Default_Narrowest_CD::~Default_Narrowest_CD()
{

}



Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_segment(const Physical_Model_2D& _model, const glm::vec3& _seg_start, const glm::vec3& _seg_end) const
{
	Geometry::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _model.get_polygons_count(); ++i)
	{
		Geometry::Intersection_Data id = _model[i].segment_intersecting_polygon(_seg_start, _seg_end);
		if (id)
		{
			++found_intersections;
			result_id.first_normal += id.first_normal;
			result_id.second_normal += id.second_normal;
			result_id.point += id.point;
		}
	}

	if(found_intersections == 0)
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);

	result_id.type = Geometry::Intersection_Data::Type::intersection;
	result_id.point /= (float)found_intersections;
	LEti::Math::shrink_vector_to_1(result_id.first_normal);
	LEti::Math::shrink_vector_to_1(result_id.second_normal);

	return result_id;
}



Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const
{
	for (unsigned int i = 0; i < _model.get_polygons_count(); ++i)
		if (_model[i].point_belongs_to_triangle(_point))
			return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::intersection);
	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}


Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D& _2) const
{
	Geometry::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _1[i].intersects_with_another_polygon(_2[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _2[i].intersects_with_another_polygon(_1[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D::Imprint& _2) const
{
	Geometry::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _1[i].intersects_with_another_polygon(_2[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _2[i].intersects_with_another_polygon(_1[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D& _2) const
{
	Geometry::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _1[i].intersects_with_another_polygon(_2[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _2[i].intersects_with_another_polygon(_1[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}

Geometry::Intersection_Data Default_Narrowest_CD::collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D::Imprint& _2) const
{
	Geometry::Intersection_Data result_id;
	unsigned int found_intersections = 0;

	for (unsigned int i = 0; i < _1.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _2.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _1[i].intersects_with_another_polygon(_2[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	for (unsigned int i = 0; i < _2.get_polygons_count(); ++i)
	{
		for (unsigned int j = 0; j < _1.get_polygons_count(); ++j)
		{
			Geometry::Intersection_Data id = _2[i].intersects_with_another_polygon(_1[j]);
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
		result_id.type = Geometry::Intersection_Data::Type::intersection;
		result_id.point /= (float)found_intersections;
		LEti::Math::shrink_vector_to_1(result_id.first_normal);
		LEti::Math::shrink_vector_to_1(result_id.second_normal);

		return result_id;
	}

	return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);
}


