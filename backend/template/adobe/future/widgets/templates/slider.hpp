#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/slider_helper.hpp>
#include <adobe/future/widgets/headers/value_range_format.hpp>

#include <adobe/widget_attributes.hpp>

#include <functional>

namespace adobe {
namespace implementation {

template <typename Backend>
struct slider : widget_base<Backend>
{
    using model_type  = double;
    using setter_type = std::function<void(const model_type&)>;

    slider(const std::string&          alt_text,
           bool                        is_vertical,
           slider_style_t              style,
           std::size_t                 num_ticks,
           const value_range_format_t& format,
           theme_t)
        : alt_text_m{alt_text}
        , is_vertical_m{is_vertical}
        , style_m{style}
        , num_ticks_m{num_ticks}
        , format_m{format}
    {
    }

    void display(const model_type& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_slider_position(this->control_m, format_m.find(value));
    }

    void monitor(const setter_type& proc)
    {
        value_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string          alt_text_m;
    bool                 is_vertical_m;
    slider_style_t       style_m;
    std::size_t          num_ticks_m;
    value_range_format_t format_m;
    setter_type          value_proc_m;
#endif
};

template <typename Backend>
void on_slider_moved(const slider<Backend>& element, long position)
{
    using model_type = typename slider<Backend>::model_type;
    element.value_proc_m(element.format_m.at(position).template cast<model_type>());
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, slider<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_slider(parent,
                                             element.alt_text_m,
                                             element.is_vertical_m,
                                             element.num_ticks_m,
                                             element.style_m,
                                             element.format_m.size(),
                                             element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
