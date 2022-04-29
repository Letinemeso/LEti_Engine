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
        typedef void(*alloc_fptr)(void*&, unsigned int);
        typedef void(*free_fptr)(void*&);
        typedef void(*parse_fptr)(void*&, unsigned int, const std::string&);

        struct Type_Handler
        {
            alloc_fptr allocate_memory = nullptr;
            free_fptr free_memory = nullptr;
            parse_fptr parse_variable = nullptr;

            Type_Handler(alloc_fptr _alloc, free_fptr _free, parse_fptr _parse) : allocate_memory(_alloc), free_memory(_free), parse_variable(_parse) {}
        };

        static std::map<std::string, Type_Handler> m_type_handlers;

	private:
		struct parsed_value
		{
			unsigned int values_count = 0;
			std::string type;
            void* value = nullptr;

            parsed_value() { };
            parsed_value(parsed_value&& _from) : values_count(_from.values_count), type(std::move(_from.type)) { value = _from.value; _from.value = nullptr;}
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
        static void init(); //inits standart parsing ( able to parse: int, float, string, texture )
        static void register_type(const std::string& _typename, alloc_fptr _allocate_func, free_fptr _free_func, parse_fptr _parse_func);
        static void unregister_type(const std::string& _typename);

	public:
		static void load_object(const char* _name, const char* _path);
		static void delete_object(const char* _name);
        static void delete_picture(const char* _name);
        static void clear_pictures();

		template<typename T>
		static std::pair<const T* const, unsigned int> get_data(const char* _object_name, const char* _variable_name);

        template<typename T>
        static void assign(std::pair<const T*, unsigned int>& _result, const char* _object_name, const char* _variable_name);

        template<typename T>
        static void assign(const T* _result, const char* _object_name, const char* _variable_name);

        static const Picture& get_picture(const char* _name);

	};

    template<typename T>
    void Resource_Loader::assign(std::pair<const T*, unsigned int>& _result, const char* _object_name, const char* _variable_name)
    {
        std::map<std::string, object_data>::iterator objects_it = m_objects.find(_object_name);
        ASSERT(objects_it == m_objects.end());
        std::map<std::string, parsed_value>::iterator vars_it = objects_it->second.variables.find(_variable_name);
        ASSERT(vars_it == objects_it->second.variables.end());

        _result.first = (T*)(vars_it->second.value);
        _result.second = vars_it->second.values_count;
    }

    template<typename T>
    void Resource_Loader::assign(const T* _result, const char* _object_name, const char* _variable_name)
    {
        std::map<std::string, object_data>::iterator objects_it = m_objects.find(_object_name);
        ASSERT(objects_it == m_objects.end());
        std::map<std::string, parsed_value>::iterator vars_it = objects_it->second.variables.find(_variable_name);
        ASSERT(vars_it == objects_it->second.variables.end());

        _result = (T*)(vars_it->second.value);
    }

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
