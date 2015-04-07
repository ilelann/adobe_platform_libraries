#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/resources.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <functional>

namespace adobe { namespace implementation {

template <typename Backend>
struct link : widget_base<Backend> {
    typedef any_regular_t model_type;
    typedef std::function<void(const model_type&)> setter_type;

    link(const std::string& alt_text,
         const any_regular_t& on_value,
         const any_regular_t& off_value,
         long count,
         theme_t theme)
            : alt_text_m{alt_text},
              on_value_m{on_value},
              off_value_m{off_value},
              count_m{count},
              theme_m{theme}
    {}


    void display(const any_regular_t& to_value);

    void enable(bool make_enabled)
    {
        assert(!this->is_null());

        widget_base<Backend>::enable(make_enabled);

        bool do_visible = (value_m == on_value_m && make_enabled);

        Backend::visible(this->link_icon_m, do_visible);
        Backend::visible(this->control_m, do_visible);
    }

    void monitor(const setter_type& proc)
    {
        hit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string          alt_text_m;
    any_regular_t on_value_m;
    any_regular_t off_value_m;
    long                 count_m;
    theme_t              theme_m;
//    guide_set_t          prongs_m;
    any_regular_t value_m;
//    point_2d_t           tl_m;
    setter_type          hit_proc_m;
#endif
};

        /****************************************************************************************************/

        template <typename Backend>
        typename Backend::image_resource link_bitmap()
        {
            static auto res = Backend::image_resource_from_name("link_icon.tga");
            return res;
        }

        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, link<Backend>& element)
        {
            Backend::pre(element);

            element.control_m = Backend::make_image(parent, link_bitmap<Backend>());

            return Backend::post(display, parent, element);
        }


    }}
