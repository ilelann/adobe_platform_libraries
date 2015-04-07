#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/widget_attributes.hpp>

#include <string>

namespace adobe { namespace implementation {

template <typename Backend>
struct group : widget_base<Backend> {
    group(const std::string& name, const std::string& alt_text, theme_t theme)
        : name_m{name}, alt_text_m{alt_text}, theme_m{theme}
    {}

#ifndef ADOBE_NO_DOCUMENTATION
std::string name_m;
std::string alt_text_m;
theme_t     theme_m;
#endif
};


        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, group<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_group(parent, element.name_m);

            return Backend::post(display, parent, element);
        }
}}
