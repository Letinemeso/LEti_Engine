#include <Physics/Collision_Resolver.h>

using namespace LEti;


Collision_Resolver::Collision_Resolver()
{

}

Collision_Resolver::Collision_Resolver(Collision_Resolver&& _other)
{
	m_resolutions = (LDS::List<Collision_Resolution_Interface*>&&)_other.m_resolutions;
}

Collision_Resolver::~Collision_Resolver()
{
	clear_resolutions();
}



void Collision_Resolver::add_resolution(Collision_Resolution_Interface *_resolution)
{
	m_resolutions.push_back(_resolution);
}

void Collision_Resolver::clear_resolutions()
{
	for(auto it = m_resolutions.begin(); !it.end_reached(); ++it)
		delete *it;

	m_resolutions.clear();
}



void Collision_Resolver::resolve_single(const Physical_Model_2D::Intersection_Data &_id) const
{
	auto it = m_resolutions.begin();
	bool resolved = false;
	while(!it.end_reached() && !resolved)
	{
		resolved = (*it)->resolve(_id);
		++it;
	}

	L_ASSERT(resolved);
}

void Collision_Resolver::resolve_all(const std::list<Physical_Model_2D::Intersection_Data> &_ids) const
{
	for(auto it = _ids.begin(); it != _ids.end(); ++it)
		resolve_single(*it);
}
