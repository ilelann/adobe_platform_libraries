#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <functional>

namespace adobe {
namespace implementation {

template <typename Backend>
struct checkbox : widget_base<Backend>
{
    using model_type  = any_regular_t;
    using setter_type = std::function<void(const model_type&)>;

    checkbox(const std::string&   name,
             const any_regular_t& true_value,
             const any_regular_t& false_value,
             theme_t,
             const std::string& alt_text)
        : name_m{name}
        , true_value_m{true_value}
        , false_value_m{false_value}
        , alt_text_m{alt_text}
    {
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_checkbox(this->control_m, value == true_value_m, value == false_value_m);
    }

    void monitor(const setter_type& proc)
    {
        assert(!this->is_null());
        hit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string   name_m;
    setter_type   hit_proc_m;
    any_regular_t true_value_m;
    any_regular_t false_value_m;
    std::string   alt_text_m;
#endif
};

/****************************************************************************************************/

template <typename Backend>
void on_checkbox_toggled(const checkbox<Backend>& element, bool active)
{
    if (!element.hit_proc_m)
        return;

    element.hit_proc_m(active ? element.true_value_m : element.false_value_m);
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, checkbox<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_checkbox(parent, element.name_m, element.alt_text_m, element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
