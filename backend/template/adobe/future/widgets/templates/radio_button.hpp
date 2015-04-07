#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

/****************************************************************************************************/

template <typename Backend>
struct radio_button : widget_base<Backend> {
    typedef any_regular_t model_type;
    typedef std::function<void(const model_type&)> setter_type;

    radio_button(const std::string& name, const std::string& alt_text, const any_regular_t& set_value, theme_t theme)
        : name_m{name}, alt_text_m{alt_text}, set_value_m{set_value}, theme_m{theme}
    {}

    void display(const any_regular_t& value)
    {
        if (this->last_m != value) {
            this->last_m = value;
            Backend::activate_radio_button(this->control_m, (this->last_m == this->set_value_m));
        }
    }

    void monitor(const setter_type& proc)
    {
        hit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string name_m;
    std::string alt_text_m;
    any_regular_t set_value_m;
    theme_t theme_m;
    setter_type hit_proc_m;
    any_regular_t last_m;
#endif
};

/****************************************************************************************************/

template <typename Backend>
void on_radio_button_toggled(radio_button<Backend>& element)
{
    if (element.hit_proc_m)
        element.hit_proc_m(element.set_value_m);
}

/****************************************************************************************************/

template <typename Backend>
typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, radio_button<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_radio_button(parent, element.name_m, element.alt_text_m, element);

    return Backend::post(display, parent, element);
}

/****************************************************************************************************/


}}
