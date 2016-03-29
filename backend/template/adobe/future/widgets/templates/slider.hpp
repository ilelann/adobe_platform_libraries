#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/value_range_format.hpp>
#include <adobe/future/widgets/headers/slider_helper.hpp>
#include <adobe/widget_attributes.hpp>

#include <functional>
#include <string>

namespace adobe { namespace implementation {

template <typename Backend>
struct slider : widget_base<Backend> {
    typedef double model_type;
    typedef std::function<void(const model_type&)> setter_type;


    slider(const std::string&          alt_text,
                       bool                        is_vertical,
                       slider_style_t              style,
                       std::size_t                 num_ticks,
                       const value_range_format_t& format,
                       theme_t                     theme) :
            alt_text_m(alt_text),
            is_vertical_m(is_vertical),
            style_m(style),
            num_ticks_m(num_ticks),
            format_m(format),
            theme_m(theme)
    { }

    void display(const model_type& value) {
        value_m = value;

        auto new_position =format_m.find(value);

        if (new_position != last_m) {
            last_m = new_position;
            Backend::set_slider_position(this->control_m, new_position);
        }
    }

    void monitor(const setter_type& proc)
    {
        value_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string alt_text_m;
    bool is_vertical_m;
    slider_style_t style_m;
    std::size_t num_ticks_m;
    value_range_format_t format_m;
    theme_t theme_m;
    setter_type value_proc_m;
    std::size_t last_m; // Used to debounce
    model_type value_m;
#endif
};

        template <typename Backend>
        void on_slider_moved(slider<Backend>& element, long position)
        {
            double new_value(element.format_m.at(position).template cast<double>());

            if (new_value != element.value_m) {
                element.value_m = new_value;

                using model_type = typename slider<Backend>::model_type;

                element.value_proc_m(static_cast<model_type>(element.value_m));
            }
        }

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, slider<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_slider(parent,
                                                     element.alt_text_m,
                                                     element.is_vertical_m,
                                                     element.num_ticks_m,
                                                     element.style_m,
                                                     element.format_m.size(),
                                                     element);

            return Backend::post(display, parent, element);
        }



    }}
