/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/widgets/headers/display.hpp>

#include <gtk/gtk.h>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

display_t& get_main_display()
{
    static display_t display_s;

    return display_s;
}

/****************************************************************************************************/

template <typename DisplayElement>
platform_display_type insert(display_t& display, platform_display_type& position, DisplayElement& element)
{ return display.insert(position, element); }

/****************************************************************************************************/

 template <>
 platform_display_type display_t::insert<platform_display_type>(platform_display_type& parent, const platform_display_type& element)
 {
     static const platform_display_type null_parent_s = platform_display_type();

     if (parent != null_parent_s && parent != get_main_display().root())
     {
         auto container = GTK_CONTAINER(parent);
         while(GTK_IS_BIN(container))
         {
             container = GTK_CONTAINER(gtk_bin_get_child(GTK_BIN(container)));
         }
         gtk_container_add(container, element);
     }
     return element;
 }

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
