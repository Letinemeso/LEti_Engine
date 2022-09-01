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
		Geometry::Simple_Intersection_Data intersection__polygon_vs_point(const Geometry::Polygon& _polygon, const glm::vec3& _point) const;
		Physical_Model_2D::Intersection_Data intersection__polygon_vs_segment(const Geometry::Polygon& _polygon, const Geometry::Segment& _segment) const;
		Physical_Model_2D::Intersection_Data intersection__polygon_vs_polygon(const Geometry::Polygon& _first, const Geometry::Polygon& _second) const;

		Physical_Model_2D::Intersection_Data collision__model_vs_segment(const Physical_Model_2D& _model, const Geometry::Segment& _segment) const;

	public:
		Geometry::Simple_Intersection_Data collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const override;

		Physical_Model_2D::Intersection_Data collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D& _2) const override;
		Physical_Model_2D::Intersection_Data collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D::Imprint& _2) const override;
		Physical_Model_2D::Intersection_Data collision__model_vs_model(const Physical_Model_2D::Imprint& _impr, const Physical_Model_2D& _model) const override;
		Physical_Model_2D::Intersection_Data collision__model_vs_model(const Physical_Model_2D& _model, const Physical_Model_2D::Imprint& _impr) const override;

	};

}


#endif // __DEFAULT_NARROWEST_CD
