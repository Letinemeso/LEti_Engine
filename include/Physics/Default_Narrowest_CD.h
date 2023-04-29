#ifndef __DEFAULT_NARROWEST_CD
#define __DEFAULT_NARROWEST_CD

#include "../../include/Physics/Narrowest_Phase_Interface.h"


namespace LEti {

class Default_Narrowest_CD : public Narrowest_Phase_Interface
{
public:
    Default_Narrowest_CD();
    ~Default_Narrowest_CD();

private:
    Geometry::Simple_Intersection_Data intersection__polygon_vs_point(const Polygon& _polygon, const glm::vec3& _point) const;
    Intersection_Data intersection__polygon_vs_segment(const Polygon& _polygon, const Geometry::Segment& _segment) const;
    Intersection_Data intersection__polygon_vs_polygon(const Polygon& _first, const Polygon& _second) const;

    Intersection_Data collision__model_vs_segment(const Physical_Model_2D& _model, const Geometry::Segment& _segment) const;

public:
    Geometry::Simple_Intersection_Data collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const override;
    Intersection_Data collision__model_vs_model(const Polygon_Holder_Base* _polygon_holder_1, unsigned int _pols_amount_1, const Polygon_Holder_Base* _polygon_holder_2, unsigned int _pols_amount_2) const override;

};

}


#endif // __DEFAULT_NARROWEST_CD
