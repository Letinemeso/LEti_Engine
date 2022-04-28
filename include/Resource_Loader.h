#ifndef __RESOURCE_LOADER
#define __RESOURCE_LOADER

#include <fstream>
#include <string>
#include <map>
#include <memory>

#include "../Debug.h"
#include "../include/Utility.h"

#include "../include/Picture.h"


namespace LEti {

	class Resource_Loader
	{
	private:
		struct parsed_value
		{
			unsigned int values_count = 0;
			std::string type;
			void* value = nullptr;
		};

		struct object_data
		{
			std::map<std::string, parsed_value> variables;
        };

	private:
        static std::map<std::string, object_data> m_objects;
        static std::map<std::string, Picture> m_pictures;

	public:
		Resource_Loader() = delete;

	private:
        static void load_variables(const std::string& _source, const char* _name);

	public:
		static void load_object(const char* _name, const char* _path);
		static void delete_object(const char* _name);

		template<typename T>
		static std::pair<const T* const, unsigned int> get_data(const char* _object_name, const char* _variable_name);
        static const Picture& get_picture(const char* _name);

	};

	template<typename T>
	std::pair<const T* const, unsigned int> Resource_Loader::get_data(const char* _object_name, const char* _variable_name)
	{
		std::pair<const T*, unsigned int> result;

        std::map<std::string, object_data>::iterator objects_it = m_objects.find(_object_name);
        ASSERT(objects_it == m_objects.end());
        std::map<std::string, parsed_value>::iterator vars_it = objects_it->second.variables.find(_variable_name);
		ASSERT(vars_it == objects_it->second.variables.end());

		result.first = (T*)(vars_it->second.value);
		result.second = vars_it->second.values_count;
		return result;
	}

}	/*LEti*/

#endif
