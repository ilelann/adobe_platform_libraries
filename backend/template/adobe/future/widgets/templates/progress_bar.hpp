#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/value_range_format.hpp>

#include <adobe/pb_style.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct progress_bar : widget_base<Backend>
{
    using model_type = any_regular_t;

    progress_bar(pb_style_t bar_style, bool is_vertical, const value_range_format_t& format, unsigned long min, unsigned long max, theme_t)
        : bar_style_m{bar_style}
        , is_vertical_m{is_vertical}
        , format_m{format}
    {
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::progress_bar_set_value(this->control_m, format_m.find(value), format_m.size());
    }

#ifndef ADOBE_NO_DOCUMENTATION
    pb_style_t           bar_style_m;
    bool                 is_vertical_m;
    value_range_format_t format_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, progress_bar<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_progress_bar(
        parent, element.is_vertical_m, element.bar_style_m);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
