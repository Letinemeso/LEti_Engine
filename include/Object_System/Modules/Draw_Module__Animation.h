#ifndef DRAW_MODULE__ANIMATION_H
#define DRAW_MODULE__ANIMATION_H

#include <Stuff/Timer.h>

#include <Object_System/Modules/Default_Draw_Module_2D.h>


namespace LEti
{

    class Draw_Module__Animation : public Default_Draw_Module_2D
    {
    public:
        DECLARE_VARIABLE;

    private:
        unsigned int m_frames_count = 0;
        unsigned int m_fps = 0;
        float m_time_before_next_frame = 0.0f;
        float m_frame_offset_ratio = 0.0f;

        unsigned int m_current_frame = 0;

    private:
        bool m_is_paused = true;
        LST::Timer m_frame_update_timer;
        unsigned int m_times_to_repeat = 0;
        unsigned int m_repetitions = 0;

    public:
        Draw_Module__Animation();

    public:
        void update(const glm::mat4x4& _matrix) override;

    public:
        void set_animation_data(unsigned int _frames_count);
        void set_fps(unsigned int _fps);
        void set_frame(unsigned int _frame);

        void start(unsigned int _cycles = 0);
        void pause();
        void unpause();

    public:
        inline unsigned int frames_count() const { return m_frames_count; }
        inline unsigned int fps() const { return m_fps; }
        inline unsigned int current_frame() const { return m_current_frame; }
        inline unsigned int repetitions() const { return m_repetitions; }
        inline bool paused() const { return m_is_paused; }

    };

    class Draw_Module__Animation__Stub : public Default_Draw_Module_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        unsigned int frames_per_second = 0;
        unsigned int frames_count = 0;
        unsigned int times_to_repeat = 0;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}


#endif // DRAW_MODULE__ANIMATION_H
