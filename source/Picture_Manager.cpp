#include "Picture_Manager.h"


INIT_FIELDS(LEti::Picture_Manager::Picture_Autoload_Stub, LV::Variable_Base)

ADD_FIELD(unsigned int, amount)
ADD_FIELD(std::string*, names)
ADD_FIELD(std::string*, paths)

FIELDS_END



namespace LEti
{

	namespace Picture_Manager
	{

		void Picture_Autoload_Stub::M_on_values_assigned()
		{
			for(unsigned int i=0; i<amount; ++i)
			{
				Picture* picture = LEti::load_picture(paths[i].c_str());
				add_picture(names[i], picture);
			}
		}

		Picture_Autoload_Stub::~Picture_Autoload_Stub()
		{
			delete[] names;
			delete[] paths;
		}



		LDS::Map<std::string, Picture*> m_pictures;

		void add_picture(const std::string& _name, Picture* _picture)
		{
			L_ASSERT(m_pictures.find(_name).is_ok() == false);

			m_pictures.insert(_name, _picture);
		}

		void remove_picture(const std::string& _name)
		{
			LDS::Map<std::string, Picture*>::Iterator it = m_pictures.find(_name);
			L_ASSERT(it.is_ok() == true);

			delete *it;
			m_pictures.erase(m_pictures.find(_name));
		}

		void clear_pictures()
		{
			for(LDS::Map<std::string, Picture*>::Iterator it = m_pictures.iterator(); !it.end_reached(); ++it)
				delete *it;
			m_pictures.clear();
		}

		const Picture* get_picture(const std::string& _name)
		{
			LDS::Map<std::string, Picture*>::Iterator it = m_pictures.find(_name);
			if(!it.is_ok())
				return nullptr;
			return *it;
		}
	}

}

