#ifndef __MESSAGE_TRANSLATOR
#define __MESSAGE_TRANSLATOR

#include <string>
#include <map>
#include <vector>

#include "../Debug.h"


namespace LEti {

    #define DEFINE_TYPE(type_name) static std::string type() { return type_name; }

    class Message_Translator
    {
    private:
        struct func_wrapper_interface
        {
            virtual void call(void* _arg) const = 0;

            func_wrapper_interface() { }
            virtual ~func_wrapper_interface() { }
        };

        template<typename arg_type, typename func_type>
        struct static_func_wrapper : public func_wrapper_interface
        {
            func_type action;

            void call(void* _arg) const override { action(*(arg_type*)_arg); }

            static_func_wrapper(func_type _action) : func_wrapper_interface(), action(_action) { /*action = _action;*/ }
            ~static_func_wrapper() {}
        };

        template<typename arg_type, typename obj_type, typename func_type>
        struct member_func_wrapper : public func_wrapper_interface
        {
            obj_type* object = nullptr;
            func_type action = nullptr;

            void call(void* _arg) const override { (object->*action)(*(arg_type*)_arg); }

            member_func_wrapper(obj_type* _object, func_type _action) : func_wrapper_interface(/*_action*/), object(_object), action(_action) { }
            ~member_func_wrapper() {}
        };

    private:
        static std::map<std::string, std::vector<func_wrapper_interface*>> m_subscribers;

    public:
        class subscriber_handle
        {
            friend class Message_Translator;
            std::map<std::string, std::vector<func_wrapper_interface*>>::iterator belongs_to_msg;
            const func_wrapper_interface* ptr = nullptr;
        };

    public:
        Message_Translator() = delete;
        Message_Translator(const Message_Translator& _other) = delete;
        Message_Translator(Message_Translator&& _other) = delete;

        template <typename msg_type>
        static void register_message_type();

        template <typename msg_type>
        static void unregister_message_type();

        template<typename msg_type, typename func_type>
        static subscriber_handle subscribe(func_type _func);

        template<typename msg_type, typename obj_type, typename func_type>
        static subscriber_handle subscribe(obj_type* _obj_ptr, func_type _func);

        template<typename msg_type>
        static void publish(const msg_type& _message);

        static void clear_subscribers();

        static void unsubscribe(subscriber_handle& _handle);

    };

    std::map<std::string, std::vector<Message_Translator::func_wrapper_interface*>> Message_Translator::m_subscribers;

    template <typename msg_type>
    void Message_Translator::register_message_type()
    {
		L_ASSERT(!(m_subscribers.find(msg_type::type()) != m_subscribers.end()));

        m_subscribers.emplace(msg_type::type(), std::vector<func_wrapper_interface*>());
    }

    template <typename msg_type>
    void Message_Translator::unregister_message_type()
    {
        std::map<std::string, std::vector<func_wrapper_interface*>>::iterator check = m_subscribers.find(msg_type::type());
		L_ASSERT(!(check == m_subscribers.end()));

        for(unsigned int i=0; i<check->second.size(); ++i)
            delete check->second[i];
        m_subscribers.erase(check);
    }

    template<typename msg_type, typename func_type>
    Message_Translator::subscriber_handle Message_Translator::subscribe(func_type _func)
    {
        std::map<std::string, std::vector<func_wrapper_interface*>>::iterator check = m_subscribers.find(msg_type::type());
		L_ASSERT(!(check == m_subscribers.end()));

        check->second.push_back(new static_func_wrapper<msg_type, func_type>(_func));

        subscriber_handle result;
        result.belongs_to_msg = check;
        std::vector<func_wrapper_interface*>::iterator it = check->second.end();
        --it;
        result.ptr = *it;
        return result;
    }

    template<typename msg_type, typename obj_type, typename func_type>
    Message_Translator::subscriber_handle Message_Translator::subscribe(obj_type* _obj_ptr, func_type _func)
    {
        std::map<std::string, std::vector<func_wrapper_interface*>>::iterator check = m_subscribers.find(msg_type::type());
		L_ASSERT(!(check == m_subscribers.end()));

        check->second.push_back(new member_func_wrapper<msg_type, obj_type, func_type>(_obj_ptr, _func));

        subscriber_handle result;
        result.belongs_to_msg = check;
        std::vector<func_wrapper_interface*>::iterator it = check->second.end();
        --it;
        result.ptr = *it;
        return result;
    }

    template<typename msg_type>
    void Message_Translator::publish(const msg_type& _message)
    {
        std::map<std::string, std::vector<func_wrapper_interface*>>::iterator subscribers = m_subscribers.find(_message.type());
		L_ASSERT(!(subscribers == m_subscribers.end()));

        for(unsigned int i=0; i<subscribers->second.size(); ++i)
            subscribers->second[i]->call((void*)&_message);
    }

    void Message_Translator::clear_subscribers()
    {
        std::map<std::string, std::vector<func_wrapper_interface*>>::iterator it = m_subscribers.begin();
        while(it != m_subscribers.end())
        {
            for(unsigned int i=0; i<it->second.size(); ++i)
            {
                delete it->second[i];
                it->second[i] = nullptr;
            }
            ++it;
        }
        m_subscribers.clear();
    }

    void Message_Translator::unsubscribe(subscriber_handle& _handle)
    {
        std::vector<func_wrapper_interface*>::iterator it = _handle.belongs_to_msg->second.begin();
        while(it != _handle.belongs_to_msg->second.end())
        {
            if(*it == _handle.ptr) break;
            ++it;
        }
        delete *it;
        _handle.belongs_to_msg->second.erase(it);

        _handle.ptr = nullptr;
        _handle.belongs_to_msg = m_subscribers.end();
    }

}


#endif // __MESSAGE_TRANSLATOR
