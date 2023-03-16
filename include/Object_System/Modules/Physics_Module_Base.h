#ifndef __PHYSICS_MODULE_BASE
#define __PHYSICS_MODULE_BASE

#include <mat4x4.hpp>

#include <Variable_Base.h>


namespace LEti
{

    class Physics_Module_Base_Stub : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    public:
        virtual ~Physics_Module_Base_Stub() { }

    };

	class Physics_Module_Base
    {
    private:
        bool m_can_collide = true;

	public:
		Physics_Module_Base();
		virtual ~Physics_Module_Base();

        virtual void init(const Physics_Module_Base_Stub* _stub) { }

	public:
		virtual void update_previous_state() = 0;
        virtual void update(const glm::mat4x4& _translation, const glm::mat4x4& _rotation, const glm::mat4x4& _scale) = 0;

    public:
        inline void set_can_collide(bool _can_collide) { m_can_collide = _can_collide; }
        inline bool can_collide() const { return m_can_collide; }

	};

}


#endif // __PHYSICS_MODULE_BASE
