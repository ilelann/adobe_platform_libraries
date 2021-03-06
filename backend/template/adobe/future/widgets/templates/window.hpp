#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/window_helper.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct window : widget_base<Backend>
{
    window(const std::string& name, window_style_t style, window_attributes_t attributes, window_modality_t modality, theme_t)
        : name_m{name}
        , style_m{style}
        , attributes_m{attributes}
        , modality_m{modality}
    {
    }

    ~window()
    {
        if (!this->is_null())
        {
            Backend::destroy_window(this->control_m);
            this->nullify();
        }
    }

    void monitor_resize(const window_resize_proc_t& proc)
    {
        this->resize_proc_m = proc;
    }

    any_regular_t underlying_handler()
    {
        return any_regular_t{this->control_m};
    }

    bool handle_key(key_type /*key*/, bool /*pressed*/, modifiers_t /*modifiers*/)
    {
        return false;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string          name_m;
    window_style_t       style_m;
    window_attributes_t  attributes_m;
    window_modality_t    modality_m;
    window_resize_proc_t resize_proc_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, window<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_window(parent,
                                             element.name_m,
                                             element.attributes_m & (window_attributes_resizeable_s | window_attributes_live_resizeable_s));

    Backend::nullify_when_destroyed(&(element.control_m));

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
