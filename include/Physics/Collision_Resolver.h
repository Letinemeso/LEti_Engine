#ifndef COLLISION_RESOLVER_H
#define COLLISION_RESOLVER_H

#include <L_Debug/L_Debug.h>
#include <Physics/Collision_Detector_2D.h>

#include <list>


namespace LEti
{

class Collision_Resolution_Interface
{
public:
    Collision_Resolution_Interface(){}
    virtual ~Collision_Resolution_Interface(){}

public:
    virtual bool resolve(const Intersection_Data& _id) = 0;

};

class Collision_Resolver
{
private:
    LDS::List<Collision_Resolution_Interface*> m_resolutions;

public:
    Collision_Resolver();
    Collision_Resolver(const Collision_Resolver& _other) = delete;
    Collision_Resolver(Collision_Resolver&& _other);
    ~Collision_Resolver();

public:
    void add_resolution(Collision_Resolution_Interface* _resolution);
    void clear_resolutions();

public:
    void resolve_single(const Intersection_Data& _id) const;
    void resolve_all(const LDS::List<Intersection_Data>& _ids) const;

};

}

#endif // COLLISION_RESOLVER_H
