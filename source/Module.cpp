#include <Module.h>

#include <Object.h>

using namespace LEti;


Module::Module()
{

}

Module::~Module()
{

}



void Module::assign_same_parent(Module& _to) const
{
    _to.set_transformation_data(m_transformation_data);
    _to.set_transformation_data_prev_state(m_transformation_data_prev_state);
    _to.set_parent_object(m_parent_object);
}



void Module::M_notify_others(const std::string& _notification_id, Module* _notifier)
{
    L_ASSERT(parent_object());

    const Object::Modules_Vec& modules = parent_object()->modules();
    for(unsigned int i = 0; i < modules.size(); ++i)
    {
        Module* module = modules[i];
        if(module == this)
            continue;

        module->notify(_notification_id, _notifier);
    }
}
