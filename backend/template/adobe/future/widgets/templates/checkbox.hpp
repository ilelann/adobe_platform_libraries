#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct checkbox : widget_base<Backend> {
    typedef any_regular_t model_type;
    typedef std::function<void(const model_type&)> setter_type;

    checkbox(
            const std::string& name,
            const any_regular_t& true_value,
            const any_regular_t& false_value,
            theme_t theme,
            const std::string& alt_text)
        : name_m{name},
          true_value_m{true_value},
          false_value_m{false_value},
          theme_m{theme},
          alt_text_m{alt_text}
    {}

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());

        if (current_value_m == value) return;

        current_value_m = value;

        Backend::set_checkbox(
                    this->control_m,
                    current_value_m == true_value_m,
                    current_value_m == false_value_m);
    }

    void monitor(const setter_type& proc)
    {
        assert(!this->is_null());
        hit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string name_m;
    setter_type hit_proc_m;
    any_regular_t true_value_m;
    any_regular_t false_value_m;
    theme_t theme_m;
    std::string alt_text_m;
    any_regular_t current_value_m;
#endif
};

        /****************************************************************************************************/

        template <typename Backend>
        void on_checkbox_toggled(checkbox<Backend>& element)
        {
            if (!element.hit_proc_m)
                return;

            adobe::any_regular_t new_value(element.true_value_m);

            if (element.current_value_m == element.true_value_m)
                new_value = element.false_value_m;

            element.hit_proc_m(new_value);
        }

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, checkbox<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_checkbox(parent, element.name_m, element.alt_text_m, element);

            return Backend::post(display, parent, element);
        }
}}
