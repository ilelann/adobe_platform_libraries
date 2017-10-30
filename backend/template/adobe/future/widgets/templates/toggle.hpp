#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct toggle : widget_base<Backend>
{
    using model_type  = any_regular_t;
    using setter_type = std::function<void(const model_type&)>;

    toggle(const std::string& label, const std::string& alt_text, const any_regular_t value_on, theme_t)
        : name_m{label}
        , alt_text_m{alt_text}
        , value_on_m{value_on}
    {
    }

    void monitor(const setter_type& proc)
    {
        setter_proc_m = proc;
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::toggle_button(this->control_m, value == this->value_on_m);
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string   name_m;
    std::string   alt_text_m;
    any_regular_t value_on_m;
    setter_type   setter_proc_m;
#endif
};

template <typename Backend>
void on_toggle_clicked(toggle<Backend>& element, bool active)
{
    if (!element.setter_proc_m)
        return;

    // toggle it.
    element.setter_proc_m(active ? adobe::any_regular_t{} : element.value_on_m);
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, toggle<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_toggle(parent, element.alt_text_m);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
