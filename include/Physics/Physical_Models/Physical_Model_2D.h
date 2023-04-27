#ifndef __PHYSICAL_MODEL_2D
#define __PHYSICAL_MODEL_2D

#include "vec3.hpp"
#include "mat4x4.hpp"

#include "L_Debug/L_Debug.h"
#include "Data_Structures/List.h"

#include <Math_Stuff.h>
#include <Physics/Physical_Models/Polygon.h>


namespace LEti
{

    class Physical_Model_2D_Imprint;

	class Physical_Model_2D
    {
	private:
		float* m_raw_coords = nullptr;
		unsigned int m_raw_coords_count = 0;

		bool* m_collision_permissions = nullptr;

        Polygon* m_polygons = nullptr;
		unsigned int m_polygons_count = 0;

	private:
		Geometry_2D::Rectangular_Border m_current_border;

	private:
		void M_update_rectangular_border();

	public:
		const Geometry_2D::Rectangular_Border& curr_rect_border() const;

	public:
		Physical_Model_2D();
		Physical_Model_2D(const Physical_Model_2D& _other);
		void setup(const float* _raw_coords, unsigned int _raw_coords_count, const bool* _collision_permissions);
		void move_raw(const glm::vec3& _stride);

		~Physical_Model_2D();

		void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
		void copy_real_coordinates(const Physical_Model_2D& _other);

        Physical_Model_2D_Imprint* create_imprint() const;

	public:
        const Polygon* get_polygons() const;
		unsigned int get_polygons_count() const;
        const Polygon& operator[](unsigned int _index) const;

	};

    class Physical_Model_2D_Imprint
    {
    private:
        const Physical_Model_2D* m_parent = nullptr;

    private:
        Polygon* m_polygons = nullptr;
        unsigned int m_polygons_count = 0;
        Geometry_2D::Rectangular_Border m_rect_border;

    public:
        Physical_Model_2D_Imprint(const Polygon* _polygons, unsigned int _polygons_count, const Physical_Model_2D* _parent);
        Physical_Model_2D_Imprint(Physical_Model_2D_Imprint&& _other);
        Physical_Model_2D_Imprint(const Physical_Model_2D_Imprint& _other);
        ~Physical_Model_2D_Imprint();

    private:
        void M_update_rectangular_border();

    public:
        void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
        void update_with_single_matrix(const glm::mat4x4& _matrix);
        void update_to_current_model_state();

        const Polygon& operator[](unsigned int _index) const;
        const Physical_Model_2D* get_parent() const;
        inline const Polygon* get_polygons() const { return m_polygons; }
        unsigned int get_polygons_count() const;
        const Geometry_2D::Rectangular_Border& curr_rect_border() const;

    };
}

#endif // __PHYSICAL_MODEL_2D
