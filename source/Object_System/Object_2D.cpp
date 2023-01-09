#include <Object_System/Object_2D.h>

using namespace LEti;


Object_2D::Object_2D() : Object_2D_Base()
{

}

Object_2D::~Object_2D()
{
	remove_draw_module();
	remove_physics_module();
}



void Object_2D::init(const char *_object_name)
{
	remove_draw_module();
	remove_physics_module();

	auto translation = LEti::Resource_Loader::get_data<float>(_object_name, "position");
	ASSERT(translation.second != 3);
	set_pos({translation.first[0], translation.first[1], translation.first[2]});

	auto scale = LEti::Resource_Loader::get_data<float>(_object_name, "scale");
	ASSERT(scale.second != 3);
	set_scale({scale.first[0], scale.first[1], scale.first[2]});

	auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
	ASSERT(raxis.second != 3);
	set_rotation_axis({raxis.first[0], raxis.first[1], raxis.first[2]});

	auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
	ASSERT(rangle.second != 1);
	set_rotation_angle(*rangle.first);

	std::pair<const float*, unsigned int> tcoords;
	LEti::Resource_Loader::assign<float>(tcoords, _object_name, "texture_coords");

	if(tcoords.first)
	{
		create_draw_module();
		m_draw_module->init_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "texture_name").first->c_str(), tcoords.first, tcoords.second);
		auto coords = LEti::Resource_Loader::get_data<float>(_object_name, "coords");
		m_draw_module->init_vertices(coords.first, coords.second);
	}

	std::pair<const float*, unsigned int> physical_model_data;
	LEti::Resource_Loader::assign(physical_model_data, _object_name, "physical_model_data");

	if((physical_model_data.first))
	{
		create_physics_module();
		m_physics_module->init(physical_model_data.first, physical_model_data.second);
	}
}



void Object_2D::create_draw_module()
{
	remove_draw_module();
	m_draw_module = new Default_Draw_Module_2D;
}

void Object_2D::remove_draw_module()
{
	delete m_draw_module;
	m_draw_module = nullptr;
}

Default_Draw_Module_2D* Object_2D::draw_module()
{
	return m_draw_module;
}

const Default_Draw_Module_2D* Object_2D::draw_module() const
{
	return m_draw_module;
}


void Object_2D::create_physics_module()
{
	remove_physics_module();
	m_physics_module = new Default_Physics_Module_2D;
}

void Object_2D::remove_physics_module()
{
	delete m_physics_module;
	m_physics_module = nullptr;
}

Default_Physics_Module_2D* Object_2D::physics_module()
{
	return m_physics_module;
}

const Default_Physics_Module_2D* Object_2D::physics_module() const
{
	return m_physics_module;
}



void Object_2D::revert_to_previous_state()
{
	m_current_state = m_previous_state;
}

void Object_2D::update_previous_state()
{
	Object_2D_Base::update_previous_state();

	if(!m_physics_module) return;

	m_physics_module->update_previous_state();
}

void Object_2D::update(float _ratio)
{
	if(!m_physics_module) return;

	Transformation_Data gtd = get_global_transformation();
	m_physics_module->update(gtd.translation_matrix, gtd.rotation_matrix, gtd.scale_matrix);
}

void Object_2D::draw() const
{
	if(!m_draw_module) return;

	Transformation_Data gtd = get_global_transformation();
	m_draw_module->draw(gtd.translation_matrix, gtd.rotation_matrix, gtd.scale_matrix);
}
