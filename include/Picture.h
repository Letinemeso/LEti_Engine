#ifndef __PICTURE
#define __PICTURE

#include "Debug/Debug.h"


namespace LEti {

    class Picture
    {
    private:
        int m_width = 0, m_height = 0;
        unsigned char* m_picture_data = nullptr;

    private:
        Picture() {}
		friend Picture* load_picture(const char* _path);

    public:
        Picture(Picture&& _from);
        ~Picture();

    public:
        int width() const;
        int height() const;
        const unsigned char* data() const;

    };

	Picture* load_picture(const char* _path);

} /*LEti*/

#endif // __PICTURE
