#include "../include/Resource_Loader.h"

using namespace LEti;


std::map<std::string, Resource_Loader::Type_Handler> Resource_Loader::m_type_handlers;

std::map<std::string, Resource_Loader::object_data> Resource_Loader::m_objects;
std::map<std::string, Picture> Resource_Loader::m_pictures;


void Resource_Loader::load_variables(const std::string& _source, const char* _name)
{
	bool is_parsing_var_name = false, is_parsing_var_type = false, is_parsing_var_count = false, is_parsing_var = false;
	unsigned int var_values_left = 0;
	auto switch_state = [](bool& _var) { _var = _var ? false : true; };

	std::string name;
	std::string str_value;

	parsed_value result;

	unsigned int i = 0;
	while (i < _source.size())
	{
		if (_source[i] == '|')
		{
			ASSERT(is_parsing_var_type || is_parsing_var_count || is_parsing_var);
			switch_state(is_parsing_var_name);
			if (is_parsing_var_name == false)
				ASSERT(name.size() == 0);
		}
		else if (_source[i] == '\"')
		{
			ASSERT(is_parsing_var_type || is_parsing_var_count || is_parsing_var_name);
			switch_state(is_parsing_var);
			if (is_parsing_var == false)
			{
				ASSERT(str_value.size() == 0);
				ASSERT(var_values_left == 0);

                const Type_Handler& handler = m_type_handlers.at(result.type);
                unsigned int index = result.values_count - var_values_left;
                handler.parse_variable(result.value, index, str_value);

				str_value.clear();
				--var_values_left;
			}
		}
		else if (_source[i] == '<')
		{
			ASSERT(is_parsing_var || is_parsing_var_count || is_parsing_var_name);
			switch_state(is_parsing_var_type);
		}
		else if (_source[i] == '[')
		{
			ASSERT(is_parsing_var_type || is_parsing_var || is_parsing_var_name);
			switch_state(is_parsing_var_count);
		}
		else if (is_parsing_var_name)
		{
			name += _source[i];
		}
		else if (is_parsing_var_count)
		{
			if (_source[i] == ']')
			{
				ASSERT(str_value.size() == 0);
				switch_state(is_parsing_var_count);
				result.values_count = std::stoi(str_value);
				ASSERT(result.values_count == 0);
				var_values_left = result.values_count;
				str_value.clear();

                const Type_Handler& handler = m_type_handlers.at(result.type);
                handler.allocate_memory(result.value, result.values_count);
			}
			else if (Math::is_digit(_source[i]))
				str_value += _source[i];
			else ASSERT(false);
		}
		else if (is_parsing_var_type)
		{
			if (_source[i] == '>')
			{
				switch_state(is_parsing_var_type);
			}
			else
				result.type += _source[i];
		}
		else if (is_parsing_var)
        {
			str_value += _source[i];
		}

		ASSERT((_source[i] == '\n' || i + 1 == _source.size()) && (is_parsing_var_name || is_parsing_var_type || is_parsing_var || var_values_left > 0));

		if (_source[i] == '\n' || i + 1 == _source.size())
		{
            if(result.type == "texture")
                m_pictures.emplace(name, LEti::load_picture(((std::string*)(result.value))->c_str()) );

            m_objects.at(_name).variables.emplace(std::move(name), std::move(result));
		}

		++i;
	}
}



void Resource_Loader::init()
{
    m_type_handlers.emplace ( "float",
                Type_Handler(
                    [](void*& _result, unsigned int _size)
                    {
                        _result = new float[_size];
                    },
                    [](void*& _result)
                    {
                        float* ptr_to_delete = (float*)_result;
                        delete[] ptr_to_delete;
                        _result = nullptr;
                    },
                    [](void*& _arr, unsigned int _index, const std::string& _str_var)
                    {
                        for(unsigned int i=0; i<_str_var.size(); ++i)
							ASSERT(!Math::is_digit(_str_var[i]) && _str_var[i] != '-' && _str_var[i] != '.');
                        float* f_arr = (float*)_arr;
                        f_arr[_index] = stof(_str_var);
                    }
                )
            );
    m_type_handlers.emplace ( "int",
                Type_Handler(
                    [](void*& _result, unsigned int _size)
                    {
                        _result = new int[_size];
                    },
                    [](void*& _result)
                    {
                        int* ptr_to_delete = (int*)_result;
                        delete[] ptr_to_delete;
                        _result = nullptr;
                    },
                    [](void*& _arr, unsigned int _index, const std::string& _str_var)
                    {
                        for(unsigned int i=0; i<_str_var.size(); ++i)
							ASSERT(!Math::is_digit(_str_var[i]) && _str_var[i] != '-');
                        int* f_arr = (int*)_arr;
                        f_arr[_index] = stoi(_str_var);
                    }
                )
            );
    m_type_handlers.emplace ( "string",
                Type_Handler(
                    [](void*& _result, unsigned int _size)
                    {
                        _result = new std::string[_size];
                    },
                    [](void*& _result)
                    {
                        std::string* ptr_to_delete = (std::string*)_result;
                        delete[] ptr_to_delete;
                        _result = nullptr;
                    },
                    [](void*& _arr, unsigned int _index, const std::string& _str_var)
                    {
                        std::string* f_arr = (std::string*)_arr;
                        f_arr[_index] = _str_var;
                    }
                )
            );
    m_type_handlers.emplace ( "texture",
                Type_Handler(
                    [](void*& _result, unsigned int _size)
                    {
                        _result = new std::string[_size];
                    },
                    [](void*& _result)
                    {
                        std::string* ptr_to_delete = (std::string*)_result;
                        delete[] ptr_to_delete;
                        _result = nullptr;
                    },
                    [](void*& _arr, unsigned int _index, const std::string& _str_var)
                    {
                        std::string* f_arr = (std::string*)_arr;
                        f_arr[_index] = _str_var;
                    }
                )
            );
}

void Resource_Loader::register_type(const std::string &_typename, alloc_fptr _allocate_func, free_fptr _free_func, parse_fptr _parse_func)
{
    std::map<std::string, Type_Handler>::iterator check = m_type_handlers.find(_typename);
    ASSERT(check != m_type_handlers.end());

    m_type_handlers.emplace(_typename, Type_Handler(_allocate_func, _free_func, _parse_func));
}

void Resource_Loader::unregister_type(const std::string &_typename)
{
    std::map<std::string, Type_Handler>::iterator it = m_type_handlers.find(_typename);
    ASSERT(it == m_type_handlers.end());

    m_type_handlers.erase(it);
}



void Resource_Loader::load_object(const char* _name, const char* _path)
{
	std::ifstream file(_path, std::ios::in);
	ASSERT(!file.is_open());

	file.seekg(0, std::ios::end);
	unsigned int size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string source;
	source.resize(size);

	for (unsigned int i = 0; i < size; ++i)
		source[i] = file.get();

	file.close();

    m_objects.emplace(std::string(_name), object_data());

	load_variables(source, _name);
}

void Resource_Loader::delete_object(const char* _name)
{
    std::map<std::string, object_data>::iterator it = m_objects.find(_name);
    ASSERT(it == m_objects.end());
    std::map<std::string, parsed_value>::iterator pv_it = it->second.variables.begin();
    while (pv_it != it->second.variables.end())
    {
        const Type_Handler& handler = m_type_handlers.at(pv_it->second.type);
        handler.free_memory(pv_it->second.value);
        ++pv_it;
    }
    m_objects.erase(_name);
}

void Resource_Loader::delete_picture(const char *_name)
{
    std::map<std::string, Picture>::iterator it = m_pictures.find(_name);
    ASSERT(it == m_pictures.end());
    m_pictures.erase(it);
}

void Resource_Loader::clear_pictures()
{
    m_pictures.clear();
}



const Picture& Resource_Loader::get_picture(const char *_name)
{
    std::map<std::string, Picture>::iterator it = m_pictures.find(_name);
    ASSERT(it == m_pictures.end());

    return it->second;
}
