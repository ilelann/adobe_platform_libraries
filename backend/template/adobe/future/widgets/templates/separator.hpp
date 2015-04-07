#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct separator : widget_base<Backend> {
    separator(bool is_vertical, theme_t theme)
    :is_vertical_m(is_vertical), theme_m(theme)
    {
    }

#ifndef ADOBE_NO_DOCUMENTATION
    bool            is_vertical_m;
    theme_t  theme_m;
#endif
};

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, separator<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_separator(parent, element.is_vertical_m);

            return Backend::post(display, parent, element);
        }
}}
