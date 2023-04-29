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

    LDS::List<const LEti::Object_2D*>::Const_Iterator model_it = _registred_objects.begin();

    bool rb_inited = false;

    float max_left = 0.0f;
    float max_right = 0.0f;
    float max_top = 0.0f;
    float max_bottom = 0.0f;

    while(!model_it.end_reached())
	{
        if(!(*model_it)->physics_module())
        {
            ++model_it;
            continue;
        }

		if((*model_it)->physics_module()->can_collide() == false)
		{
			++model_it;
			continue;
		}

        const Geometry_2D::Rectangular_Border& rb = (*model_it)->physics_module()->rectangular_border();

        if(!rb_inited)
        {
            max_left = rb.left;
            max_right = rb.right;
            max_top = rb.top;
            max_bottom = rb.bottom;
            rb_inited = true;
        }
        else
        {
            if(rb.left < max_left) max_left = rb.left;
            if(rb.right > max_right) max_right = rb.right;
            if(rb.top > max_top) max_top = rb.top;
            if(rb.bottom < max_bottom) max_bottom = rb.bottom;
        }

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
    objects_list::Const_Iterator model_it = _registred_objects.begin();
    while(!model_it.end_reached())
	{
        if(!(*model_it)->physics_module())
        {
            ++model_it;
            continue;
        }

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

                if(!m_array[hash])
                {
                    m_array[hash] = new objects_list;
                    m_array[hash]->push_back(*model_it);
                    continue;
                }

                bool copy = false;

                const objects_list& list = *m_array[hash];
                for(objects_list::Const_Iterator list_it = list.begin(); !list_it.end_reached(); ++list_it)
                {
                    if(*list_it == *model_it)
                        copy = true;
                }

                if(!copy)
                    m_array[hash]->push_back(*model_it);
			}
		}

		++model_it;
	}
}

void Space_Hasher_2D::check_for_possible_collisions__points(const points_list &_registred_points)
{
	m_possible_collisions__points.clear();

    points_list::Const_Iterator point_it = _registred_points.begin();
    while(!point_it.end_reached())
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

        const objects_list& list = *m_array[hash];
        objects_list::Const_Iterator it = list.begin();
        while(!it.end_reached())
		{
			if((*it)->physics_module()->can_collide())
                m_possible_collisions__points.insert(Colliding_Point_And_Object(*it, *point_it));
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

        objects_list::Const_Iterator first = curr_list.begin();
        while(!first.end_reached())
		{
            objects_list::Const_Iterator second = first;
			++second;

            while(!second.end_reached())
			{
				Colliding_Pair cd(*first, *second);
                if(!m_possible_collisions__models.find(cd).is_ok())
                    m_possible_collisions__models.insert(cd);

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


LDS::List<Space_Hasher_2D::Colliding_Pair> Space_Hasher_2D::get_possible_collisions__models()
{
    LDS::List<Colliding_Pair> result;
    LDS::AVL_Tree<Colliding_Pair>::Iterator it = m_possible_collisions__models.iterator();

    while(!it.end_reached())
	{
        result.push_back({it->first, it->second});
		++it;
	}

	return result;
}

LDS::List<Space_Hasher_2D::Colliding_Point_And_Object> Space_Hasher_2D::get_possible_collisions__points()
{
    LDS::List<Colliding_Point_And_Object> result;
    LDS::AVL_Tree<Colliding_Point_And_Object>::Iterator it = m_possible_collisions__points.iterator();

    while(!it.end_reached())
	{
        result.push_back({it->object, it->point});
		++it;
	}

	return result;
}
