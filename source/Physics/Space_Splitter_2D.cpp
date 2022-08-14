#include "../../include/Physics/Space_Splitter_2D.h"


using namespace LEti;


std::list<const Object_2D*> Space_Splitter_2D::m_registred_models;
std::list<const glm::vec3*> Space_Splitter_2D::m_registred_points;

Broad_Phase_Interface* Space_Splitter_2D::m_broad_phase = nullptr;
Middle_Phase_Interface* Space_Splitter_2D::m_middle_phase = nullptr;
Narrow_Phase_Interface* Space_Splitter_2D::m_narrow_phase = nullptr;



void Space_Splitter_2D::debug_assert_if_model_copy_found(const Object_2D *_model, bool _reverse)
{
	std::list<const Object_2D*>::iterator check = m_registred_models.begin();
	while(check != m_registred_models.end())
	{
		if(!_reverse)
			ASSERT(*check == _model);
		++check;
	}
	if(_reverse)
		ASSERT(true);
}

void Space_Splitter_2D::debug_assert_if_point_copy_found(const glm::vec3 *_point, bool _reverse)
{
	std::list<const glm::vec3*>::iterator check = m_registred_points.begin();
	while(check != m_registred_points.end())
	{
		if(!_reverse)
			ASSERT(*check == _point);
		++check;
	}
	if(_reverse)
		ASSERT(true);
}



Broad_Phase_Interface* Space_Splitter_2D::get_broad_phase()
{
	return m_broad_phase;
}

Middle_Phase_Interface* Space_Splitter_2D::get_middle_phase()
{
	return m_middle_phase;
}

Narrow_Phase_Interface* Space_Splitter_2D::get_narrow_phase()
{
	return m_narrow_phase;
}



void Space_Splitter_2D::register_object(const Object_2D *_model)
{
	DEBUG_FUNC_2ARG(debug_assert_if_model_copy_found, _model, false);
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

void Space_Splitter_2D::register_point(const glm::vec3 *_point)
{
	DEBUG_FUNC_2ARG(debug_assert_if_point_copy_found, _point, false);
	m_registred_points.push_back(_point);
}

void Space_Splitter_2D::unregister_point(const glm::vec3 *_point)
{
	std::list<const glm::vec3*>::iterator it = m_registred_points.begin();
	while(it != m_registred_points.end())
	{
		if(*it == _point) break;
		++it;
	}
	ASSERT(it == m_registred_points.end());
	m_registred_points.erase(it);
}



void Space_Splitter_2D::update()
{
	ASSERT(!m_broad_phase || !m_narrow_phase);

	m_broad_phase->update(m_registred_models, m_registred_points);

	std::list<Broad_Phase_Interface::Colliding_Pair> possible_collisions__models = m_broad_phase->get_possible_collisions__models();
	std::list<Broad_Phase_Interface::Colliding_Point_And_Object> possible_collisions__points = m_broad_phase->get_possible_collisions__points();

	if(m_middle_phase)
		m_middle_phase->update(possible_collisions__models, possible_collisions__points);

	m_narrow_phase->update(possible_collisions__models, possible_collisions__points);

//	save_actual_collisions();
}



const Narrow_Phase_Interface::Collision_Data_List__Models& Space_Splitter_2D::get_collisions__models()
{
	ASSERT(!m_broad_phase || !m_narrow_phase);
	return m_narrow_phase->get_collisions__models();
}

const Narrow_Phase_Interface::Collision_Data_List__Points& Space_Splitter_2D::get_collisions__points()
{
	ASSERT(!m_broad_phase || !m_narrow_phase);
	return m_narrow_phase->get_collisions__points();
}






































