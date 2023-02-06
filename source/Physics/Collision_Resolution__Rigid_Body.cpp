#include <Physics/Collision_Resolution__Rigid_Body.h>

using namespace LEti;


bool Collision_Resolution__Rigid_Body::resolve(const Physical_Model_2D::Intersection_Data &_id)
{
	LEti::Rigid_Body_2D* bodyA = LV::cast_variable<Rigid_Body_2D>((Object_2D*)_id.first);	//	too lazy to figure out appropriate way to pass non-const pointer here
	LEti::Rigid_Body_2D* bodyB = LV::cast_variable<Rigid_Body_2D>((Object_2D*)_id.second);

	if(!bodyA || !bodyB)
		return false;

	float e = 1.0f;

	glm::vec3 ra = _id.point - bodyA->physics_module()->get_physical_model()->center_of_mass();
	glm::vec3 rb = _id.point - bodyB->physics_module()->get_physical_model()->center_of_mass();

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
			(raPerpDotN * raPerpDotN) / bodyA->physics_module()->get_physical_model()->moment_of_inertia() +
			(rbPerpDotN * rbPerpDotN) / bodyB->physics_module()->get_physical_model()->moment_of_inertia();

	float j = -(1.0f + e) * contactVelocityMag;
	j /= denom;

	glm::vec3 impulse = j * _id.normal;

	float avA = LEti::Math::cross_product(ra, impulse) / bodyA->physics_module()->get_physical_model()->moment_of_inertia();
	float avB = LEti::Math::cross_product(rb, impulse) / bodyB->physics_module()->get_physical_model()->moment_of_inertia();

	bodyA->revert_to_previous_state();
	bodyA->update(_id.time_of_intersection_ratio);
	bodyB->revert_to_previous_state();
	bodyB->update(_id.time_of_intersection_ratio);

	bodyA->move(_id.normal * _id.depth / 2.0f);
	bodyB->move(-_id.normal * _id.depth / 2.0f);

	bodyA->apply_linear_impulse(-impulse / bodyA->mass());
	bodyA->apply_rotation(-avA);
	bodyB->apply_linear_impulse(impulse / bodyB->mass());
	bodyB->apply_rotation(avB);

	return true;
}
