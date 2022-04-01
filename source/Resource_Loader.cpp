#include "../include/Resource_Loader.h"

using namespace LEti;


std::map<std::string, Resource_Loader::object_data> Resource_Loader::objects;


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

				if (result.type == "float")
					((float*)(result.value))[result.values_count - var_values_left] = std::stof(str_value);
				else if (result.type == "string")
					((std::string*)(result.value))[result.values_count - var_values_left] = std::move(str_value);
				else if (result.type == "int")
					((int*)(result.value))[result.values_count - var_values_left] = std::stoi(str_value);
				else
					ASSERT(true);

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

				if (result.type == "float")
					result.value = (void*)(new float[result.values_count]);
				else if (result.type == "string")
					result.value = (void*)(new std::string[result.values_count]);
				else if (result.type == "int")
					result.value = (void*)(new int[result.values_count]);
				else
					ASSERT(true);
			}
			else if (Utility::is_digit(_source[i]))
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
			ASSERT(result.type == "float" && (!Utility::is_digit(_source[i]) && _source[i] != '-' && _source[i] != '.'));
			ASSERT(result.type == "int" && (!Utility::is_digit(_source[i]) && _source[i] != '-'));
			str_value += _source[i];
		}

		ASSERT((_source[i] == '\n' || i + 1 == _source.size()) && (is_parsing_var_name || is_parsing_var_type || is_parsing_var || var_values_left > 0));

		if (_source[i] == '\n' || i + 1 == _source.size())
		{
			objects.at(_name).variables.emplace(std::move(name), result);
			result.type.clear();
			result.value = nullptr;
			result.values_count = 1;
		}

		++i;
	}
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

	objects.emplace(std::string(_name), object_data());

	load_variables(source, _name);

	//	может пригодиться для сохранения
	/*std::map<std::string, object_data>::iterator it = objects.begin();
	while (it != objects.end())
	{
		std::cout << it->first << ":\n";
		std::map<std::string, parsed_value>::iterator it2 = it->second.variables.begin();
		while (it2 != it->second.variables.end())
		{
			std::cout << "\t" << it2->second.type << ' ' << it2->first << ": ";
			if (it2->second.type == "float")
			{
				for (unsigned int i = 0; i < it2->second.values_count; ++i)
					std::cout << ((float*)(it2->second.value))[i] << ' ';
			}
			else if (it2->second.type == "string")
			{
				for (unsigned int i = 0; i < it2->second.values_count; ++i)
					std::cout << ((std::string*)(it2->second.value))[i] << ' ';
			}
			else if (it2->second.type == "int")
			{
				for (unsigned int i = 0; i < it2->second.values_count; ++i)
					std::cout << ((int*)(it2->second.value))[i] << ' ';
			}
			std::cout << '\n';
			++it2;
		}
		std::cout << '\n';
		++it;
	}*/
}

void Resource_Loader::delete_object(const char* _name)
{
	std::map<std::string, object_data>::iterator it = objects.find(_name);
	ASSERT(it == objects.end());
	std::map<std::string, parsed_value>::iterator pv_it = it->second.variables.begin();
	while (pv_it != it->second.variables.end())
	{
		if (pv_it->second.type == "float")
		{
			float* ptr = (float*)(pv_it->second.value);
			delete[] ptr;
		}
		else if (pv_it->second.type == "string")
		{
			std::string* ptr = (std::string*)(pv_it->second.value);
			delete[] ptr;
		}
		else if (pv_it->second.type == "int")
		{
			int* ptr = (int*)(pv_it->second.value);
			delete[] ptr;
		}
		++pv_it;
	}
	objects.erase(_name);
}
