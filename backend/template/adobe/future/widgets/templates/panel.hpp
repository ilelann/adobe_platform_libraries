#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct panel : widget_base<Backend>
{
    using model_type = any_regular_t;

    panel(const any_regular_t& show_value, theme_t theme)
        : show_value_m{show_value}
        , theme_m{theme}
    {
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        this->set_visible(value == this->show_value_m);
    }

#ifndef ADOBE_NO_DOCUMENTATION
    any_regular_t show_value_m;
    theme_t       theme_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, panel<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_panel(parent);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
