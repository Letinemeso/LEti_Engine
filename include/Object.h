#ifndef __OBJECT
#define __OBJECT

#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/Vertices.h"
#include "../include/Camera.h"

#include "../OpenGL/GLM/mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"


namespace LEti {

	class Object
	{
	private:
		unsigned int vertex_array = 0;
		unsigned int buffer[2] = { 0 };

		LEti::Texture texture;
		LEti::Vertices vertices;

		glm::mat4x4 translation_matrix, rotation_matrix, scale_matrix;
		glm::vec3 rotation_axis;
		float rotation_angle = 0.0f;

	public:
		Object();
		~Object();

	public:
		void init_texture(const char* _tex_path, float* tex_coords, unsigned int _tex_coords_count);
		void init_vertices(float* _coords, unsigned int _coords_count);

	public:
		void draw() const;
		void update(float _dt);

	public:
		void set_pos(float _x, float _y, float _z);
		void move(float _x, float _y, float _z);

		void set_rotation_axis(float _x, float _y, float _z);
		void set_rotation_angle(float _angle);
		void set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle);
		void rotate(float _angle);

		void set_scale(float _scale_x, float _scale_y, float _scale_z);
		void set_overall_scale(float _scale);

	};

}	//LEti

#endif