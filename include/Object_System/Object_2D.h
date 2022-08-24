#ifndef __OBJECT_2D
#define __OBJECT_2D

#include "Object_2D_Base.h"

#include "Modules/Default_Draw_Module_2D.h"
#include "Modules/Default_Physics_Module_2D.h"


namespace LEti {

	class Object_2D : public Object_2D_Base
	{
	protected:
		Default_Draw_Module_2D* m_draw_module = nullptr;
		Default_Physics_Module_2D* m_physics_module = nullptr;

	public:
		Object_2D();
		~Object_2D();

	public:
		virtual void init(const char* _object_name);

	public:
		virtual void create_draw_module();
		void remove_draw_module();
		Default_Draw_Module_2D* draw_module();
		const Default_Draw_Module_2D* draw_module() const;

		virtual void create_physics_module();
		void remove_physics_module();
		Default_Physics_Module_2D* physics_module();
		const Default_Physics_Module_2D* physics_module() const;

	public:
		virtual void revert_to_previous_state();
		void update_previous_state() override;
		void update(float _ratio = 1.0f) override;
		void draw() const override;

	};

}


#endif // __OBJECT_2D
