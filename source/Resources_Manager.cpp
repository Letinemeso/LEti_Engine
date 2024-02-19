#include "Resources_Manager.h"


using namespace LEti;


Resources_Manager::Resources_Manager()
{

}

Resources_Manager::~Resources_Manager()
{
    clear();
}



void Resources_Manager::add_resource(const std::string& _name, LV::Variable_Base* _resource, bool _rewrite)
{
    L_ASSERT(_resource);
    L_ASSERT(_name.size() > 0);

    Resources_Map::Iterator maybe_loaded_resource_it = m_resources.find(_name);
    if(!maybe_loaded_resource_it.is_ok())
        return m_resources.insert(_name, _resource);

    L_ASSERT(_rewrite);     //  if rewrite is not allowed, program should crash if image with same name already loaded

    delete *maybe_loaded_resource_it;
    *maybe_loaded_resource_it = _resource;
}

void Resources_Manager::remove_resource(const std::string& _name)
{
    Resources_Map::Iterator maybe_loaded_resource_it = m_resources.find(_name);
    L_ASSERT(!maybe_loaded_resource_it.is_ok());

    delete *maybe_loaded_resource_it;
    m_resources.erase(maybe_loaded_resource_it);
}


void Resources_Manager::clear()
{
    for(Resources_Map::Iterator it = m_resources.iterator(); !it.end_reached(); ++it)
        delete *it;

    m_resources.clear();
}





Resources_Manager_Stub::~Resources_Manager_Stub()
{
    for(LV::Variable_Base::Childs_List::Iterator it = resources_stubs.begin(); !it.end_reached(); ++it)
        delete it->child_ptr;
}



BUILDER_STUB_CONSTRUCTION_FUNC(Resources_Manager_Stub) BUILDER_STUB_CONSTRUCTION_FUNC_DEFAULT_IMPL

BUILDER_STUB_INITIALIZATION_FUNC(Resources_Manager_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    for(LV::Variable_Base::Childs_List::Const_Iterator it = resources_stubs.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(LV::cast_variable<LV::Builder_Stub>(it->child_ptr));
        product->add_resource(it->name, ((LV::Builder_Stub*)it->child_ptr)->construct());
    }
}
