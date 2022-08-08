#ifndef __SPACE_SPLITTER_2D
#define __SPACE_SPLITTER_2D

#include "../Debug.h"

#include <list>
#include "Object.h"

#include "../include/Physics/Broad_Phase_Interface.h"
#include "../include/Physics/Middle_Phase_Interface.h"
#include "../include/Physics/Narrow_Phase_Interface.h"


namespace LEti
{
	class Space_Splitter_2D final		//	TODO: try spatial hashing instead of a quad-tree
	{
	private:
		Space_Splitter_2D() = delete;

	private:
		static std::list<const Object_2D*> m_registred_models;
		static std::list<const glm::vec3*> m_registred_points;

	private:
		static void debug_assert_if_model_copy_found(const Object_2D *_model, bool _reverse);
		static void debug_assert_if_point_copy_found(const glm::vec3* _point, bool _reverse);

	private:
		static Broad_Phase_Interface* m_broad_phase;
		static Middle_Phase_Interface* m_middle_phase;
		static Narrow_Phase_Interface* m_narrow_phase;

	public:
		template<typename Broad_Phase_Realization>
		static void set_broad_phase();
		template<typename Middle_Phase_Realization>
		static void set_middle_phase();
		template<typename Narrow_Phase_Realization>
		static void set_narrow_phase();

		static Broad_Phase_Interface* get_broad_phase();
		static Middle_Phase_Interface* get_middle_phase();
		static Narrow_Phase_Interface* get_narrow_phase();

	public:
		static void register_object(const Object_2D* _model);
		static void unregister_object(const Object_2D* _model);
		static void register_point(const glm::vec3* _point);
		static void unregister_point(const glm::vec3* _point);

	public:
		static void update();

	public:
		static const Narrow_Phase_Interface::Collision_Data_List__Models& get_collisions__models();
		static const Narrow_Phase_Interface::Collision_Data_List__Points& get_collisions__points();

	};


	template<typename Broad_Phase_Realization>
	void Space_Splitter_2D::set_broad_phase()
	{
		delete m_broad_phase;
		m_broad_phase = new Broad_Phase_Realization;
	}

	template<typename Middle_Phase_Realization>
	void Space_Splitter_2D::set_middle_phase()
	{
		delete m_middle_phase;
		m_middle_phase = new Middle_Phase_Realization;
	}

	template<typename Narrow_Phase_Realization>
	void Space_Splitter_2D::set_narrow_phase()
	{
		delete m_narrow_phase;
		m_narrow_phase = new Narrow_Phase_Realization;
	}

}	/*LEti*/

#endif // __Space_Splitter_2D
