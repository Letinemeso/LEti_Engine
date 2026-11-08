#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Transformation_Data.h>


#define SUBSCRIBABLE \
    private: \
        using Subscription_Action = LST::Function<void(Module*, Module*)>; \
        using Subscriptions_Map = LDS::Map<std::string, Subscription_Action>; \
        inline static Subscriptions_Map s_subscriptions; \
    \
    public: \
        inline static void subscribe_to_notification(const std::string& _notification_id, const Subscription_Action& _action) { s_subscriptions.insert(_notification_id, _action); } \
    \
        void notify(const std::string& _notification_id, LEti::Module* _notifier) override \
        { \
            Parent_Type::notify(_notification_id, _notifier); \
            Subscriptions_Map::Iterator maybe_subscription_it = s_subscriptions.find(_notification_id); \
            if(maybe_subscription_it.is_ok()) \
                (*maybe_subscription_it)(this, _notifier); \
        }

#define CREATE_NOTIFICATION(_NOTIFICATION) \
    public: \
        inline static const std::string notification_id__##_NOTIFICATION = get_estimated_type() + "::" #_NOTIFICATION; \
        inline void notify__##_NOTIFICATION () { M_notify_others( notification_id__##_NOTIFICATION, this ); }

#define SUBSCRIBE(_SUBSCRIBER_TYPE, _SUBSCRIBER_NAME, _NOTIFIER_TYPE, _NOTIFIER_NAME, _NOTIFICATION) \
    using Subscriber_Type = _SUBSCRIBER_TYPE; \
    using Notifier_Type = _NOTIFIER_TYPE; \
    \
    class __Subscription_Helper__ ## _SUBSCRIBER_TYPE ## __ ## _NOTIFIER_TYPE ## __ ##_NOTIFICATION \
    { \
    public: \
        __Subscription_Helper__ ## _SUBSCRIBER_TYPE ## __ ## _NOTIFIER_TYPE ## __ ##_NOTIFICATION() \
        { \
            Subscriber_Type::subscribe_to_notification( Notifier_Type::notification_id__on_damaged, [](LEti::Module* _subscriber, LEti::Module* _notifier) \
            { \
                L_ASSERT(LV::can_be_cast_to<Subscriber_Type>(_subscriber)); \
                L_ASSERT(LV::can_be_cast_to<Notifier_Type>(_notifier)); \
                 \
                Subscriber_Type* subscriber = LST::raw_cast<Subscriber_Type>(_subscriber); \
                Notifier_Type* notifier = LST::raw_cast<Notifier_Type>(_notifier); \
                 \
                M_subscription_action(subscriber, notifier); \
            }); \
        } \
        \
    private: \
        static void M_subscription_action(Subscriber_Type* _subscriber, Notifier_Type* _notifier); \
    } __Subscription_Helper__ ## _SUBSCRIBER_TYPE ## __ ## _NOTIFIER_TYPE ## __ ##_NOTIFICATION ## __instance; \
    \
    void __Subscription_Helper__ ## _SUBSCRIBER_TYPE ## __ ## _NOTIFIER_TYPE ## __ ##_NOTIFICATION::M_subscription_action(Subscriber_Type* _SUBSCRIBER_NAME, Notifier_Type* _NOTIFIER_NAME) \


namespace LEti
{

    class Object;


    class Module : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LEti::Module, LV::Variable_Base)

    private:
        Transformation_Data* m_transformation_data = nullptr;
        const Transformation_Data* m_transformation_data_prev_state = nullptr;

        Object* m_parent_object = nullptr;

    public:
        Module();
        ~Module();

    public:
        virtual void set_transformation_data(Transformation_Data* _data) { m_transformation_data = _data; }
        virtual void set_transformation_data_prev_state(const Transformation_Data* _data) { m_transformation_data_prev_state = _data; }
        inline void set_parent_object(Object* _ptr) { m_parent_object = _ptr; M_on_parent_object_set(); }

        inline Transformation_Data* transformation_data() const { return m_transformation_data; }
        inline const Transformation_Data* transformation_data_prev_state() const { return m_transformation_data_prev_state; }
        inline Object* parent_object() const { return m_parent_object; }

        void assign_same_parent(Module& _to) const;

    protected:
        void M_notify_others(const std::string& _notification_id, Module* _notifier);

    public:
        virtual void notify(const std::string& _notification_id, Module* _notifier) { }

    protected:
        virtual void M_on_parent_object_set() { }

    public:
        virtual void update_prev_state() { }
        virtual void update(float _dt) { }

    };


    class Module_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LEti::Module_Stub, LV::Builder_Stub)

    public:
        const Object* future_parent = nullptr;

    public:
        INIT_NULL_BUILDER_STUB(Module)

    };

}
