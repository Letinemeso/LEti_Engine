#include "../include/EIPF.h"

using namespace LEti;


void EIPF::rotate_cw(Direction &_d) const
{
    switch(_d)
    {
    case Direction::up: _d = Direction::right; break;
    case Direction::right: _d = Direction::down; break;
    case Direction::down: _d = Direction::left; break;
    case Direction::left: _d = Direction::up; break;
    }
}



void EIPF::intp::move(Direction _d)
{
    switch (_d)
    {
    case Direction::up:{ ++y; break; }
    case Direction::down:{ --y; break; }
    case Direction::left:{ --x; break; }
    case Direction::right:{ ++x; break; }
    }
}

EIPF::intp EIPF::intp::after_movement(Direction _d) const
{
    switch (_d)
    {
    case Direction::up:{ return intp(x, y+1); }
    case Direction::down:{ return intp(x, y-1); }
    case Direction::left:{ return intp(x-1, y); }
    case Direction::right:{ return intp(x+1, y); }
    }
}



bool EIPF::is_step_unique(const intp &_step) const
{
    auto iter = m_tree.create_iterator();

    if(!iter.valid()) return true;

    while(!iter.end())
    {
        if(*iter == _step) return false;
        ++iter;
    }
    if(*iter == _step) return false;

    return true;
}

unsigned int EIPF::get_distance(const intp &_first, const intp &_second) const
{
    return abs(_first.x - _second.x) + abs(_first.y - _second.y);
}

bool EIPF::out_of_bounds(const intp &_ind) const
{
    return _ind.x < 0 || _ind.y < 0 || _ind.x >= m_size_x || _ind.y >= m_size_y;
}

bool EIPF::at(const intp &_ind) const
{
    return m_occupied[_ind.x][_ind.y];
}


bool EIPF::can_move_here(const intp &_where) const
{
    if(out_of_bounds(_where)) return false;
    if(at(_where)) return false;
    return is_step_unique(_where);
}


std::pair<LEti::Tree<EIPF::intp, 3>::Iterator, EIPF::intp> EIPF::find_closest_pos(const intp& _dest) const
{
    auto iter = m_tree.create_iterator();

    unsigned int min_dist = -1;
    intp min_dist_pos = {-1, -1};
    LEti::Tree<intp, 3>::Iterator mdp_parent = iter;

    while(!iter.end())
    {
        Direction d = Direction::up;
        for(unsigned int i=0; i<4; ++i)
        {
            if(!can_move_here((*iter).after_movement(d))) { rotate_cw(d); continue; }
            unsigned int local_min_dist = get_distance(*iter, _dest);
            if(local_min_dist < min_dist)
            {
                min_dist = local_min_dist;
                min_dist_pos = (*iter).after_movement(d);
                mdp_parent = iter;
            }
            rotate_cw(d);
        }
        ++iter;
    }
    Direction d = Direction::up;
    for(unsigned int i=0; i<4; ++i)
    {
        if(!can_move_here((*iter).after_movement(d))) { rotate_cw(d); continue; }
        unsigned int local_min_dist = get_distance(*iter, _dest);
        if(local_min_dist < min_dist)
        {
            min_dist = local_min_dist;
            min_dist_pos = (*iter).after_movement(d);
            mdp_parent = iter;
        }
        rotate_cw(d);
    }

    return {mdp_parent, min_dist_pos};
}



EIPF::EIPF()
{

}

EIPF::~EIPF()
{
    if(m_occupied)
    {
        for(unsigned int i=0; i<m_size_x; ++i)
            delete m_occupied[i];
        delete m_occupied;
    }
}



void EIPF::set_field_size(unsigned int _x, unsigned int _y)
{
    if(m_occupied)
    {
        for(unsigned int i=0; i<m_size_x; ++i)
            delete m_occupied[i];
        delete m_occupied;
    }

    m_size_x = _x; m_size_y = _y;

    for(unsigned int i=0; i<m_size_x; ++i)
    {
        m_occupied = new bool * [m_size_y];
        for(unsigned int j=0; j<m_size_y; ++j) m_occupied[i][j] = false;
    }
}

void EIPF::set_destination(unsigned int _x, unsigned int _y)
{
    m_destination = {(int)_x, (int)_y};
}

void EIPF::set_start_pos(unsigned int _x, unsigned int _y)
{
    m_current_pos = {(int)_x, (int)_y};
}

bool& EIPF::field_element(unsigned int _x, unsigned int _y)
{
    ASSERT(out_of_bounds({(int)_x, (int)_y}));
    return m_occupied[_x][_y];
}



void EIPF::build_path()
{
    ASSERT(out_of_bounds(m_current_pos) || out_of_bounds(m_destination) || !m_occupied);

    auto iter = m_tree.create_iterator();
    if(iter.valid()) iter.delete_branch();
    iter.insert_into_availible_index(m_current_pos);

    m_steps_count = 0;
    m_path_found = false;

    while(*iter == m_destination)
    {
        auto a = find_closest_pos(m_destination);
        if(a.second == intp(-1, -1)) return;

        a.first.descend( a.first.insert_into_availible_index(a.second) );
        iter = a.first;

        ++m_steps_count;
    }

    m_path_end = iter;
    m_path_found = true;
}

std::pair<int, int> EIPF::get_next_step() const
{
    ASSERT(out_of_bounds(m_current_pos) || out_of_bounds(m_destination) || !m_occupied);

    if(m_steps_count == 0) return {-1, -1};

    auto iter = m_path_end;
    if(!iter.valid()) return {-1, -1};

    unsigned int second_step_index = 0;
    while(!iter.begin())
        second_step_index = iter.ascend();

    iter.descend(second_step_index);
    return {(*iter).x, (*iter).y};
}

std::vector<std::pair<int, int>> EIPF::get_full_path() const
{
    ASSERT(out_of_bounds(m_current_pos) || out_of_bounds(m_destination) || !m_occupied);

    std::vector<std::pair<int, int>> result;
    if(m_steps_count == 0) return result;

    auto iter = m_path_end;

    while(!iter.begin())
    {
        result.push_back({(*iter).x, (*iter).y});
        iter.ascend();
    }
    result.push_back({(*iter).x, (*iter).y});

    for(unsigned int i=0; i<result.size() / 2; ++i)
        std::swap(result[i], result[result.size() - i - 1]);

    return result;
}
