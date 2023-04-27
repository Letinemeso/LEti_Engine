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

    unsigned int counted_points = 0;

    for(unsigned int p=0; p<_model.get_polygons_count(); ++p)
    {
        const Polygon& polygon = _model.get_polygons()[p];

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


float Collision_Resolution__Rigid_Body_2D::M_calculate_kinetic_energy(const glm::vec3& _velocity, float _angular_velocity, float _mass, float _moment_of_inertia) const
{
    float velocity = Math::vector_length(_velocity);

    float movemental = (_mass * velocity * velocity) / 2.0f;
    float rotational = (_moment_of_inertia * _angular_velocity * _angular_velocity) / 2.0f;

    return movemental + rotational;
}



bool Collision_Resolution__Rigid_Body_2D::resolve(const Intersection_Data &_id)
{
    LEti::Object_2D* bodyA = (LEti::Object_2D*)_id.first;	//	too lazy to figure out appropriate way to pass non-const pointer here
    LEti::Object_2D* bodyB = (LEti::Object_2D*)_id.second;

    Physics_Module__Rigid_Body_2D* pm1 = LV::cast_variable<Physics_Module__Rigid_Body_2D>((Object_2D*)_id.first->physics_module());
    Physics_Module__Rigid_Body_2D* pm2 = LV::cast_variable<Physics_Module__Rigid_Body_2D>((Object_2D*)_id.second->physics_module());

    if(!pm1 || !pm2)
        return false;

    glm::vec3 A_center_of_mas = M_calculate_center_of_mass(*pm1->get_physical_model());
    glm::vec3 B_center_of_mas = M_calculate_center_of_mass(*pm2->get_physical_model());

    float A_moment_of_inertia = M_calculate_moment_of_inertia(*pm1->get_physical_model(), pm1->mass());
    float B_moment_of_inertia = M_calculate_moment_of_inertia(*pm2->get_physical_model(), pm2->mass());

    glm::vec3 A_velocity = (bodyA->get_pos() - bodyA->get_pos_prev()) / LEti::Event_Controller::get_dt();
    glm::vec3 B_velocity = (bodyB->get_pos() - bodyB->get_pos_prev()) / LEti::Event_Controller::get_dt();
    float A_angular_velocity = (bodyA->get_rotation_angle() - bodyA->get_rotation_angle_prev()) / LEti::Event_Controller::get_dt();
    float B_angular_velocity = (bodyB->get_rotation_angle() - bodyB->get_rotation_angle_prev()) / LEti::Event_Controller::get_dt();

    float ke_before = M_calculate_kinetic_energy(A_velocity, A_angular_velocity, pm1->mass(), A_moment_of_inertia) + M_calculate_kinetic_energy(B_velocity, B_angular_velocity, pm2->mass(), B_moment_of_inertia);

    bodyA->revert_to_previous_state();
    bodyA->update(_id.time_of_intersection_ratio);
    bodyB->revert_to_previous_state();
    bodyB->update(_id.time_of_intersection_ratio);

    float e = 1.0f;

    glm::vec3 ra = _id.point - A_center_of_mas;
    glm::vec3 rb = _id.point - B_center_of_mas;

    glm::vec3 raPerp = {-ra.y, ra.x, 0.0f};
    glm::vec3 rbPerp = {-rb.y, rb.x, 0.0f};

    //	angular linear velocity
    glm::vec3 alvA = raPerp * A_angular_velocity;
    glm::vec3 alvB = rbPerp * B_angular_velocity;

    glm::vec3 relativeVelocity = (B_velocity + alvB) - (A_velocity + alvA);

    float contactVelocityMag = LEti::Math::dot_product(relativeVelocity, _id.normal);

    float raPerpDotN = LEti::Math::dot_product(raPerp, _id.normal);
    float rbPerpDotN = LEti::Math::dot_product(rbPerp, _id.normal);

    float denom = 1 / pm1->mass() + 1 / pm2->mass() +
            (raPerpDotN * raPerpDotN) / A_moment_of_inertia +
            (rbPerpDotN * rbPerpDotN) / B_moment_of_inertia;

    float j = -(1.0f + e) * contactVelocityMag;
    j /= denom;

    glm::vec3 impulse = j * _id.normal;

    float avA = LEti::Math::cross_product(ra, impulse) / A_moment_of_inertia;
    float avB = LEti::Math::cross_product(rb, impulse) / B_moment_of_inertia;

    bodyA->move(_id.normal * (_id.depth + 0.1f) / 2.0f);
    bodyB->move(-_id.normal * (_id.depth + 0.1f) / 2.0f);

    pm1->apply_linear_impulse(-impulse / pm1->mass());
    pm1->apply_rotation(-avA);
    pm2->apply_linear_impulse(impulse / pm2->mass());
    pm2->apply_rotation(avB);

    //  attempt to fix increase of models' velocities after some collisions. and it seems to work fine!
    float ke_after = M_calculate_kinetic_energy(pm1->velocity(), pm1->angular_velocity(), pm1->mass(), A_moment_of_inertia) + M_calculate_kinetic_energy(pm2->velocity(), pm2->angular_velocity(), pm2->mass(), B_moment_of_inertia);

    if(!Math::floats_are_equal(ke_after, 0.0f) && !Math::floats_are_equal(ke_before, 0.0f))
    {
        float ratio_sqrt = sqrtf(ke_before / ke_after);

        pm1->set_velocity(pm1->velocity() * ratio_sqrt);
        pm1->set_angular_velocity(pm1->angular_velocity() * ratio_sqrt);
        pm2->set_velocity(pm2->velocity() * ratio_sqrt);
        pm2->set_angular_velocity(pm2->angular_velocity() * ratio_sqrt);
    }

    return true;
}
