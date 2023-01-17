#ifndef __A_STAR
#define __A_STAR

#include <cmath>
#include <vector>

#include "Debug/Debug.h"

#include "Tree.h"


namespace LEti {

    class EIPF  //Extremely Ineficient Path Finder
    {
    private:
        enum class Direction { up, down, left, right };
        void rotate_cw(Direction& _d) const;

    private:
        struct intp
        {
            int x = 0, y = 0;

            intp(int _x, int _y) : x(_x), y(_y) {}

            bool operator==(const intp& _other) const { return x == _other.x && y == _other.y; }
            bool operator!=(const intp& _other) const { return !(*this == _other); }

            void move(Direction _d);
            intp after_movement(Direction _d) const;
        };

    private:
        bool** m_occupied = nullptr;
        unsigned int m_size_x = 0, m_size_y = 0;

    public:
        std::pair<int, int> get_field_size() const;

    protected:
        unsigned int get_distance(const intp& _first, const intp& _second) const;
        bool at(const intp& _ind) const;
        bool out_of_bounds(const intp& _ind) const;

    private:
        bool is_step_unique(const intp& _step) const;

        bool can_move_here(const EIPF::intp &_where) const;

		std::pair<LEti::Tree<EIPF::intp, 4>::Iterator, EIPF::intp> find_closest_pos(const intp& _dest);

    private:
        LEti::Tree<intp, 4> m_tree;
        LEti::Tree<intp, 4>::Const_Iterator m_path_end;
        bool m_path_found = false;
        unsigned int m_steps_count = 0;
        intp m_current_pos = {-1, -1}, m_destination = {-1, -1};

    public:
        EIPF();
        ~EIPF();

    public:
        void set_field_size(unsigned int _x, unsigned int _y);
        void set_destination(unsigned int _x, unsigned int _y);
        void set_start_pos(unsigned int _x, unsigned int _y);
        bool& field_element(unsigned int _x, unsigned int _y);

    public:
        virtual void build_path();
        std::pair<int, int> get_next_step() const;
        std::vector<std::pair<int, int>> get_full_path() const;
        unsigned int get_steps_count() const;
        bool is_path_found() const;

    };
}

#endif // __A_STAR

