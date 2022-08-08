#include "../../include/Physics/Default_Narrow_CD.h"

using namespace LEti;



Default_Narrow_CD::float_pair Default_Narrow_CD::find_ratio(const Object_2D &_moving_1, const Object_2D &_moving_2, Axis_To_Use _axis) const
{
	const Physical_Model_2D::Rectangular_Border& prv_rb_1 = _moving_1.get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& cur_rb_1 = _moving_1.get_physical_model()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& prv_rb_2 = _moving_2.get_physical_model_prev_state()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& cur_rb_2 = _moving_2.get_physical_model()->curr_rect_border();

	float_pair f;
	float_pair s;
	if(_axis == Axis_To_Use::X)
	{
		f = {(prv_rb_1.right + prv_rb_1.left) / 2.0f, (cur_rb_1.right + cur_rb_1.left) / 2.0f};
		s = {(prv_rb_2.right + prv_rb_2.left) / 2.0f, (cur_rb_2.right + cur_rb_2.left) / 2.0f};
	}
	else
	{
		f = {(prv_rb_1.top + prv_rb_1.bottom) / 2.0f, (cur_rb_1.top + cur_rb_1.bottom) / 2.0f};
		s = {(prv_rb_2.top + prv_rb_2.bottom) / 2.0f, (cur_rb_2.top + cur_rb_2.bottom) / 2.0f};
	}

	float f_w = f.first > f.second ? f.first - f.second : f.second - f.first;
	float f_ofs = (prv_rb_1.right - prv_rb_1.left) / 2.0f;
	float s_w = s.first > s.second ? s.first - s.second : s.second - s.first;
	float s_ofs = (prv_rb_2.right - prv_rb_2.left) / 2.0f;

	float dofs = f_ofs + s_ofs;

	auto inbounds = [](float _left, float _right, float _num)->bool
	{
		if(_left > _right)
		{
			float temp = _right;
			_right = _left;
			_left = temp;
		}

		return _num <= _right && _num >= _left;
	};

	bool f_inbounds = inbounds(f.first, f.second, s.first) && inbounds(f.first, f.second, s.second);
	bool s_inbounds = inbounds(s.first, s.second, f.first) && inbounds(s.first, s.second, f.second);

	if(f_inbounds || s_inbounds) //	one object's trajectory completely include other's
	{
		auto calculate_ratio = [](float _iw, const float_pair& _outside, float _ow, float _dofs)->float_pair
		{
			float ratio_to = _iw / 2.0f;
			if(_outside.first > _outside.second) ratio_to = _ow - ratio_to;
			return { (ratio_to + _dofs) / _ow, (ratio_to - _dofs) / _ow };
		};

		if(f_inbounds)
			return calculate_ratio(f_w, s, s_w, dofs);
		if(s_inbounds)
			return calculate_ratio(s_w, f, f_w, dofs);
	}
	else if(inbounds(f.first, f.second, s.first) || inbounds(f.first, f.second, s.second)) //	trajectories have common part
	{
		float max_ratio = -0.1f;
		float min_ratio = 1.1f;

		bool first_on_left = f.first < s.first;

		if(first_on_left)
		{
			min_ratio = (s.first - f.first - dofs) / (f_w + s_w);
			max_ratio = (s.first - f.first + dofs) / (f_w + s_w);
		}
		else
		{
			min_ratio = (f.first - s.first - dofs) / (f_w + s_w);
			max_ratio = (f.first - s.first + dofs) / (f_w + s_w);
		}
		return { min_ratio, max_ratio };

	}
	else if(!inbounds(f.first, f.second, s.first) && !inbounds(f.first, f.second, s.second)) //	trajectories does not have commot part, but objects may still intersect
	{
		float start_dist = f.first > s.first ? f.first - s.first : s.first - f.first;
		float start1_end2_dist = f.first > s.second ? f.first - s.second : s.first - f.second;
		float start2_end1_dist = s.first > f.second ? s.first - f.second : f.first - s.second;
		float end_dist = f.second > s.second ? f.second - s.second : s.second - f.second;

		auto calculate_ratio = [&](float _dist)->float
		{
			float dist_sum = f_w + s_w;
			_dist = dofs - _dist;
			float dist_ratio = (_dist / dist_sum) * f_w;
			return 1 - (dist_ratio / f_w);
		};

		float min = 1.1f, max = -0.1f;
		auto rewrite = [&min, &max](float _ratio)->void
		{
			if(_ratio <= 1.0f && _ratio >= 0.0f)
			{
				if(min > _ratio)
					min = _ratio;
				if(max < _ratio)
					max = _ratio;
			}
		};

		if(start_dist < dofs)
		{
			float ratio = calculate_ratio(start_dist);
			rewrite(ratio);
		}
		if(start1_end2_dist < dofs)
		{
			float ratio = calculate_ratio(start1_end2_dist);
			rewrite(ratio);
		}
		if(start2_end1_dist < dofs)
		{
			float ratio = calculate_ratio(start2_end1_dist);
			rewrite(ratio);
		}
		if(end_dist < dofs)
		{
			float ratio = calculate_ratio(end_dist);
			rewrite(ratio);
		}

		return { min, max };
	}

	return { -0.1f, 1.1f };
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
			{
				min_intersection_ratio = _ratio;
				std::cout << "rewriting min\n";
			}
			if(max_intersection_ratio < _ratio)
			{
				max_intersection_ratio = _ratio;
				std::cout << "rewriting max\n";
			}
		}
	};

	float_pair rx = find_ratio(_moving_1, _moving_2, Axis_To_Use::X);
	float_pair ry = find_ratio(_moving_1, _moving_2, Axis_To_Use::Y);

	if(rx.first > rx.second)
		std::swap(rx.first, rx.second);
	if(ry.first > ry.second)
		std::swap(ry.first, ry.second);

	auto ratio_is_valid = [](float _ratio)->bool
	{
		return _ratio <= 1.0f && _ratio >= 0.0f;
	};

	bool rx_valid = ratio_is_valid(rx.first) && ratio_is_valid(rx.second);
	bool ry_valid = ratio_is_valid(ry.first) && ratio_is_valid(ry.second);

	if(!rx_valid)
	{
		rewrite_min_max_ratio(ry.first);
		rewrite_min_max_ratio(ry.second);
	}
	else if(!ry_valid)
	{
		rewrite_min_max_ratio(rx.first);
		rewrite_min_max_ratio(rx.second);
	}
	else if(rx.second - rx.first < ry.second - ry.first)
	{
		rewrite_min_max_ratio(rx.first);
		rewrite_min_max_ratio(rx.second);
	}
	else
	{
		rewrite_min_max_ratio(ry.first);
		rewrite_min_max_ratio(ry.second);
	}

	if((min_intersection_ratio <= 1.0f && min_intersection_ratio >= 0.0f && max_intersection_ratio <= 1.0f && max_intersection_ratio >= 0.0f))
	{
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
	auto rewrite_min_max_ratio = [&min_intersection_ratio, &max_intersection_ratio](float _new)->void
	{
		if(min_intersection_ratio > _new)
			min_intersection_ratio = _new;
		if(max_intersection_ratio < _new)
			max_intersection_ratio = _new;
	};

	Physical_Model_2D::Rectangular_Border shared_space = _moving.get_dynamic_rb() && _static.get_physical_model()->curr_rect_border();

	bool first_on_left = _moving.get_dynamic_rb().left < _static.get_physical_model()->curr_rect_border().left;
	const Physical_Model_2D::Rectangular_Border& on_left = first_on_left ? _moving.get_dynamic_rb() : _static.get_physical_model()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& on_right = first_on_left ? _static.get_physical_model()->curr_rect_border() : _moving.get_dynamic_rb();
	bool first_on_bottom = _moving.get_dynamic_rb().bottom < _static.get_physical_model()->curr_rect_border().bottom;
	const Physical_Model_2D::Rectangular_Border& on_bottom = first_on_bottom ? _moving.get_dynamic_rb() : _static.get_physical_model()->curr_rect_border();
	const Physical_Model_2D::Rectangular_Border& on_top = first_on_bottom ? _static.get_physical_model()->curr_rect_border() : _moving.get_dynamic_rb();

	float on_left_width = on_left.right - on_left.left;
	float on_right_width = on_right.right - on_right.left;
	float on_top_height = on_top.top - on_top.bottom;
	float on_bottom_height = on_bottom.top - on_bottom.bottom;

	float tr_x_1 = (shared_space.left - on_left.left) / on_left_width;
	float tr_x_2 = (on_right.right - shared_space.right) / on_right_width;
	float tr_x_3 = (on_left.right - shared_space.left) / on_left_width;
	float tr_x_4 = (shared_space.right - on_right.left) / on_right_width;

	float tr_y_1 = (shared_space.bottom - on_bottom.bottom) / on_bottom_height;
	float tr_y_2 = (on_top.top - shared_space.top) / on_top_height;
	float tr_y_3 = (on_bottom.top - shared_space.bottom) / on_bottom_height;
	float tr_y_4 = (shared_space.top - on_top.bottom) / on_top_height;

	bool fully_inside_x = (shared_space.left <= on_left.left && shared_space.right >= on_left.right)
			|| (shared_space.left <= on_right.left && shared_space.right >= on_right.right);
	bool fully_inside_y = (shared_space.bottom <= on_left.bottom && shared_space.top >= on_left.top)
			|| (shared_space.bottom <= on_right.bottom && shared_space.top >= on_right.top);

	if(fully_inside_x && fully_inside_y)
	{
		min_intersection_ratio = 0.0f;
		max_intersection_ratio = 1.0f;
	}
	else if(fully_inside_x)
	{
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}
	else if(fully_inside_y)
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
	}
	else
	{
		rewrite_min_max_ratio(tr_x_1);
		rewrite_min_max_ratio(tr_x_2);
		rewrite_min_max_ratio(tr_x_3);
		rewrite_min_max_ratio(tr_x_4);
		rewrite_min_max_ratio(tr_y_1);
		rewrite_min_max_ratio(tr_y_2);
		rewrite_min_max_ratio(tr_y_3);
		rewrite_min_max_ratio(tr_y_4);
	}

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
