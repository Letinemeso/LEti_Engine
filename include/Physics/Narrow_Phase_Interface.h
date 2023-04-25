#ifndef __NARROW_PHASE_INTERFACE
#define __NARROW_PHASE_INTERFACE

//#include "../../include/Object.h"
#include <Object_System/Object_2D.h>

#include "Broad_Phase_Interface.h"
#include "Narrowest_Phase_Interface.h"

namespace LEti
{

class Narrow_Phase_Interface
{
protected:
    Narrowest_Phase_Interface* m_narrowest_phase = nullptr;

public:
    void set_narrowest_phase(Narrowest_Phase_Interface* _narrowest_phase_impl);

public:
    using Collision_Data_List__Models = std::list<Physical_Model_2D::Intersection_Data>;
    using Collision_Data_List__Points = std::list<Physical_Model_2D::Intersection_Data>;

public:
    virtual ~Narrow_Phase_Interface();

public:
    virtual void update(const Broad_Phase_Interface::Colliding_Pair_List& _possible_collisions__models, const Broad_Phase_Interface::Colliding_Point_And_Object_List& _possible_collisions__pointsnp) = 0;

    virtual const Collision_Data_List__Models& get_collisions__models() const = 0;
    virtual const Collision_Data_List__Points& get_collisions__points() const = 0;

    virtual void set_precision(unsigned int _precision) = 0;

};

}


#endif // __NARROW_PHASE_INTERFACE
