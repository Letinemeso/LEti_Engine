#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "../../OpenGL/GLM/vec3.hpp"
#include "../../OpenGL/GLM/mat4x4.hpp"

#include "../../include/Math_Stuff.h"
#include "../../include/Tree.h"

#include "../../Debug.h"


namespace LEti
{
	class Physical_Model_2D
	{
	public:
		class Imprint final
		{
		private:
			friend class Physical_Model_2D;
			const Physical_Model_2D* m_parent = nullptr;

		private:
			Geometry_2D::Polygon* m_polygons = nullptr;
			unsigned int m_polygons_count = 0;
			Geometry_2D::Rectangular_Border m_rect_border;

		private:
			Imprint(const Geometry_2D::Polygon* _polygons, unsigned int _polygons_count, const Physical_Model_2D* _parent);

		public:
			Imprint(Imprint&& _other);
			Imprint(const Imprint& _other);
			~Imprint();

		private:
			void update_rectangular_border();

		public:
			void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
			void update_with_single_matrix(const glm::mat4x4 _matrix);
			void update_to_current_model_state();

			const Geometry_2D::Polygon& operator[](unsigned int _index) const;
			const Physical_Model_2D* get_parent() const;
			unsigned int get_polygons_count() const;
			const Geometry_2D::Rectangular_Border& curr_rect_border() const;

		};

	private:
		float* m_raw_coords = nullptr;
		unsigned int m_raw_coords_count = 0;

		Geometry_2D::Polygon* m_polygons = nullptr;
		unsigned int m_polygons_count = 0;

	private:
		Geometry_2D::Rectangular_Border m_current_border;

	private:
		void update_rectangular_border();

	public:
		const Geometry_2D::Rectangular_Border& curr_rect_border() const;

	public:
		Physical_Model_2D();
		Physical_Model_2D(const float* _raw_coords, unsigned int _raw_coords_count);
		Physical_Model_2D(const Physical_Model_2D& _other);
		void setup(const float* _raw_coords, unsigned int _raw_coords_count);

		~Physical_Model_2D();

		void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void copy_real_coordinates(const Physical_Model_2D& _other);

		Imprint create_imprint() const;

	public:
		unsigned int get_polygons_count() const;
		const Geometry_2D::Polygon& operator[](unsigned int _index) const;

	};
}

#endif // __PHYSICAL_MODEL_2D
