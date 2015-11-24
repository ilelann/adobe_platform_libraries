/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/widgets/headers/display.hpp>

#include <gtk/gtk.h>
#include <stdexcept>
#include <assert.h>

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

         if (GTK_IS_NOTEBOOK(container))
         {
             auto notebook = GTK_NOTEBOOK(container);
             auto npages = gtk_notebook_get_n_pages(notebook);

             if (0 == npages)
             {
                 throw std::runtime_error("no pages in notebook");
             }

             auto i = 0;
             auto tab = gtk_notebook_get_nth_page(notebook, i);
             while(!GTK_IS_LABEL(tab) && ((++i) < npages))
             {
                 tab = gtk_notebook_get_nth_page(notebook, i);
             }

             if (!GTK_IS_LABEL(tab))
             {
                 throw std::runtime_error("no place found to insert page");
             }

             std::string label = gtk_notebook_get_tab_label_text (notebook, tab);
             gtk_notebook_remove_page(notebook, i);
             gtk_notebook_insert_page(notebook, element, gtk_label_new(label.c_str()), i);
         }
         else
         {
             gtk_container_add(container, element);
         }
     }
     return element;
 }

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
