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

    class Polygon_Holder_Base
    {
    protected:
        Polygon* polygons = nullptr;

    public:
        Polygon_Holder_Base() { }
        ~Polygon_Holder_Base() { delete[] polygons; }

    public:
        virtual Polygon_Holder_Base* create_copy() const = 0;
        virtual void allocate(unsigned int _amount) = 0;
        virtual Polygon* get_polygon(unsigned int _index) = 0;
        virtual const Polygon* get_polygon(unsigned int _index) const = 0;

    };

    template<typename T>
    class Polygon_Holder final : public Polygon_Holder_Base
    {
    private:
        inline T* M_cast() { return (T*)polygons; }
        inline const T* M_cast() const { return (const T*)polygons; }

    public:
        inline Polygon_Holder_Base* create_copy() const override { return new Polygon_Holder<T>; }
        void allocate(unsigned int _amount) override { delete[] polygons;  polygons = new T[_amount]; }
        inline Polygon* get_polygon(unsigned int _index) { return &M_cast()[_index]; }
        inline const Polygon* get_polygon(unsigned int _index) const { return &M_cast()[_index]; }

    };


    class Physical_Model_2D_Imprint;

	class Physical_Model_2D
    {
	private:
		float* m_raw_coords = nullptr;
		unsigned int m_raw_coords_count = 0;

		bool* m_collision_permissions = nullptr;

        Polygon_Holder_Base* m_polygons_holder = nullptr;
		unsigned int m_polygons_count = 0;

    private:
        glm::vec3 m_center_of_mass{0.0f, 0.0f, 0.0f};

	private:
		Geometry_2D::Rectangular_Border m_current_border;

    private:
        virtual Polygon_Holder_Base* M_create_polygons_holder() const;

	private:
		void M_update_rectangular_border();
        virtual glm::vec3 M_calculate_center_of_mass() const;

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
        const Polygon* get_polygon(unsigned int _index) const;
        const Polygon_Holder_Base* get_polygons() const;
        unsigned int get_polygons_count() const;

        inline const glm::vec3& center_of_mass() const { return m_center_of_mass; }

	};

    class Physical_Model_2D_Imprint
    {
    private:
        const Physical_Model_2D* m_parent = nullptr;

    private:
        Polygon_Holder_Base* m_polygons_holder = nullptr;
        unsigned int m_polygons_count = 0;
        Geometry_2D::Rectangular_Border m_rect_border;

    public:
        Physical_Model_2D_Imprint(const Physical_Model_2D* _parent);
        Physical_Model_2D_Imprint(Physical_Model_2D_Imprint&& _other);
        Physical_Model_2D_Imprint(const Physical_Model_2D_Imprint& _other);
        ~Physical_Model_2D_Imprint();

    private:
        void M_update_rectangular_border();

    public:
        void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale);
        void update_with_single_matrix(const glm::mat4x4& _matrix);
        void update_to_current_model_state();

        const Physical_Model_2D* get_parent() const;
        const Polygon* get_polygon(unsigned int _index) const;
        const Polygon_Holder_Base* get_polygons() const;
        unsigned int get_polygons_count() const;
        const Geometry_2D::Rectangular_Border& curr_rect_border() const;

    };
}

#endif // __PHYSICAL_MODEL_2D
