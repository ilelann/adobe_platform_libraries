#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/widget_attributes.hpp>

#include <string>

namespace adobe {
namespace implementation {

template <typename Backend>
struct group : widget_base<Backend>
{
    group(const std::string& name, const std::string& alt_text, theme_t)
        : name_m{name}
        , alt_text_m{alt_text}
    {
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string name_m;
    std::string alt_text_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, group<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_group(parent, element.name_m);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
