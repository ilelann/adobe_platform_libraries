#pragma once

#include <adobe/any_regular.hpp>

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct reveal;

template <typename Backend>
typename Backend::image_resource reveal_current_bitmap(reveal<Backend>& element);

template <typename Backend>
struct reveal : widget_base<Backend>
{
    using model_type  = any_regular_t;
    using setter_type = std::function<void(const model_type&)>;

    reveal(const std::string& name, const any_regular_t& show_value, theme_t theme, const std::string& alt_text)
        : name_m{name, {}, 0, theme}
        , show_value_m{show_value}
        , alt_text_m{alt_text}
    {
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_image(this->control_m, reveal_current_bitmap(*this));
    }

    void monitor(const setter_type& proc)
    {
        hit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    label<Backend> name_m;
    any_regular_t  show_value_m;
    std::string    alt_text_m;
    bool           revealed_m;
    bool           using_label_m;
    setter_type    hit_proc_m;
#endif
};

template <typename Backend>
void on_reveal_clicked(const reveal<Backend>& element)
{
    if (!element.hit_proc_m)
        return;

    // toggle it.
    element.hit_proc_m(element.revealed_m ? any_regular_t{} : element.show_value_m);
}

template <typename Backend>
typename Backend::image_resource reveal_bitmap_showing()
{
    static auto res = Backend::image_resource_from_name("windows_reveal_down.tga");
    return res;
}

template <typename Backend>
typename Backend::image_resource reveal_bitmap_hidden()
{
    static auto res = Backend::image_resource_from_name("windows_reveal_up.tga");
    return res;
}

template <typename Backend>
typename Backend::image_resource reveal_current_bitmap(reveal<Backend>& element)
{
    return element.revealed_m ? reveal_bitmap_showing<Backend>() : reveal_bitmap_hidden<Backend>();
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, reveal<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_reveal(parent, reveal_current_bitmap(element), element.alt_text_m, element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
