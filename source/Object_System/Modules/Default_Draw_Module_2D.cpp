#include "../../../include/Object_System/Modules/Default_Draw_Module_2D.h"

using namespace LEti;


INIT_FIELDS(LEti::Default_Draw_Module_2D_Stub, LEti::Draw_Module_Base_Stub)

ADD_FIELD(std::string, texture_name)

ADD_FIELD(unsigned int, tcoords_count)
ADD_FIELD(float*, tcoords)

ADD_FIELD(unsigned int, coords_count)
ADD_FIELD(float*, coords)

ADD_FIELD(unsigned int, colors_count)
ADD_FIELD(float*, colors)

FIELDS_END


INIT_FIELDS(LEti::Default_Draw_Module_2D, LEti::Draw_Module_Base)
FIELDS_END



Default_Draw_Module_2D_Stub::~Default_Draw_Module_2D_Stub()
{
    delete[] coords;
    delete[] tcoords;
    delete[] colors;
}



LV::Variable_Base* Default_Draw_Module_2D_Stub::M_construct_product() const
{
    return new Default_Draw_Module_2D;
}

void Default_Draw_Module_2D_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    Default_Draw_Module_2D* result = (Default_Draw_Module_2D*)_product;

    result->init_vertices(coords, coords_count);
    result->init_colors(colors, colors_count);
    result->init_texture(LEti::Picture_Manager::get_picture(texture_name), tcoords, tcoords_count);
}



Default_Draw_Module_2D::Default_Draw_Module_2D() : Draw_Module_Base()
{
	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

	m_vertices.vertex_array = &m_vertex_array;
	m_texture.vertex_array = &m_vertex_array;
    m_colors.vertex_array = &m_vertex_array;
}

Default_Draw_Module_2D::~Default_Draw_Module_2D()
{
	glDeleteVertexArrays(1, &m_vertex_array);
}



void Default_Draw_Module_2D::init_vertices(const float *const _coords, unsigned int _coords_count)
{
    glBindVertexArray(m_vertex_array);
    m_vertices.init(_coords, _coords_count);
    m_vertices.setup_buffer(0, 3);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}

void Default_Draw_Module_2D::init_colors(const float *const _colors, unsigned int _colors_count)
{
    glBindVertexArray(m_vertex_array);
    m_colors.init(_colors, _colors_count);
    m_colors.setup_buffer(1, 4);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
}

void Default_Draw_Module_2D::init_texture(const Picture* _picture, const float *const tex_coords, unsigned int _tex_coords_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.init(_picture, tex_coords, _tex_coords_count);
    m_texture.setup_buffer(2, 2);		//TODO: this data shpuld not be hard-coded. it should be stored in LEti::Shader (probably)
    m_texture.reconfigure_texture_coords();
}


void Default_Draw_Module_2D::set_texture(const Picture* _picture)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_picture(_picture);
}

void Default_Draw_Module_2D::set_texture_coords(const float* _tc, unsigned int _tc_count)
{
	glBindVertexArray(m_vertex_array);
	m_texture.set_texture_coords(_tc, _tc_count);
    m_texture.setup_buffer(2, 2);
}


void Default_Draw_Module_2D::move_raw(const glm::vec3 &_stride)
{
    for(unsigned int i=0; i<m_vertices.size(); i += 3)
    {
        m_vertices[i] += _stride.x;
        m_vertices[i + 1] += _stride.y;
        m_vertices[i + 2] += _stride.z;
    }
}
