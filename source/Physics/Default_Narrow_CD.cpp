#include "../../include/Physics/Default_Narrow_CD.h"

using namespace LEti;



Default_Narrow_CD::float_pair Default_Narrow_CD::find_ratio(const Object_2D &_moving_1, const Object_2D &_moving_2) const
{
	float min_intersection_ratio = 1.1f, max_intersection_ratio = -0.1f;
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _ratio)->void
	{
		if(_ratio <= 1.0f && _ratio >= 0.0f)
		{
			if(min_intersection_ratio > _ratio)
				min_intersection_ratio = _ratio;
			if(max_intersection_ratio < _ratio)
				max_intersection_ratio = _ratio;
		}
	};

	using segment = std::pair<glm::vec3, glm::vec3>;

	auto get_polygon_segment = [](const Geometry_2D::Polygon& _pol, unsigned int _ind)->segment
	{
		ASSERT(_ind > 2);

		segment result;
		if(_ind < 2)
		{
			result.first = _pol[_ind];
			result.second = _pol[_ind + 1];
		}
		else
		{
			result.first = _pol[_ind];
			result.second = _pol[0];
		}
		return result;
	};

	Physical_Model_2D::Imprint ppm1 = *_moving_1.get_physical_model_prev_state();
	Physical_Model_2D::Imprint ppm1_relative = ppm1;

	glm::mat4x4 fake_default_matrix{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 pos_diff_vector_prev = _moving_2.get_pos_prev() - _moving_1.get_pos_prev();
	pos_diff_vector_prev = _moving_2.get_rotation_matrix_inversed_for_time_ratio(0.0f) * glm::vec4(pos_diff_vector_prev, 1.0f);

	glm::mat4x4 diff_pos_prev = fake_default_matrix;
	diff_pos_prev[3][0] += pos_diff_vector_prev[0];
	diff_pos_prev[3][1] += pos_diff_vector_prev[1];
	glm::mat4x4 diff_rotation_prev = _moving_1.get_rotation_matrix_for_time_ratio(0.0f) / _moving_2.get_rotation_matrix_for_time_ratio(0.0f);
	glm::mat4x4 diff_scale_prev = _moving_1.get_scale_matrix_for_time_ratio(0.0f);


	glm::vec3 pos_diff_vector = _moving_2.get_pos() - _moving_1.get_pos();
	pos_diff_vector = _moving_2.get_rotation_matrix_inversed_for_time_ratio(1.0f) * glm::vec4(pos_diff_vector, 1.0f);

	glm::mat4x4 diff_pos = fake_default_matrix;
	diff_pos[3][0] += pos_diff_vector[0];
	diff_pos[3][1] += pos_diff_vector[1];
	glm::mat4x4 diff_rotation = _moving_1.get_rotation_matrix_for_time_ratio(1.0f) / _moving_2.get_rotation_matrix_for_time_ratio(1.0f);
	glm::mat4x4 diff_scale = _moving_1.get_scale_matrix_for_time_ratio(1.0f) * (_moving_2.get_scale_matrix_for_time_ratio(1.0f) / _moving_2.get_scale_matrix_for_time_ratio(0.0f));

	ppm1.update(diff_pos_prev, diff_rotation_prev, diff_scale_prev);
	ppm1_relative.update(diff_pos, diff_rotation, diff_scale);

	Physical_Model_2D::Imprint ppm2 = *_moving_2.get_physical_model_prev_state();
	ppm2.update(fake_default_matrix, fake_default_matrix, _moving_2.get_scale_matrix_for_time_ratio(0.0f));

	for(unsigned int pol1=0; pol1<ppm1.get_polygons_count(); ++pol1)
	{
		for(unsigned int seg1 = 0; seg1 < 3; ++seg1)
		{
			segment trajectory(ppm1[pol1][seg1], ppm1_relative[pol1][seg1]);
			float distance = Math::get_distance(trajectory.first, trajectory.second);

			for(unsigned int pol2 = 0; pol2 < ppm2.get_polygons_count(); ++pol2)
			{
				for(unsigned int seg2 = 0; seg2 < 3; ++seg2)
				{
					segment seg = get_polygon_segment(ppm2[pol2], seg2);
					Geometry::Intersection_Data id = Geometry_2D::segments_intersect(trajectory.first, trajectory.second, seg.first, seg.second);
					if(id)
					{
						float ratio = Math::get_distance(trajectory.first, id.point) / distance;
						rewrite_min_max_ratio(ratio);
					}
				}
			}
		}
	}

	return {min_intersection_ratio, max_intersection_ratio};
}

Geometry::Intersection_Data Default_Narrow_CD::get_precise_time_ratio_of_collision(const Object_2D& _first, const Object_2D& _second, float _min_ratio, float _max_ratio) const
{
	ASSERT(!_first.get_collision_possibility() || !_second.get_collision_possibility());
	ASSERT(!_first.is_dynamic() && !_second.is_dynamic());
	ASSERT(_min_ratio < 0.0f || _max_ratio < 0.0f || _min_ratio > 1.0f || _max_ratio > 1.0f);

	float diff = _max_ratio - _min_ratio;
	float step_diff = diff / (float)m_precision;

	Geometry::Intersection_Data id;

	float curr_time_point = _min_ratio;
	while(curr_time_point <= _max_ratio)
	{
		Physical_Model_2D::Imprint this_impr = *_first.get_physical_model_prev_state();
		this_impr.update(_first.get_translation_matrix_for_time_ratio(curr_time_point), _first.get_rotation_matrix_for_time_ratio(curr_time_point), _first.get_scale_matrix_for_time_ratio(curr_time_point));
		Physical_Model_2D::Imprint other_impr = *_second.get_physical_model_prev_state();
		other_impr.update(_second.get_translation_matrix_for_time_ratio(curr_time_point), _second.get_rotation_matrix_for_time_ratio(curr_time_point), _second.get_scale_matrix_for_time_ratio(curr_time_point));
		id = this_impr.imprints_intersect(other_impr);
		if(id) break;

		curr_time_point += step_diff;
	}

	if(!id && Math::floats_are_equal(_max_ratio, 1.0f))
		id = _first.get_physical_model()->is_intersecting_with_another_model(*_second.get_physical_model());

	if(id)
	{
		id.time_of_intersection_ratio = curr_time_point - step_diff;
		if(id.time_of_intersection_ratio < 0.0f) id.time_of_intersection_ratio = 0.0f;
	}

	return id;

	return Geometry::Intersection_Data();
}

Geometry::Intersection_Data Default_Narrow_CD::collision__moving_vs_moving(const Object_2D &_moving_1, const Object_2D &_moving_2) const
{
	float min_intersection_ratio = 1.1f, max_intersection_ratio = -0.1f;
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _ratio)->void
	{
		if(_ratio <= 1.0f && _ratio >= 0.0f)
		{
			if(min_intersection_ratio > _ratio)
				min_intersection_ratio = _ratio;
			if(max_intersection_ratio < _ratio)
				max_intersection_ratio = _ratio;
		}
	};

	float_pair potential_ratio = find_ratio(_moving_1, _moving_2);
	rewrite_min_max_ratio(potential_ratio.first);
	rewrite_min_max_ratio(potential_ratio.second);
	potential_ratio = find_ratio(_moving_2, _moving_1);
	rewrite_min_max_ratio(potential_ratio.first);
	rewrite_min_max_ratio(potential_ratio.second);

	if((min_intersection_ratio <= 1.0f && min_intersection_ratio >= 0.0f && max_intersection_ratio <= 1.0f && max_intersection_ratio >= 0.0f))
	{
		min_intersection_ratio -= 0.02f;
		max_intersection_ratio += 0.02f;

		if(min_intersection_ratio < 0.0f) min_intersection_ratio = 0.0f;
		if(max_intersection_ratio > 1.0f) max_intersection_ratio = 1.0f;

		if(Math::floats_are_equal(min_intersection_ratio, max_intersection_ratio))
			return get_precise_time_ratio_of_collision(_moving_1, _moving_2, min_intersection_ratio, 1.0f);
		else
			return get_precise_time_ratio_of_collision(_moving_1, _moving_2, min_intersection_ratio, max_intersection_ratio);
	}

	return Geometry::Intersection_Data();
}

Geometry::Intersection_Data Default_Narrow_CD::collision__moving_vs_static(const Object_2D& _moving, const Object_2D& _static) const
{
	float min_intersection_ratio = 1.1f, max_intersection_ratio = -0.1f;
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _ratio)->void
	{
		if(_ratio <= 1.0f && _ratio >= 0.0f)
		{
			if(min_intersection_ratio > _ratio)
				min_intersection_ratio = _ratio;
			if(max_intersection_ratio < _ratio)
				max_intersection_ratio = _ratio;
		}
	};

	float_pair potential_ratio = find_ratio(_moving, _static);
	rewrite_min_max_ratio(potential_ratio.first);
	rewrite_min_max_ratio(potential_ratio.second);
	potential_ratio = find_ratio(_static, _moving);
	rewrite_min_max_ratio(potential_ratio.first);
	rewrite_min_max_ratio(potential_ratio.second);

	if(min_intersection_ratio <= 1.0f && min_intersection_ratio >= 0.0f && max_intersection_ratio <= 1.0f && max_intersection_ratio >= 0.0f)
	{
		if(Math::floats_are_equal(min_intersection_ratio, max_intersection_ratio))
			return get_precise_time_ratio_of_collision(_moving, _static, min_intersection_ratio, 1.0f);
		else
			return get_precise_time_ratio_of_collision(_moving, _static, min_intersection_ratio, max_intersection_ratio);
	}

	return Geometry::Intersection_Data();
}

Geometry::Intersection_Data Default_Narrow_CD::collision__static_vs_point(const Object_2D &_static, const glm::vec3 &_point) const
{
	return _static.get_physical_model()->is_intersecting_with_point(_point);
}



Geometry::Intersection_Data Default_Narrow_CD::objects_collide(const Object_2D& _first, const Object_2D& _second) const
{
	ASSERT(_first.get_collision_possibility() && !_first.get_physical_model());
	ASSERT(_second.get_collision_possibility() && !_second.get_physical_model());

	if(!_second.get_collision_possibility() || !_first.get_collision_possibility())
		return Geometry::Intersection_Data(Geometry::Intersection_Data::Type::none);

	if(!_second.is_dynamic() && !_first.is_dynamic())
	{
		return _first.get_physical_model()->is_intersecting_with_another_model(*_second.get_physical_model());
	}
	else if(_first.is_dynamic() ^ _second.is_dynamic())
	{
		const Object_2D& dynamic_object = _first.is_dynamic() ? _first : _second;
		const Object_2D& static_object = _first.is_dynamic() ? _second : _first;
		Physical_Model_2D::Rectangular_Border check = dynamic_object.get_dynamic_rb() && static_object.get_physical_model()->curr_rect_border();
		if(check == Physical_Model_2D::Rectangular_Border())
			return Geometry::Intersection_Data();
		if(dynamic_object.moved_since_last_frame())
		{
			Geometry::Intersection_Data cd = collision__moving_vs_static(dynamic_object, static_object);
			if(cd)
				return cd;
		}
		return _first.get_physical_model()->is_intersecting_with_another_model(*_second.get_physical_model());
	}
	else /*if(is_dynamic() && _other.is_dynamic())*/
	{
		Physical_Model_2D::Rectangular_Border check = _first.get_dynamic_rb() && _second.get_dynamic_rb();
		if(check == Physical_Model_2D::Rectangular_Border())
			return Geometry::Intersection_Data();

		auto prev_state_cd = _first.get_physical_model_prev_state()->imprints_intersect(*_second.get_physical_model_prev_state());
		if(prev_state_cd)
		{
			return prev_state_cd;
		}

		if(_first.moved_since_last_frame() && _second.moved_since_last_frame())
		{
			Geometry::Intersection_Data cd = collision__moving_vs_moving(_first, _second);
			if(cd)
				return cd;
			else
				cd = collision__moving_vs_moving(_first, _second);
		}
		else if(_first.moved_since_last_frame() ^ _second.moved_since_last_frame())
		{
			const Object_2D& static_object = _first.moved_since_last_frame() ? _first : _second;
			const Object_2D& dynamic_object = _first.moved_since_last_frame() ? _second : _first;
			Geometry::Intersection_Data cd = collision__moving_vs_static(dynamic_object, static_object);
			if(cd)
				return cd;
		}
		else
		{
			return _first.get_physical_model()->is_intersecting_with_another_model(*_second.get_physical_model());
		}
	}
	return Geometry::Intersection_Data();
}



void Default_Narrow_CD::update(const Broad_Phase_Interface::Colliding_Pair_List &_possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List &_possible_collisions__points)
{
	m_collisions__models.clear();
	m_collisions__points.clear();

	Broad_Phase_Interface::Colliding_Pair_List::const_iterator itm = _possible_collisions__models.cbegin();
	while(itm != _possible_collisions__models.cend())
	{
		Collision_Data__Models cd(itm->first, itm->second);
		cd.collision_data = objects_collide(*cd.first, *cd.second);
		if(cd.collision_data)
			m_collisions__models.push_back(cd);

		++itm;
	}

	Broad_Phase_Interface::Colliding_Point_And_Object_List::const_iterator itp = _possible_collisions__points.cbegin();
	while(itp != _possible_collisions__points.cend())
	{
		Collision_Data__Points cd(itp->object, itp->point);
		cd.collision_data = collision__static_vs_point(*cd.object, *cd.point);
		if(cd.collision_data)
			m_collisions__points.push_back(cd);

		++itp;
	}
}


const Default_Narrow_CD::Collision_Data_List__Models& Default_Narrow_CD::get_collisions__models() const
{
	return m_collisions__models;
}

const Default_Narrow_CD::Collision_Data_List__Points& Default_Narrow_CD::get_collisions__points() const
{
	return m_collisions__points;
}


void Default_Narrow_CD::set_precision(unsigned int _precision)
{
	m_precision = _precision;
}
