#ifndef __OBJECT
#define __OBJECT

#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/Vertices.h"
#include "../include/Camera.h"
#include "../include/Resource_Loader.h"
#include "../include/Buffer.h"

#include "../OpenGL/GLM/mat4x4.hpp"
#include "../OpenGL/GLM/gtx/transform.hpp"

#include "../include/Physical_Model_2D.h"
#include "../include/Physical_Model_3D.h"


namespace LEti {

	class Object_Interface
	{
	private:
		bool m_visible = true;

	public:
		virtual void draw() const = 0;
		virtual void update() = 0;

	public:
		virtual void set_visible(bool _visible);
		bool get_visible() const;

	public:
		Object_Interface() { }
		Object_Interface(const Object_Interface& /*_other*/) { }
		Object_Interface(Object_Interface&& /*_from*/) { }
		virtual ~Object_Interface() { }

	public:
		virtual void set_pos(float _x, float _y, float _z) = 0;
		virtual void move(float _x, float _y, float _z) = 0;

		virtual void set_rotation_axis(float _x, float _y, float _z) = 0;
		virtual void set_rotation_angle(float _angle) = 0;
		virtual void set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle) = 0;
		virtual void rotate(float _angle) = 0;

		virtual void set_scale(float _scale_x, float _scale_y, float _scale_z) = 0;
		virtual void set_overall_scale(float _scale) = 0;

	public:
		virtual glm::vec3 get_pos() const = 0;
		virtual glm::vec3 get_scale() const = 0;
		virtual glm::vec3 get_rotation_axis() const = 0;
		virtual float get_rotation_angle() const = 0;

	};



	class Drawable_Object : public Object_Interface
	{
	protected:
		unsigned int m_vertex_array = 0;

		LEti::Texture m_texture;
		LEti::Vertices m_vertices;

		glm::mat4x4 m_translation_matrix, m_rotation_matrix, m_scale_matrix;
		glm::vec3 m_rotation_axis;
		float m_rotation_angle = 0.0f;

		struct
		{
			glm::mat4x4 translation_matrix, rotation_matrix, scale_matrix;
			glm::vec3 rotation_axis;
			float rotation_angle = 0.0f;
			void update(const glm::mat4x4& _translation_matrix, const glm::mat4x4& _rotation_matrix, const glm::mat4x4& _scale_matrix,
						const glm::vec3& _rotation_axis, float _rotation_angle)
				{ translation_matrix = _translation_matrix; rotation_matrix = _rotation_matrix; scale_matrix = _scale_matrix; rotation_axis = _rotation_axis; rotation_angle = _rotation_angle; }
		} m_previous_state;

	public:
		glm::mat4x4 get_translation_matrix_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_rotation_matrix_for_time_ratio(float _ratio) const;
		glm::mat4x4 get_scale_matrix_for_time_ratio(float _ratio) const;

	public:
		Drawable_Object();
		virtual ~Drawable_Object();

	public:
		void init_texture(const char* _tex_path, const float* const tex_coords, unsigned int _tex_coords_count);
		void init_vertices(const float* const _coords, unsigned int _coords_count);
		virtual void init(const char* _object_name);

		LEti::Vertices& get_vertices();
		LEti::Texture& get_texture();

	public:
		void set_texture(const char* _name);
		void set_texture_coords(const float* _tc, unsigned int _tc_count);

	public:
		virtual void update_previous_state();

	public:
        virtual void draw() const override = 0;
		virtual void update() override = 0;

	public:
		void set_pos(float _x, float _y, float _z) override;
		void move(float _x, float _y, float _z) override;

		void set_rotation_axis(float _x, float _y, float _z) override;
		void set_rotation_angle(float _angle) override;
		void set_rotation_data(float _axis_x, float _axis_y, float _axis_z, float _angle) override;
		void rotate(float _angle) override;

		void set_scale(float _scale_x, float _scale_y, float _scale_z) override;
		void set_overall_scale(float _scale) override;

	public:
		glm::vec3 get_pos() const override;
		glm::vec3 get_scale() const override;
		glm::vec3 get_rotation_axis() const override;
		float get_rotation_angle() const override;

		glm::vec3 get_pos_prev() const;
		glm::vec3 get_scale_prev() const;
		glm::vec3 get_rotation_axis_prev() const;
		float get_rotation_angle_prev() const;

		bool moved_since_last_frame() const;

	};





	class Object_2D : public Drawable_Object
	{
	public:
	    Object_2D();
	    virtual ~Object_2D();

	    void init_physical_model(const float* _coords, unsigned int _coords_count);

	    virtual void init(const char* _object_name) override;

	private:
	    bool m_can_cause_collision = false;
		Physical_Model_2D* m_physical_model = nullptr;
		Physical_Model_2D::Imprint* m_physical_model_prev_state = nullptr;
	    Physical_Model_2D::Rectangular_Border m_dynamic_rb;
		bool m_is_dynamic = false;

	public:
	    bool is_dynamic() const;
	    void set_dynamic(bool _is_dynamic);

	    void set_can_cause_collision(bool _can_cause_collision);
	    bool get_collision_possibility() const;

	public:
		virtual void draw() const override;
		virtual void update() override;
		virtual void update_previous_state() override;

	public:
	    Physical_Model_2D* get_physical_model();
		Physical_Model_2D::Imprint* get_physical_model_prev_state();
	public:
	    const Physical_Model_2D* get_physical_model() const;
		const Physical_Model_2D::Imprint* get_physical_model_prev_state() const;
		const Physical_Model_2D::Rectangular_Border& get_dynamic_rb() const;

//	private:
//		static Geometry::Intersection_Data get_precise_time_ratio_of_collision(const Object_2D& _first, const Object_2D& _second, float _min_ratio, float _max_ratio, unsigned int _precision);
//		static Geometry::Intersection_Data collision__moving_vs_moving(const Object_2D& _moving_1, const Object_2D& _moving_2);
//		static Geometry::Intersection_Data collision__moving_vs_static(const Object_2D& _moving, const Object_2D& _static);
//		static Geometry::Intersection_Data collision__static_vs_static(const Object_2D& _moving, const Object_2D& _static);

//	public:
//	    Geometry::Intersection_Data is_colliding_with_other(const Object_2D& _other) const;

	};


/*
	class Object_3D : public Drawable_Object
	{
	public:
		Object_3D();
		virtual ~Object_3D();

        void init_physical_model(const float* _coords, unsigned int _coords_count) override;

        virtual void init(const char* _object_name) override;

	public:
		virtual void draw() const override;
		virtual void update() override;
	};
*/

}	//LEti

#endif
