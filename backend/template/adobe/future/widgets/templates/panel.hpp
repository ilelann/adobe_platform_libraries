#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct panel : widget_base<Backend> {
    typedef any_regular_t model_type;

    panel(const any_regular_t& show_value, theme_t theme)
        : show_value_m{show_value}, theme_m{theme}
    {}

    void display(const any_regular_t& value)
    {
        this->set_visible(value == this->show_value_m);
    }

#ifndef ADOBE_NO_DOCUMENTATION
    any_regular_t    show_value_m;
    theme_t          theme_m;
#endif
};

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, panel<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_panel(parent);

            return Backend::post(display, parent, element);
        }

}}
