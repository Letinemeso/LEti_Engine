#include "../include/Space_Splitter_2D.h"

using namespace LEti;


std::list<const Object_2D*> Space_Splitter_2D::m_registred_models;

unsigned int Space_Splitter_2D::m_max_tree_depth = 4;
LEti::Tree<Space_Splitter_2D::Area, 4> Space_Splitter_2D::m_quad_tree;

std::list<Space_Splitter_2D::Collision_Data> Space_Splitter_2D::m_collisions;
std::map<Space_Splitter_2D::Collision_Data, bool> Space_Splitter_2D::m_possible_collisions;

Timer Space_Splitter_2D::m_timer;



bool Space_Splitter_2D::Area::rectangle_is_inside(const Physical_Model_2D::Rectangular_Border& _rectangle) const
{
	bool result = true;

	if(!right.inf) result = result && _rectangle.left <= right.value;
	if(!left.inf) result = result && _rectangle.right >= left.value;
	if(!top.inf) result = result && _rectangle.bottom <= top.value;
	if(!bottom.inf) result = result && _rectangle.top >= bottom.value;

	return result;
}



void Space_Splitter_2D::Area::split(LEti::Tree<Area, 4>::Iterator _it)
{
	glm::vec3 split_point;
	split_point.x = (_it->right.value + _it->left.value) / 2.0f;
	split_point.y = (_it->top.value + _it->bottom.value) / 2.0f;

	_it.insert_into_availible_index({_it->left, split_point.x, _it->top, split_point.y});
	_it.insert_into_availible_index({split_point.x, _it->right, _it->top, split_point.y});
	_it.insert_into_availible_index({split_point.x, _it->right, split_point.y, _it->bottom});
	_it.insert_into_availible_index({_it->left, split_point.x, split_point.y, _it->bottom});
}



void Space_Splitter_2D::Collision_Data::update_collision_data()
{
	//	if(!_first->is_dynamic() && !_second->is_dynamic())
	{
		collision_data = first->is_colliding_with_other(*second);
		return;
	}
}



void Space_Splitter_2D::set_max_tree_depth(unsigned int _max_depth)
{
	m_max_tree_depth = _max_depth;
}


void Space_Splitter_2D::register_object(const Object_2D *_model)
{
	std::list<const Object_2D*>::iterator check = m_registred_models.begin();
	while(check != m_registred_models.end())
	{
		ASSERT(*check == _model);
		++check;
	}

	m_registred_models.push_back(_model);
}

void Space_Splitter_2D::unregister_object(const Object_2D *_model)
{
	std::list<const Object_2D*>::iterator it = m_registred_models.begin();
	while(it != m_registred_models.end())
	{
		if(*it == _model) break;
		++it;
	}
	ASSERT(it == m_registred_models.end());
	m_registred_models.erase(it);
}



void Space_Splitter_2D::split_space_recursive(LEti::Tree<Area, 4>::Iterator _it, unsigned int _level)
{
	const auto& objects = _it->objects;
	if (objects.size() < 3 || _level > m_max_tree_depth) return;

	_it->split(_it);

	for(unsigned int i=0; i<4; ++i)
	{
		LEti::Tree<Area, 4>::Iterator next = _it;
		next.descend(i);

		std::list<const Object_2D*>::const_iterator point_it = objects.begin();
		while(point_it != objects.end())
		{
			const Physical_Model_2D::Rectangular_Border& rb = (*point_it)->is_dynamic() ?
						(*point_it)->get_dynamic_rb() :
						(*point_it)->get_physical_model()->curr_rect_border();
			if(next->rectangle_is_inside(rb))
				next->objects.push_back(*point_it);

			++point_it;
		}

		split_space_recursive(next, _level+1);
	}
}

void Space_Splitter_2D::check_for_possible_collisions(LEti::Tree<Area, 4>::Iterator _it)
{
	LEti::Tree<Area, 4>::Iterator it = _it;

	while(!it.end())
	{
		if(it->objects.size() == 2)
		{
			std::list<const Object_2D*>::iterator colliding_model = it->objects.begin();
			std::list<const Object_2D*>::iterator next = colliding_model;
			++next;
			save_possible_collision(*colliding_model, *next);
		}
		else if(it.is_leaf() && it->objects.size() > 2)
		{
			std::list<const Object_2D*>::const_iterator colliding_model = it->objects.begin();
			while(colliding_model != it->objects.end())
			{
				std::list<const Object_2D*>::const_iterator next = colliding_model;
				++next;
				while(next != it->objects.end())
				{
					save_possible_collision(*colliding_model, *next);
					++next;
				}
				++colliding_model;
			}
		}
		++it;
	}
	if(it->objects.size() == 2)
	{
		std::list<const Object_2D*>::iterator colliding_model = it->objects.begin();
		std::list<const Object_2D*>::iterator next = colliding_model;
		++next;
		save_possible_collision(*colliding_model, *next);
	}
	else if(it.is_leaf() && it->objects.size() > 2)
	{
		std::list<const Object_2D*>::const_iterator colliding_model = it->objects.begin();
		while(colliding_model != it->objects.end())
		{
			std::list<const Object_2D*>::const_iterator next = colliding_model;
			++next;
			while(next != it->objects.end())
			{
				save_possible_collision(*colliding_model, *next);
				++next;
			}
			++colliding_model;
		}
	}
}

bool Space_Splitter_2D::possible_collision_is_already_saved(const Collision_Data &_cd)
{
	std::map<Collision_Data, bool>::iterator it = m_possible_collisions.find(_cd);
	return it != m_possible_collisions.end();
}

void Space_Splitter_2D::save_possible_collision(const Object_2D *_first, const Object_2D *_second)
{
	Collision_Data cd(_first, _second);
	if(!possible_collision_is_already_saved(cd))
		m_possible_collisions.emplace(cd, false);
}

void Space_Splitter_2D::save_actual_collisions()
{
	std::map<Collision_Data, bool>::const_iterator it = m_possible_collisions.begin();
	while(it != m_possible_collisions.end())
	{
		Collision_Data collision_data = it->first;
		collision_data.update_collision_data();
		if(collision_data.collision_data)
			m_collisions.push_back(collision_data);
		++it;
	}

}


void Space_Splitter_2D::update()
{
//	m_timer.update();
//	if(m_timer.is_active()) return;
//	m_timer.start(0.05f);

	m_possible_collisions.clear();
	m_collisions.clear();

	LEti::Tree<Area, 4>::Iterator it = m_quad_tree.create_iterator();
	if(it.valid()) it.delete_branch();
	it = m_quad_tree.create_iterator();

	it.insert_into_availible_index({{}, {}, {}, {}});

	if(m_registred_models.size() == 0) return;

	std::list<const Object_2D*>::iterator model_it = m_registred_models.begin();
	while(model_it != m_registred_models.end())
	{
		if((*model_it)->get_collision_possibility() == false)
		{
			++model_it;
			continue;
		}

		const Physical_Model_2D::Rectangular_Border& rb = (*model_it)->is_dynamic() ?
					(*model_it)->get_dynamic_rb() :
					(*model_it)->get_physical_model()->curr_rect_border();

		if(rb.left < it->left.value || it->left.inf) it->left = rb.left;
		if(rb.right > it->right.value || it->right.inf) it->right = rb.right;
		if(rb.top > it->top.value || it->top.inf) it->top = rb.top;
		if(rb.bottom < it->bottom.value || it->bottom.inf) it->bottom = rb.bottom;

		it->objects.push_back(*model_it);

		++model_it;
	}

	split_space_recursive(it, 0);

	check_for_possible_collisions(it);

	save_actual_collisions();
}



const std::list<Space_Splitter_2D::Collision_Data>& Space_Splitter_2D::get_collisions()
{
	return m_collisions;
}
