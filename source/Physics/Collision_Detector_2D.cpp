#include "../../include/Physics/Collision_Detector_2D.h"

using namespace LEti;


Collision_Detector_2D::Collision_Detector_2D()
{

}

Collision_Detector_2D::~Collision_Detector_2D()
{
	delete m_broad_phase;
	delete m_narrow_phase;
}



void Collision_Detector_2D::debug_assert_if_model_copy_found(const LEti::Object_2D *_model, bool _reverse)
{
	std::list<const LEti::Object_2D*>::iterator check = m_registred_models.begin();
	while(check != m_registred_models.end())
	{
		if(!_reverse)
		{ L_ASSERT(!(*check == _model)); }
		++check;
	}

	L_ASSERT(!_reverse);
}

void Collision_Detector_2D::debug_assert_if_point_copy_found(const glm::vec3 *_point, bool _reverse)
{
	std::list<const glm::vec3*>::iterator check = m_registred_points.begin();
	while(check != m_registred_points.end())
	{
		if(!_reverse)
			{ L_ASSERT(!(*check == _point)); }
		++check;
	}

	L_ASSERT(!_reverse);
}



void Collision_Detector_2D::set_broad_phase(Broad_Phase_Interface* _broad_phase_impl, unsigned int _precision)
{
	delete m_broad_phase;
	m_broad_phase = _broad_phase_impl;
	m_broad_phase->set_precision(_precision);
}

void Collision_Detector_2D::set_narrow_phase(Narrow_Phase_Interface* _narrow_phase_impl, unsigned int _precision)
{
	delete m_narrow_phase;
	m_narrow_phase = _narrow_phase_impl;
	m_narrow_phase->set_precision(_precision);
}

void Collision_Detector_2D::set_narrowest_phase(Narrowest_Phase_Interface* _narrowest_phase_impl)
{
	L_ASSERT(m_narrow_phase);
	m_narrow_phase->set_narrowest_phase(_narrowest_phase_impl);
}



void Collision_Detector_2D::register_object(const LEti::Object_2D *_model)
{
	L_DEBUG_FUNC_2ARG(debug_assert_if_model_copy_found, _model, false);
	m_registred_models.push_back(_model);
}

void Collision_Detector_2D::unregister_object(const LEti::Object_2D *_model)
{
	std::list<const LEti::Object_2D*>::iterator it = m_registred_models.begin();
	while(it != m_registred_models.end())
	{
		if(*it == _model) break;
		++it;
	}
	L_ASSERT(it != m_registred_models.end());
	m_registred_models.erase(it);
}

void Collision_Detector_2D::register_point(const glm::vec3 *_point)
{
	L_DEBUG_FUNC_2ARG(debug_assert_if_point_copy_found, _point, false);
	m_registred_points.push_back(_point);
}

void Collision_Detector_2D::unregister_point(const glm::vec3 *_point)
{
	std::list<const glm::vec3*>::iterator it = m_registred_points.begin();
	while(it != m_registred_points.end())
	{
		if(*it == _point) break;
		++it;
	}
	L_ASSERT(!(it == m_registred_points.end()));
	m_registred_points.erase(it);
}



void Collision_Detector_2D::update()
{
	L_ASSERT(!(!m_broad_phase || !m_narrow_phase));

	m_broad_phase->update(m_registred_models, m_registred_points);

	std::list<Broad_Phase_Interface::Colliding_Pair> possible_collisions__models = m_broad_phase->get_possible_collisions__models();
	std::list<Broad_Phase_Interface::Colliding_Point_And_Object> possible_collisions__points = m_broad_phase->get_possible_collisions__points();

	m_narrow_phase->update(possible_collisions__models, possible_collisions__points);
}



const Narrow_Phase_Interface::Collision_Data_List__Models& Collision_Detector_2D::get_collisions__models()
{
	L_ASSERT(!(!m_broad_phase || !m_narrow_phase));
	return m_narrow_phase->get_collisions__models();
}

const Narrow_Phase_Interface::Collision_Data_List__Points& Collision_Detector_2D::get_collisions__points()
{
	L_ASSERT(!(!m_broad_phase || !m_narrow_phase));
	return m_narrow_phase->get_collisions__points();
}






































