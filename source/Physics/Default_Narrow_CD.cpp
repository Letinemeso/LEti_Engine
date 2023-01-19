#include "../../include/Physics/Default_Narrow_CD.h"

using namespace LEti;



Default_Narrow_CD::float_pair Default_Narrow_CD::find_ratio(const LEti::Object_2D &_moving_1, const LEti::Object_2D &_moving_2) const
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

	using Segment = Geometry::Segment;

	auto get_polygon_Segment = [](const Geometry::Polygon& _pol, unsigned int _ind)->Segment
	{
		L_ASSERT(!(_ind > 2));

		if(_ind < 2)
			return {_pol[_ind], _pol[_ind + 1]};
		else
			return {_pol[_ind], _pol[0]};
	};

	Physical_Model_2D::Imprint ppm1_relative_prev = *_moving_1.physics_module()->get_physical_model_prev_state();
	Physical_Model_2D::Imprint ppm1_relative = ppm1_relative_prev;

	glm::mat4x4 fake_default_matrix{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 pos_diff_vector_prev = _moving_1.get_pos_prev() - _moving_2.get_pos_prev();
	pos_diff_vector_prev = _moving_2.get_rotation_matrix_inversed_for_time_ratio(0.0f) * glm::vec4(pos_diff_vector_prev, 1.0f);

	glm::mat4x4 diff_pos_prev = fake_default_matrix;
	diff_pos_prev[3][0] += pos_diff_vector_prev[0];
	diff_pos_prev[3][1] += pos_diff_vector_prev[1];
	glm::mat4x4 diff_rotation_prev = _moving_1.get_rotation_matrix_for_time_ratio(0.0f) / _moving_2.get_rotation_matrix_for_time_ratio(0.0f);
	glm::mat4x4 diff_scale_prev = _moving_1.get_scale_matrix_for_time_ratio(0.0f);


	glm::vec3 pos_diff_vector = _moving_1.get_pos() - _moving_2.get_pos();
	pos_diff_vector = _moving_2.get_rotation_matrix_inversed_for_time_ratio(1.0f) * glm::vec4(pos_diff_vector, 1.0f);

	glm::mat4x4 diff_pos = fake_default_matrix;
	diff_pos[3][0] += pos_diff_vector[0];
	diff_pos[3][1] += pos_diff_vector[1];
	glm::mat4x4 diff_rotation = _moving_1.get_rotation_matrix_for_time_ratio(1.0f) / _moving_2.get_rotation_matrix_for_time_ratio(1.0f);

	glm::vec3 m2_scale_diff_vec = _moving_2.get_scale() - _moving_2.get_scale_prev();
	glm::mat4x4 m2_scale_diff_matrix = fake_default_matrix;
	for(unsigned int i=0; i<3; ++i)
		m2_scale_diff_matrix[i][i] += m2_scale_diff_vec[i];

	glm::mat4x4 diff_scale = _moving_1.get_scale_matrix_for_time_ratio(1.0f) * m2_scale_diff_matrix;

	ppm1_relative_prev.update(diff_pos_prev, diff_rotation_prev, diff_scale_prev);
	ppm1_relative.update(diff_pos, diff_rotation, diff_scale);

	Physical_Model_2D::Imprint ppm2 = *_moving_2.physics_module()->get_physical_model_prev_state();
	ppm2.update(fake_default_matrix, fake_default_matrix, _moving_2.get_scale_matrix_for_time_ratio(0.0f));

	for(unsigned int pol1=0; pol1<ppm1_relative_prev.get_polygons_count(); ++pol1)
	{
		for(unsigned int seg1 = 0; seg1 < 3; ++seg1)
		{
			Segment trajectory(ppm1_relative_prev[pol1][seg1], ppm1_relative[pol1][seg1]);
			float distance = Math::get_distance(trajectory.start, trajectory.end);

			for(unsigned int pol2 = 0; pol2 < ppm2.get_polygons_count(); ++pol2)
			{
				for(unsigned int seg2 = 0; seg2 < 3; ++seg2)
				{
					Segment seg = get_polygon_Segment(ppm2[pol2], seg2);
					Geometry::Simple_Intersection_Data id = Geometry_2D::segments_intersect({trajectory.start, trajectory.end}, {seg.start, seg.end});
					if(id)
					{
						float ratio = Math::get_distance(trajectory.start, id.point) / distance;
						rewrite_min_max_ratio(ratio);
					}
				}
			}
		}
	}

	return {min_intersection_ratio, max_intersection_ratio};
}

Physical_Model_2D::Intersection_Data Default_Narrow_CD::get_precise_time_ratio_of_collision(const LEti::Object_2D& _first, const LEti::Object_2D& _second, float _min_ratio, float _max_ratio) const
{
	L_ASSERT(!(!_first.physics_module()->can_collide() || !_second.physics_module()->can_collide()));
	L_ASSERT(!(!_first.physics_module()->is_dynamic() && !_second.physics_module()->is_dynamic()));
	L_ASSERT(!(_min_ratio < 0.0f || _max_ratio < 0.0f || _min_ratio > 1.0f || _max_ratio > 1.0f));

	float diff = _max_ratio - _min_ratio;
	float step_diff = diff / (float)m_precision;

	Physical_Model_2D::Intersection_Data id;

	float curr_time_point = _min_ratio;
	while(curr_time_point <= _max_ratio)
	{
		Physical_Model_2D::Imprint first_impr = *_first.physics_module()->get_physical_model_prev_state();
		first_impr.update(_first.get_translation_matrix_for_time_ratio(curr_time_point), _first.get_rotation_matrix_for_time_ratio(curr_time_point), _first.get_scale_matrix_for_time_ratio(curr_time_point));
		Physical_Model_2D::Imprint second_impr = *_second.physics_module()->get_physical_model_prev_state();
		second_impr.update(_second.get_translation_matrix_for_time_ratio(curr_time_point), _second.get_rotation_matrix_for_time_ratio(curr_time_point), _second.get_scale_matrix_for_time_ratio(curr_time_point));
		id = m_narrowest_phase->collision__model_vs_model(first_impr, second_impr);
		if(id) break;

		curr_time_point += step_diff;
	}

	if(!id && Math::floats_are_equal(_max_ratio, 1.0f))
		id = m_narrowest_phase->collision__model_vs_model(*_first.physics_module()->get_physical_model(), *_second.physics_module()->get_physical_model());

	if(id)
	{
		id.time_of_intersection_ratio = curr_time_point - step_diff;
		if(id.time_of_intersection_ratio < 0.0f) id.time_of_intersection_ratio = 0.0f;
	}
	return id;
}

Physical_Model_2D::Intersection_Data Default_Narrow_CD::collision__moving_vs_moving(const LEti::Object_2D &_moving_1, const LEti::Object_2D &_moving_2) const
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

	return Physical_Model_2D::Intersection_Data();
}

Physical_Model_2D::Intersection_Data Default_Narrow_CD::collision__moving_vs_static(const LEti::Object_2D& _moving, const LEti::Object_2D& _static) const
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

	return Physical_Model_2D::Intersection_Data();
}

Geometry::Simple_Intersection_Data Default_Narrow_CD::collision__static_vs_point(const LEti::Object_2D &_static, const glm::vec3 &_point) const
{
	return m_narrowest_phase->collision__model_vs_point(*_static.physics_module()->get_physical_model(), _point);
}



Physical_Model_2D::Intersection_Data Default_Narrow_CD::objects_collide(const LEti::Object_2D& _first, const LEti::Object_2D& _second) const
{
	L_ASSERT(!(!_first.physics_module() || !_second.physics_module()));

	if(!_second.physics_module()->can_collide() || !_first.physics_module()->can_collide())
		return Physical_Model_2D::Intersection_Data();

	Physical_Model_2D::Intersection_Data result_id;

	if(!_second.physics_module()->is_dynamic() && !_first.physics_module()->is_dynamic())
	{
		return m_narrowest_phase->collision__model_vs_model(*_first.physics_module()->get_physical_model(), *_second.physics_module()->get_physical_model());
	}
	else if(_first.physics_module()->is_dynamic() ^ _second.physics_module()->is_dynamic())
	{
		const LEti::Object_2D& dynamic_object = _first.physics_module()->is_dynamic() ? _first : _second;
		const LEti::Object_2D& static_object = _first.physics_module()->is_dynamic() ? _second : _first;
		if((dynamic_object.physics_module()->get_dynamic_rb() && static_object.physics_module()->get_physical_model()->curr_rect_border()))
			return Physical_Model_2D::Intersection_Data();

		Physical_Model_2D::Intersection_Data prev_state_cd = m_narrowest_phase->collision__model_vs_model(*dynamic_object.physics_module()->get_physical_model_prev_state(), *static_object.physics_module()->get_physical_model());
		if(prev_state_cd)
			return prev_state_cd;

		if(dynamic_object.moved_since_last_frame())
			return collision__moving_vs_static(dynamic_object, static_object);

		return m_narrowest_phase->collision__model_vs_model(*_first.physics_module()->get_physical_model(), *_second.physics_module()->get_physical_model());
	}
	else /*if(is_dynamic() && _other.is_dynamic())*/
	{
		if(!(_first.physics_module()->get_dynamic_rb() && _second.physics_module()->get_dynamic_rb()))
			return Physical_Model_2D::Intersection_Data();

		Physical_Model_2D::Intersection_Data prev_state_cd = m_narrowest_phase->collision__model_vs_model(*_first.physics_module()->get_physical_model_prev_state(), *_second.physics_module()->get_physical_model_prev_state());
		if(prev_state_cd)
			return prev_state_cd;

		if(_first.moved_since_last_frame() || _second.moved_since_last_frame())
			return collision__moving_vs_moving(_first, _second);
		else
			return m_narrowest_phase->collision__model_vs_model(*_first.physics_module()->get_physical_model(), *_second.physics_module()->get_physical_model());
	}
	return Physical_Model_2D::Intersection_Data();
}



void Default_Narrow_CD::update(const Broad_Phase_Interface::Colliding_Pair_List &_possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List &_possible_collisions__points)
{
	m_collisions__models.clear();
	m_collisions__points.clear();

	Broad_Phase_Interface::Colliding_Pair_List::const_iterator itm = _possible_collisions__models.cbegin();
	while(itm != _possible_collisions__models.cend())
	{
		Physical_Model_2D::Intersection_Data id = objects_collide(*itm->first, *itm->second);
		if(id)
		{
			id.first = itm->first;
			id.second = itm->second;
			m_collisions__models.push_back(id);
		}

		++itm;
	}

	Broad_Phase_Interface::Colliding_Point_And_Object_List::const_iterator itp = _possible_collisions__points.cbegin();
	while(itp != _possible_collisions__points.cend())
	{
		Geometry::Simple_Intersection_Data id = collision__static_vs_point(*itp->object, *itp->point);
		if(id)
		{
			Physical_Model_2D::Intersection_Data result(Physical_Model_2D::Intersection_Data::Type::intersection);
			result.first = itp->object;
			result.second = nullptr;
			result.point = *itp->point;
			m_collisions__points.push_back(result);
		}

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
