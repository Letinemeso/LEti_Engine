#include <Renderer.h>

using namespace LEti;


Renderer::Renderer()
{

}

Renderer::Renderer(const Renderer& _other)
{
    m_camera = _other.m_camera;
    m_shader = _other.m_shader;
}

void Renderer::operator=(const Renderer& _other)
{
    m_camera = _other.m_camera;
    m_shader = _other.m_shader;
}


Renderer::~Renderer()
{

}



void Renderer::draw(const Draw_Module_Base &_what) const
{
    if (!_what.visible())
        return;

    L_ASSERT(_what.vertex_array() != 0);

    m_shader->set_projection_matrix(m_camera->matrix());
    m_shader->set_transform_matrix(_what.transform_matrix());
    m_shader->set_texture(_what.texture());

    glBindVertexArray(_what.vertex_array());

    glDrawArrays(_what.gl_draw_mode(), 0, _what.vertices().vertices_count());

    glBindVertexArray(0);
}
