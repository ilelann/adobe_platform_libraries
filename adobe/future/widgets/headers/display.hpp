/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#ifndef ADOBE_DISPLAY_HPP
#define ADOBE_DISPLAY_HPP

/****************************************************************************************************/

#include <adobe/config.hpp>

#include <adobe/future/platform_primitives.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

struct display_t
{
    display_t()
        : root_m{platform_display_type{}}
    {
    }

    void set_root(platform_display_type element)
    {
// root display is not used for GTK backend
// but some common code calls root method
// so the best I found was to to let it null for now
#ifndef ADOBE_PLATFORM_GTK
        root_m = element;
#endif
    }

    platform_display_type root()
    {
        return root_m;
    }

#ifndef ADOBE_PLATFORM_GTK
    template <typename DisplayElement>
    platform_display_type insert(platform_display_type& parent, const DisplayElement& element);
#endif

private:
    platform_display_type root_m;
};

/****************************************************************************************************/

display_t& get_main_display();

/****************************************************************************************************/

template <typename DisplayElement>
platform_display_type insert(display_t& display, platform_display_type& parent, DisplayElement& element);

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif // ADOBE_DISPLAY_HPP

/****************************************************************************************************/
