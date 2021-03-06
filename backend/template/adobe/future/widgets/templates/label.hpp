#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/widget_attributes.hpp>

#include <string>

namespace adobe {
namespace implementation {

template <typename Backend>
struct label : widget_base<Backend>
{
    label(const std::string& name, const std::string& alt_text, std::size_t characters, theme_t)
        : name_m{name}
        , alt_text_m{alt_text}
        , characters_m{characters}
    {
    }

    std::string name_m;
    std::string alt_text_m;
    std::size_t characters_m;
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, label<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_label(parent, element.name_m, element.alt_text_m);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
