#include <Physics/Collision_Resolution__Rigid_Body_2D.h>

using namespace LEti;


glm::vec3 Collision_Resolution__Rigid_Body_2D::M_calculate_center_of_mass(const Physical_Model_2D &_model) const
{
	glm::vec3 result(0.0f, 0.0f, 0.0f);

	for(unsigned int i=0; i<_model.get_polygons_count(); ++i)
		result += _model.get_polygons()[i].center();
	result /= (float)_model.get_polygons_count();

	return result;
}

float Collision_Resolution__Rigid_Body_2D::M_calculate_moment_of_inertia(const Physical_Model_2D &_model, float _mass) const
{
	float result = 0.0f;

	glm::vec3 center_of_mass = M_calculate_center_of_mass(_model);

//    if(_model.get_polygons_count() == 1)
//    {
//        for(unsigned int v=0; v < 3; ++v)
//        {
//            glm::vec3 distance_vec = M_calculate_center_of_mass(_model) - _model.get_polygons()[0][v];
//            float distance_squared = (distance_vec.x * distance_vec.x) + (distance_vec.y * distance_vec.y) + (distance_vec.z * distance_vec.z);
//            result += distance_squared;
//        }
//        result *= _mass;
//        result /= _model.get_polygons_count() * 3;
//    }
//    else
//    {
//        for(unsigned int i=0; i<_model.get_polygons_count(); ++i)
//        {
//            glm::vec3 distance_vec = center_of_mass - _model.get_polygons()[i].center();

//            result += (distance_vec.x * distance_vec.x) + (distance_vec.y * distance_vec.y) + (distance_vec.z * distance_vec.z);
//        }
//        result *= _mass;
//        result /= _model.get_polygons_count();
//    }


    unsigned int counted_points = 0;

    for(unsigned int p=0; p<_model.get_polygons_count(); ++p)
    {
        const Geometry::Polygon& polygon = _model.get_polygons()[p];

        for(unsigned int v=0; v < 3; ++v)
        {
            if(!polygon.segment_can_collide(v))
                continue;

            glm::vec3 center_to_point = center_of_mass - polygon[v];
            float distance_squared = (center_to_point.x * center_to_point.x) + (center_to_point.y * center_to_point.y) + (center_to_point.z * center_to_point.z);
            result += distance_squared;

            ++counted_points;
        }
    }
    result *= _mass;
    result /= (float)counted_points;

	return result;
}



bool Collision_Resolution__Rigid_Body_2D::resolve(const Physical_Model_2D::Intersection_Data &_id)
{
	LEti::Rigid_Body_2D* bodyA = LV::cast_variable<Rigid_Body_2D>((Object_2D*)_id.first);	//	too lazy to figure out appropriate way to pass non-const pointer here
	LEti::Rigid_Body_2D* bodyB = LV::cast_variable<Rigid_Body_2D>((Object_2D*)_id.second);

	bodyA->revert_to_previous_state();
	bodyA->update(_id.time_of_intersection_ratio);
	bodyB->revert_to_previous_state();
	bodyB->update(_id.time_of_intersection_ratio);

	if(!bodyA || !bodyB)
		return false;

	float e = 1.0f;

	glm::vec3 A_center_of_mas = M_calculate_center_of_mass(*bodyA->physics_module()->get_physical_model());
	glm::vec3 B_center_of_mas = M_calculate_center_of_mass(*bodyB->physics_module()->get_physical_model());

	float A_moment_of_inertia = M_calculate_moment_of_inertia(*bodyA->physics_module()->get_physical_model(), bodyA->mass());
	float B_moment_of_inertia = M_calculate_moment_of_inertia(*bodyB->physics_module()->get_physical_model(), bodyA->mass());

	glm::vec3 ra = _id.point - A_center_of_mas;
	glm::vec3 rb = _id.point - B_center_of_mas;

	glm::vec3 raPerp = {-ra.y, ra.x, 0.0f};
	glm::vec3 rbPerp = {-rb.y, rb.x, 0.0f};

	//	angular linear velocity
	glm::vec3 alvA = raPerp * bodyA->angular_velocity();
	glm::vec3 alvB = rbPerp * bodyB->angular_velocity();

	glm::vec3 relativeVelocity = (bodyB->velocity() + alvB) - (bodyA->velocity() + alvA);

	float contactVelocityMag = LEti::Math::dot_product(relativeVelocity, _id.normal);

	float raPerpDotN = LEti::Math::dot_product(raPerp, _id.normal);
	float rbPerpDotN = LEti::Math::dot_product(rbPerp, _id.normal);

	float denom = 1 / bodyA->mass() + 1 / bodyB->mass() +
			(raPerpDotN * raPerpDotN) / A_moment_of_inertia +
			(rbPerpDotN * rbPerpDotN) / B_moment_of_inertia;

	float j = -(1.0f + e) * contactVelocityMag;
	j /= denom;

	glm::vec3 impulse = j * _id.normal;

	float avA = LEti::Math::cross_product(ra, impulse) / A_moment_of_inertia;
	float avB = LEti::Math::cross_product(rb, impulse) / B_moment_of_inertia;

	bodyA->move(_id.normal * _id.depth / 2.0f);
	bodyB->move(-_id.normal * _id.depth / 2.0f);

	bodyA->apply_linear_impulse(-impulse / bodyA->mass());
	bodyA->apply_rotation(-avA);
	bodyB->apply_linear_impulse(impulse / bodyB->mass());
	bodyB->apply_rotation(avB);

	return true;
}
