#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "vec3.hpp"
#include "mat4x4.hpp"

#include "Data_Structures/List.h"

#include "../../include/Math_Stuff.h"

#include "Debug/Debug.h"


namespace LEti
{
	class Object_2D;

	class Physical_Model_2D
	{
	public:
		struct Intersection_Data
		{
			enum class Type
			{
				none = 0,
				intersection,
				same_line
			};
			Type type = Type::none;
			glm::vec3 point{0.0f, 0.0f, 0.0f};	//	TODO: make this shit return only "points" \|/
			glm::vec3 normal{0.0f, 0.0f, 0.0f};
			const Object_2D* first = nullptr, *second = nullptr;
			float time_of_intersection_ratio = 1.0f;

			LDS::List<glm::vec3> points;

			Intersection_Data() { }
			Intersection_Data(Type _type) : type(_type) { }
			Intersection_Data(Type _type, const glm::vec3& _point) : type(_type), point(_point) { }
			Intersection_Data(const Intersection_Data& _other) : type(_other.type), point(_other.point), normal(_other.normal),
				first(_other.first), second(_other.second), time_of_intersection_ratio(_other.time_of_intersection_ratio), points(_other.points) { }
			Intersection_Data(Intersection_Data&& _other) : type(_other.type), point(_other.point), normal(_other.normal),
				first(_other.first), second(_other.second), time_of_intersection_ratio(_other.time_of_intersection_ratio), points(_other.points) { }
			void operator=(const Intersection_Data& _other) { type = _other.type; point = _other.point; time_of_intersection_ratio = _other.time_of_intersection_ratio;
															  normal = _other.normal; first = _other.first; second = _other.second; points = _other.points; }
			operator bool() { return type != Type::none; }
		};

	public:
		class Imprint final
		{
		private:
			friend class Physical_Model_2D;
			const Physical_Model_2D* m_parent = nullptr;

		private:
			Geometry::Polygon* m_polygons = nullptr;
			unsigned int m_polygons_count = 0;
			Geometry_2D::Rectangular_Border m_rect_border;

			glm::vec3 m_center_of_mass_raw, m_center_of_mass;

		private:
			Imprint(const Geometry::Polygon* _polygons, unsigned int _polygons_count, const Physical_Model_2D* _parent);

		public:
			Imprint(Imprint&& _other);
			Imprint(const Imprint& _other);
			~Imprint();

		private:
			void update_rectangular_border();

		public:
			void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
			void update_with_single_matrix(const glm::mat4x4& _matrix);
			void update_to_current_model_state();

			const Geometry::Polygon& operator[](unsigned int _index) const;
			const Physical_Model_2D* get_parent() const;
			inline const Geometry::Polygon* get_polygons() const { return m_polygons; }
			unsigned int get_polygons_count() const;
			const Geometry_2D::Rectangular_Border& curr_rect_border() const;
			const glm::vec3& center_of_mass() const;

		};

	private:
		float* m_raw_coords = nullptr;
		unsigned int m_raw_coords_count = 0;

		Geometry::Polygon* m_polygons = nullptr;
		unsigned int m_polygons_count = 0;

		glm::vec3 m_center_of_mass_raw, m_center_of_mass;
		float m_moment_of_inertia = 0.0f;

	private:
		Geometry_2D::Rectangular_Border m_current_border;

	private:
		void update_rectangular_border();
		void update_moment_of_inertia();

	public:
		const Geometry_2D::Rectangular_Border& curr_rect_border() const;

	public:
		Physical_Model_2D();
		Physical_Model_2D(const Physical_Model_2D& _other);
		void setup(const float* _raw_coords, unsigned int _raw_coords_count);

		~Physical_Model_2D();

		void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void copy_real_coordinates(const Physical_Model_2D& _other);

		Imprint create_imprint() const;

	public:
		const Geometry::Polygon* get_polygons() const;
		unsigned int get_polygons_count() const;
		const Geometry::Polygon& operator[](unsigned int _index) const;
		const glm::vec3& center_of_mass() const;
		float moment_of_inertia() const;

	};
}

#endif // __PHYSICAL_MODEL_2D
