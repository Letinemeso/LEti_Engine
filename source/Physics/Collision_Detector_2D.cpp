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
    LDS::List<const LEti::Object_2D*>::Iterator check = m_registred_models.begin();
    while(!check.end_reached())
	{
        L_ASSERT(!_reverse && *check != _model);
		++check;
	}

	L_ASSERT(!_reverse);
}

void Collision_Detector_2D::debug_assert_if_point_copy_found(const glm::vec3 *_point, bool _reverse)
{
    LDS::List<const glm::vec3*>::Iterator check = m_registred_points.begin();
    while(!check.end_reached())
    {
        L_ASSERT(!_reverse && *check != _point);
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
    LDS::List<const LEti::Object_2D*>::Iterator it = m_registred_models.begin();
    while(!it.end_reached())
	{
        if(*it == _model)
            break;
		++it;
	}
    L_ASSERT(!it.end_reached());
	m_registred_models.erase(it);
}

void Collision_Detector_2D::unregister_all_objects()
{
    m_registred_models.clear();
}

void Collision_Detector_2D::register_point(const glm::vec3 *_point)
{
	L_DEBUG_FUNC_2ARG(debug_assert_if_point_copy_found, _point, false);
	m_registred_points.push_back(_point);
}

void Collision_Detector_2D::unregister_point(const glm::vec3 *_point)
{
    LDS::List<const glm::vec3*>::Iterator it = m_registred_points.begin();
    while(!it.end_reached())
	{
        if(*it == _point)
            break;
		++it;
	}
    L_ASSERT(!it.end_reached());
	m_registred_points.erase(it);
}

void Collision_Detector_2D::unregister_all_points()
{
    m_registred_points.clear();
}



void Collision_Detector_2D::update()
{
	L_ASSERT(!(!m_broad_phase || !m_narrow_phase));

	m_broad_phase->update(m_registred_models, m_registred_points);

    LDS::List<Broad_Phase_Interface::Colliding_Pair> possible_collisions__models = m_broad_phase->get_possible_collisions__models();
    LDS::List<Broad_Phase_Interface::Colliding_Point_And_Object> possible_collisions__points = m_broad_phase->get_possible_collisions__points();

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






































