#ifndef SAT_NARROWEST_CD_H
#define SAT_NARROWEST_CD_H

#include "Data_Structures/List.h"

#include "Narrowest_Phase_Interface.h"


namespace LEti
{

class SAT_Narrowest_CD : public Narrowest_Phase_Interface
{
public:
    SAT_Narrowest_CD() {}
    ~SAT_Narrowest_CD() {}

private:
    struct Intersection_Data
    {
        bool intersection = false;
        float min_dist = -1.0f;
        glm::vec3 min_dist_axis;
    };

    using mm_pair = std::pair<float, float>;

private:
    mm_pair M_get_minmax_projections(const glm::vec3& _axis, const Polygon& _pol) const;

    Intersection_Data M_polygons_collision(const Polygon& _first, const Polygon& _second) const;

    float M_smallest_point_to_polygon_distance(const glm::vec3& _point, const Polygon& _pol) const;
    LDS::List<glm::vec3> M_points_of_contact(const Polygon_Holder_Base* _f_pols, unsigned int _f_count, const Polygon_Holder_Base* _s_pols, unsigned int _s_count) const;

    Geometry::Simple_Intersection_Data intersection__polygon_vs_point(const Polygon& _polygon, const glm::vec3& _point) const;

public:
    Geometry::Simple_Intersection_Data collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const override;
    LEti::Intersection_Data collision__model_vs_model(const Polygon_Holder_Base* _polygon_holder_1, unsigned int _pols_amount_1, const Polygon_Holder_Base* _polygon_holder_2, unsigned int _pols_amount_2) const override;
};

}

#endif // SAT_NARROWEST_CD_H
