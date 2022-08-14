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
		Geometry::Intersection_Data collision__model_vs_segment(const Physical_Model_2D& _model, const glm::vec3& _seg_start, const glm::vec3& _seg_end) const;

	public:
		Geometry::Intersection_Data collision__model_vs_point(const Physical_Model_2D& _model, const glm::vec3& _point) const override;

		Geometry::Intersection_Data collision__model_vs_model(const Physical_Model_2D& _1, const Physical_Model_2D& _2) const override;
		Geometry::Intersection_Data collision__model_vs_model(const Physical_Model_2D::Imprint& _1, const Physical_Model_2D::Imprint& _2) const override;
		Geometry::Intersection_Data collision__model_vs_model(const Physical_Model_2D::Imprint& _impr, const Physical_Model_2D& _model) const override;
		Geometry::Intersection_Data collision__model_vs_model(const Physical_Model_2D& _model, const Physical_Model_2D::Imprint& _impr) const override;

	};

}


#endif // __DEFAULT_NARROWEST_CD
