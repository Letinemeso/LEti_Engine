#ifndef __NARROWEST_PHASE_INTERFACE
#define __NARROWEST_PHASE_INTERFACE

#include <Math_Stuff.h>
#include <Physics/Physical_Models/Physical_Model_2D.h>
#include <Physics/Intersection_Data.h>


namespace LEti {

    class Narrowest_Phase_Interface
    {
    public:
        Narrowest_Phase_Interface();
        virtual ~Narrowest_Phase_Interface();

    public:
        virtual Geometry::Simple_Intersection_Data collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const = 0;
        virtual Intersection_Data collision__model_vs_model(const Polygon* _pols_1, unsigned int _pols_amount_1, const Polygon* _pols_2, unsigned int _pols_amount_2) const = 0;

    };

}


#endif // __NARROWEST_PHASE_INTERFACE
