#include "../../include/Physics/Space_Hasher_2D.h"

using namespace LEti;


unsigned int Space_Hasher_2D::get_number_binary_length(unsigned int _number)
{
	unsigned int result = 0;
	for(unsigned int i=0; i<sizeof(_number) * 8; ++i)
		if(_number & (1u << i))
			result = i + 1;
	return result;
}



unsigned int Space_Hasher_2D::construct_hash(unsigned int _x, unsigned int _y)
{
	return (_x << m_number_binary_length) | (_y);
}

void Space_Hasher_2D::update_border(const objects_list& _registred_objects)
{
	if(_registred_objects.size() == 0) return;

	std::list<const LEti::Object_2D*>::const_iterator model_it = _registred_objects.begin();

    const Geometry_2D::Rectangular_Border& first_rb = (*model_it)->physics_module()->rectangular_border();
	float max_left = first_rb.left;
	float max_right = first_rb.right;
	float max_top = first_rb.top;
	float max_bottom = first_rb.bottom;
	++model_it;

	while(model_it != _registred_objects.end())
	{
		if((*model_it)->physics_module()->can_collide() == false)
		{
			++model_it;
			continue;
		}

        const Geometry_2D::Rectangular_Border& rb = (*model_it)->physics_module()->rectangular_border();

		if(rb.left < max_left) max_left = rb.left;
		if(rb.right > max_right) max_right = rb.right;
		if(rb.top > max_top) max_top = rb.top;
		if(rb.bottom < max_bottom) max_bottom = rb.bottom;

		++model_it;
	}

	m_space_borders.min_x = max_left;
	m_space_borders.min_y = max_bottom;
	m_space_borders.width = max_right - max_left;
	m_space_borders.height= max_top - max_bottom;
}

void Space_Hasher_2D::reset_hash_array()
{
	L_ASSERT(!(m_array == nullptr));

	for(unsigned int i=0; i<m_array_size; ++i)
	{
		if(m_array[i] != nullptr)
		{
			delete m_array[i];
			m_array[i] = nullptr;
		}
	}
}

void Space_Hasher_2D::hash_objects(const objects_list& _registred_objects)
{
	objects_list::const_iterator model_it = _registred_objects.cbegin();
	while(model_it != _registred_objects.end())
	{
        const Geometry_2D::Rectangular_Border& curr_rb = (*model_it)->physics_module()->rectangular_border();

		unsigned int min_index_x = (unsigned int)((curr_rb.left - m_space_borders.min_x) / m_space_borders.width * m_precision);
		unsigned int max_index_x = (unsigned int)((curr_rb.right - m_space_borders.min_x) / m_space_borders.width * m_precision);
		unsigned int min_index_y = (unsigned int)((curr_rb.bottom - m_space_borders.min_y) / m_space_borders.height * m_precision);
		unsigned int max_index_y = (unsigned int)((curr_rb.top - m_space_borders.min_y) / m_space_borders.height * m_precision);

		for(unsigned int x = min_index_x; x <= max_index_x; ++x)
		{
			for(unsigned int y = min_index_y; y <= max_index_y; ++y)
			{
				unsigned int hash = construct_hash(x, y);

				if(m_array[hash])
				{
					bool copy = false;
					for(const LEti::Object_2D*& a : *(m_array[hash]))
					{
						if(a == *model_it)
							copy = true;
					}
					if(!copy)
						m_array[hash]->push_back(*model_it);
				}
				else
				{
					m_array[hash] = new objects_list;
					m_array[hash]->push_back(*model_it);
				}
			}
		}

		++model_it;
	}
}

void Space_Hasher_2D::check_for_possible_collisions__points(const points_list &_registred_points)
{
//		m_possible_point_and_object_collisions
//		while*
	m_possible_collisions__points.clear();

	points_list::const_iterator point_it = _registred_points.cbegin();
	while(point_it != _registred_points.cend())
	{
		glm::vec3 point_with_offset = *(*point_it);
		point_with_offset.x -= m_space_borders.min_x;
		point_with_offset.y -= m_space_borders.min_y;

		if(point_with_offset.x < 0.0f || point_with_offset.y < 0.0f || point_with_offset.x > m_space_borders.width || point_with_offset.y > m_space_borders.height)
		{
			++point_it;
			continue;
		}

		unsigned int x = (unsigned int)(point_with_offset.x / m_space_borders.width * m_precision);
		unsigned int y = (unsigned int)(point_with_offset.y / m_space_borders.height * m_precision);
		unsigned int hash = construct_hash(x, y);

		if(m_array[hash] == nullptr)
		{
			++point_it;
			continue;
		}

		const objects_list& list = *(m_array[hash]);
		objects_list::const_iterator it = list.cbegin();
		while(it != list.end())
		{
			if((*it)->physics_module()->can_collide())
				m_possible_collisions__points.emplace(Colliding_Point_And_Object(*it, *point_it), false);
			++it;
		}

		++point_it;
	}
}



void Space_Hasher_2D::check_for_possible_collisions__models()
{
	m_possible_collisions__models.clear();

	for(unsigned int i=0; i<m_array_size; ++i)
	{
		if(m_array[i] == nullptr) continue;
		const objects_list& curr_list = *(m_array[i]);
		if(curr_list.size() < 2) continue;

		objects_list::const_iterator first = curr_list.begin();
		while(first != curr_list.end())
		{
			objects_list::const_iterator second = first;
			++second;

			while(second != curr_list.end())
			{
				Colliding_Pair cd(*first, *second);
				if(m_possible_collisions__models.find(cd) == m_possible_collisions__models.end())
					m_possible_collisions__models.emplace(cd, false);

				++second;
			}

			++first;
		}
	}
}



Space_Hasher_2D::~Space_Hasher_2D()
{
	if(m_array)
	{
		for(unsigned int i=0; i<m_array_size; ++i)
			delete m_array[i];
		delete[] m_array;
	}
}



void Space_Hasher_2D::set_precision(unsigned int _precision)
{
	L_ASSERT(!(_precision == 0));

	if(m_array)
	{
		for(unsigned int i=0; i<m_array_size; ++i)
			delete m_array[i];
		delete[] m_array;
		m_array = nullptr;
	}

	m_number_binary_length = get_number_binary_length(_precision);
	m_precision = _precision;
	m_array_size = ((m_precision + 1) << m_number_binary_length) | m_precision + 1;
	m_array = new objects_list*[m_array_size];
	for(unsigned int i=0; i<m_array_size; ++i)
		m_array[i] = nullptr;
}


void Space_Hasher_2D::update(const objects_list &_registred_objects, const points_list &_registred_points)
{
	update_border(_registred_objects);
	reset_hash_array();
	hash_objects(_registred_objects);
	check_for_possible_collisions__models();
	check_for_possible_collisions__points(_registred_points);
}


std::list<Space_Hasher_2D::Colliding_Pair> Space_Hasher_2D::get_possible_collisions__models()
{
	std::list<Colliding_Pair> result;
	std::map<Colliding_Pair, bool>::const_iterator it = m_possible_collisions__models.begin();

	while(it != m_possible_collisions__models.end())
	{
		result.push_back({it->first.first, it->first.second});
		++it;
	}

	return result;
}

std::list<Space_Hasher_2D::Colliding_Point_And_Object> Space_Hasher_2D::get_possible_collisions__points()
{
	std::list<Colliding_Point_And_Object> result;
	std::map<Colliding_Point_And_Object, bool>::const_iterator it = m_possible_collisions__points.begin();

	while(it != m_possible_collisions__points.end())
	{
		result.push_back({it->first.object, it->first.point});
		++it;
	}

	return result;
}
