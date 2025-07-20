#include <LEti_Registration.h>

#include <vec2.hpp>
#include <vec3.hpp>

#include <Data_Structures/Vector.h>
#include <Data_Structures/List.h>

#include <Object.h>
#include <Resources_Manager.h>
#include <Weak_Module_Holder.h>

using namespace LEti;


void LEti::register_types(LV::Object_Constructor& _object_constructor)
{
    LV::Type_Manager::register_type("glm::vec3", {
        [](const std::string& _val)
        {
            if(_val == ".")
                return false;

            unsigned int dots_count = 0;
            unsigned int i=0;
            if(_val[0] == '+' || _val[0] == '-')
                ++i;
            for(; i<_val.size(); ++i)
            {
                if(_val[i] == '.')
                {
                    ++dots_count;
                    continue;
                }
                if(_val[i] < '0' || _val[i] > '9')
                    return false;
            }

            if(dots_count > 1)
                return false;

            return true;
        },
            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
        {
            L_ASSERT(_values_as_string.size() == 3);

            glm::vec3& vec = *((glm::vec3*)_variable_vptr);
            for(unsigned int i=0; i<3; ++i)
                vec[i] = std::stof(_values_as_string[i]);
        },
            [](void* _variable_vptr)
        {
            const glm::vec3& variable = *((glm::vec3*)_variable_vptr);
            LDS::Vector<std::string> result;
            result.resize(3);
            result.push(std::to_string(variable[0]));
            result.push(std::to_string(variable[1]));
            result.push(std::to_string(variable[2]));
            return result;
        }
    });
    LV::Type_Manager::register_type("LDS::List<glm::vec3>", {
        [](const std::string& _val)
        {
            if(_val == ".")
                return false;

            unsigned int dots_count = 0;
            unsigned int i=0;
            if(_val[0] == '+' || _val[0] == '-')
                ++i;
            for(; i<_val.size(); ++i)
            {
                if(_val[i] == '.')
                {
                    ++dots_count;
                    continue;
                }
                if(_val[i] < '0' || _val[i] > '9')
                    return false;
            }

            if(dots_count > 1)
                return false;

            return true;
        },
            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
        {
            LDS::List<glm::vec3>& list = *((LDS::List<glm::vec3>*)_variable_vptr);
            list.clear();

            L_ASSERT(_values_as_string.size() % 3 == 0);

            for(unsigned int i=0; i<_values_as_string.size(); i += 3)
            {
                glm::vec3 vector;
                vector[0] = std::stof(_values_as_string[i]);
                vector[1] = std::stof(_values_as_string[i + 1]);
                vector[2] = std::stof(_values_as_string[i + 2]);

                list.push_back(vector);
            }
        },
            [](void* _variable_vptr)
        {
            const LDS::List<glm::vec3>& variable = *((LDS::List<glm::vec3>*)_variable_vptr);
            LDS::Vector<std::string> result;
            result.resize(variable.size() * 3);
            for(LDS::List<glm::vec3>::Const_Iterator it = variable.begin(); !it.end_reached(); ++it)
            {
                const glm::vec3& vec = *it;
                result.push(std::to_string(vec[0]));
                result.push(std::to_string(vec[1]));
                result.push(std::to_string(vec[2]));
            }
            return result;
        }
    });

    LV::Type_Manager::register_type("glm::vec2", {
        [](const std::string& _val)
        {
            if(_val == ".")
                return false;

            unsigned int dots_count = 0;
            unsigned int i=0;
            if(_val[0] == '+' || _val[0] == '-')
                ++i;
            for(; i<_val.size(); ++i)
            {
                if(_val[i] == '.')
                {
                    ++dots_count;
                    continue;
                }
                if(_val[i] < '0' || _val[i] > '9')
                    return false;
            }

            if(dots_count > 1)
                return false;

            return true;
        },
            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
        {
            L_ASSERT(_values_as_string.size() == 2);

            glm::vec2& vec = *((glm::vec2*)_variable_vptr);
            for(unsigned int i=0; i<2; ++i)
                vec[i] = std::stof(_values_as_string[i]);
        },
            [](void* _variable_vptr)
        {
            const glm::vec2& variable = *((glm::vec2*)_variable_vptr);
            LDS::Vector<std::string> result;
            result.resize(2);
            result.push(std::to_string(variable[0]));
            result.push(std::to_string(variable[1]));
            return result;
        }
    });
    LV::Type_Manager::register_type("LDS::List<glm::vec2>", {
        [](const std::string& _val)
        {
            if(_val == ".")
                return false;

            unsigned int dots_count = 0;
            unsigned int i=0;
            if(_val[0] == '+' || _val[0] == '-')
                ++i;
            for(; i<_val.size(); ++i)
            {
                if(_val[i] == '.')
                {
                    ++dots_count;
                    continue;
                }
                if(_val[i] < '0' || _val[i] > '9')
                    return false;
            }

            if(dots_count > 1)
                return false;

            return true;
        },
            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
        {
            LDS::List<glm::vec2>& list = *((LDS::List<glm::vec2>*)_variable_vptr);
            list.clear();

            L_ASSERT(_values_as_string.size() % 2 == 0);

            for(unsigned int i=0; i<_values_as_string.size(); i += 2)
            {
                glm::vec2 vector;
                vector[0] = std::stof(_values_as_string[i]);
                vector[1] = std::stof(_values_as_string[i + 1]);

                list.push_back(vector);
            }
        },
            [](void* _variable_vptr)
        {
            const LDS::List<glm::vec2>& variable = *((LDS::List<glm::vec2>*)_variable_vptr);
            LDS::Vector<std::string> result;
            result.resize(variable.size() * 2);
            for(LDS::List<glm::vec2>::Const_Iterator it = variable.begin(); !it.end_reached(); ++it)
            {
                const glm::vec2& vec = *it;
                result.push(std::to_string(vec[0]));
                result.push(std::to_string(vec[1]));
            }
            return result;
        }
    });
    LV::Type_Manager::register_type("LDS::Vector<glm::vec2>", {
        [](const std::string& _val)
        {
            if(_val == ".")
                return false;

            unsigned int dots_count = 0;
            unsigned int i=0;
            if(_val[0] == '+' || _val[0] == '-')
                ++i;
            for(; i<_val.size(); ++i)
            {
                if(_val[i] == '.')
                {
                    ++dots_count;
                    continue;
                }
                if(_val[i] < '0' || _val[i] > '9')
                    return false;
            }

            if(dots_count > 1)
                return false;

            return true;
        },
            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
        {
            LDS::Vector<glm::vec2>& vector = *((LDS::Vector<glm::vec2>*)_variable_vptr);
            vector.clear();

            L_ASSERT(_values_as_string.size() % 2 == 0);

            vector.resize(_values_as_string.size() / 2);

            for(unsigned int i=0; i<_values_as_string.size(); i += 2)
            {
                glm::vec2 vec;
                vec[0] = std::stof(_values_as_string[i]);
                vec[1] = std::stof(_values_as_string[i + 1]);

                vector.push(vec);
            }
        },
            [](void* _variable_vptr)
        {
            const LDS::Vector<glm::vec2>& variable = *((LDS::Vector<glm::vec2>*)_variable_vptr);
            LDS::Vector<std::string> result;
            result.resize(variable.size() * 2);
            for(unsigned int i=0; i<variable.size(); ++i)
            {
                result.push(std::to_string(variable[i][0]));
                result.push(std::to_string(variable[i][1]));
            }
            return result;
        }
    });

    _object_constructor.register_type<LEti::Object_Stub>();

    _object_constructor.register_type<LEti::Resources_Manager_Stub>();

    _object_constructor.register_type<LEti::Weak_Module_Holder_Stub>();
}
