#ifndef __OBJECT_2D
#define __OBJECT_2D

#include "Object_2D_Base.h"

#include "Modules/Default_Draw_Module_2D.h"
#include "Modules/Default_Physics_Module_2D.h"


namespace TEST {

	class Object_2D : public Object_2D_Base
	{
	private:
		Default_Draw_Module_2D* m_draw_module = nullptr;
		Default_Physics_Module_2D* m_physics_module = nullptr;

	public:
		Object_2D();
		~Object_2D();

	public:
		void init(const char* _object_name);

	public:
		void create_draw_module();
		void remove_draw_module();
		Default_Draw_Module_2D* draw_module();
		const Default_Draw_Module_2D* draw_module() const;

		void create_physics_module();
		void remove_physics_module();
		Default_Physics_Module_2D* physics_module();
		const Default_Physics_Module_2D* physics_module() const;

	public:
		void update_prev_state();
		void update(float _ratio = 1.0f) override;
		void draw() const override;

	};

}


#endif // __OBJECT_2D
