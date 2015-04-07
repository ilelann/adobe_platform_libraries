#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct toggle : widget_base<Backend> {
    typedef any_regular_t model_type;

    typedef std::function<void(const model_type&)> setter_type;

    typedef std::string image_type;

    toggle(const std::string & label, const std::string& alt_text, const any_regular_t value_on, theme_t theme)
            :
    name_m{label},
   alt_text_m{alt_text},
   value_on_m{value_on},
   theme_m{theme}
    {}


    void monitor(const setter_type& proc)
    {
        setter_proc_m = proc;
    }

    void display(const any_regular_t& to_value)
    {
        assert(!this->is_null());

        if (this->last_m != to_value) {
            this->last_m = to_value;
            Backend::toggle_button(this->control_m, (this->last_m == this->value_on_m));
        }
    }

#ifndef ADOBE_NO_DOCUMENTATION
std::string name_m;
std::string alt_text_m;
any_regular_t value_on_m;
setter_type setter_proc_m;
any_regular_t last_m;
theme_t theme_m;
#endif
};

        template <typename Backend>
        void on_toggle_clicked(toggle<Backend>& element)
        {
            if (!element.setter_proc_m)
                return;

            // toggle it.
            adobe::any_regular_t new_value = element.last_m == element.value_on_m
                                             ? adobe::any_regular_t(adobe::empty_t())
                                             : element.value_on_m;

            element.setter_proc_m(new_value);
        }

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, toggle<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_toggle(parent, element.alt_text_m);

            return Backend::post(display, parent, element);
        }
}}
