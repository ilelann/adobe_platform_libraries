/*
    Copyright 2015 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/widgets/headers/display.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

display_t& get_main_display()
{
    static display_t display_s;
    return display_s;
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
