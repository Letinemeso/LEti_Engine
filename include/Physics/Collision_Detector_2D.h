#ifndef __Collision_Detector_2D
#define __Collision_Detector_2D

#include "L_Debug/L_Debug.h"
#include <Data_Structures/List.h>

#include <Object_System/Object_2D.h>

#include <Physics/Broad_Phase_Interface.h>
#include <Physics/Narrow_Phase_Interface.h>


namespace LEti
{
	class Collision_Detector_2D
	{
	public:
		Collision_Detector_2D();
		virtual ~Collision_Detector_2D();

	private:
        LDS::List<const LEti::Object_2D*> m_registred_models;
        LDS::List<const glm::vec3*> m_registred_points;

	private:
		void debug_assert_if_model_copy_found(const LEti::Object_2D *_model, bool _reverse);
		void debug_assert_if_point_copy_found(const glm::vec3* _point, bool _reverse);

	private:
		Broad_Phase_Interface* m_broad_phase = nullptr;
		Narrow_Phase_Interface* m_narrow_phase = nullptr;

	public:
		void set_broad_phase(Broad_Phase_Interface* _broad_phase_impl, unsigned int _precision);
		void set_narrow_phase(Narrow_Phase_Interface* _narrow_phase_impl, unsigned int _precision);
		void set_narrowest_phase(Narrowest_Phase_Interface* _narrowest_phase_impl);

	public:
		void register_object(const LEti::Object_2D* _model);
		void unregister_object(const LEti::Object_2D* _model);
		void unregister_all_objects();
		void register_point(const glm::vec3* _point);
		void unregister_point(const glm::vec3* _point);
		void unregister_all_points();

	public:
		void update();

	public:
		const Narrow_Phase_Interface::Collision_Data_List__Models& get_collisions__models();
		const Narrow_Phase_Interface::Collision_Data_List__Points& get_collisions__points();

	};

}	/*LEti*/

#endif // __Collision_Detector_2D
