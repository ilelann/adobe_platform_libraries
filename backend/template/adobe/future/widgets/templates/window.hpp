#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/window_helper.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct window : widget_base<Backend> {
    window(const std::string& name, window_style_t style, window_attributes_t attributes, window_modality_t modality, theme_t theme)
            : name_m{name}, style_m{style}, attributes_m{attributes}, modality_m{modality}, theme_m{theme}
    {}

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

    any_regular_t underlying_handler() { return any_regular_t{this->control_m}; }

    bool handle_key(key_type /*key*/, bool /*pressed*/, modifiers_t /*modifiers*/)
    {
        return false;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string name_m;
    window_style_t       style_m;
    window_attributes_t attributes_m;
    window_modality_t    modality_m;
    theme_t              theme_m;
   window_resize_proc_t resize_proc_m;
#endif
};

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, window<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_window(parent,
                                                     element.name_m,
                                                     element.attributes_m & (window_attributes_resizeable_s | window_attributes_live_resizeable_s));

            return Backend::post(display, parent, element);
        }
}}
