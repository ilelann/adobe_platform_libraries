#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/controller_concept.hpp>
#include <adobe/dictionary_fwd.hpp>
#include <adobe/view_concept.hpp>

#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>

#include <functional>

namespace adobe { namespace implementation {

template <typename Backend>
struct image : widget_base<Backend> {
    /// model types for this widget
    typedef dictionary_t controller_model_type;
    typedef boost::gil::rgba8_image_t view_model_type;
    typedef std::function<void(const controller_model_type&)> setter_proc_type;

    image(const view_model_type& image)
        : image_m{image}, enabled_m{false}
    {}

    void display(const view_model_type& value)
    {
        Backend::set_image(
                this->control_m,
                Backend::make_image_resource(value));
    }

    void monitor(const setter_proc_type& proc)
    {
        callback_m = proc;
    }

    void enable(bool make_enabled)
    {
        this->enabled_m = make_enabled;
        widget_base<Backend>::enable(make_enabled);
    }

#ifndef ADOBE_NO_DOCUMENTATION
    view_model_type image_m;
    setter_proc_type callback_m;
    dictionary_t metadata_m;
    bool enabled_m;
// // mouse tracking stuff
// std::unique_ptr<message_handler_t> handler_m;
// HWND                               prev_capture_m;
// bool                               tracking_m;
// POINTS                             last_point_m;
// std::pair<long, long>              origin_m;
#endif
};

/****************************************************************************************************/

template <typename Backend>
typename Backend::widget_handle
do_insert (display_t& display, typename Backend::widget_handle& parent, image<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_image(parent, Backend::make_image_resource(element.image_m));

    return Backend::post(display, parent, element);
}
}

                  template <typename Backend>
                  struct controller_model_type<implementation::image<Backend> > {
                      typedef typename implementation::image<Backend>::controller_model_type type;
                  };

                  template <typename Backend>
                  struct view_model_type<implementation::image<Backend> > {
                      typedef typename implementation::image<Backend>::view_model_type type;
                  };
                }

