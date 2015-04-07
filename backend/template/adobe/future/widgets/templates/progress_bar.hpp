#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/value_range_format.hpp>

#include <adobe/pb_style.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct progress_bar : widget_base<Backend> {
    typedef progress_bar widget_type_t;
    typedef any_regular_t model_type;

    progress_bar(pb_style_t bar_style, bool is_vertical, const value_range_format_t& format, unsigned long min, unsigned long max, theme_t theme)
        : bar_style_m{ bar_style }
        , is_vertical_m{ is_vertical }
        , format_m{ format }
        , theme_m{ theme }
    {
    }

    void display(const any_regular_t& value)
    {
        Backend::progress_bar_set_value(this->control_m,
                                        format_m.find(value.cast<double>()), format_m.size());
    }

    void set_min_value(long min_value) {}
    void set_max_value(long max_value) {}

#ifndef ADOBE_NO_DOCUMENTATION
    pb_style_t bar_style_m;
    bool is_vertical_m;
    value_range_format_t format_m;
    theme_t theme_m;
#endif
};

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, progress_bar<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_progress_bar(
                    parent, element.is_vertical_m, element.bar_style_m);

            return Backend::post(display, parent, element);
        }

    }}
