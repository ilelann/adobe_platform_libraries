/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#ifndef ADOBE_IMAGE_T_HPP
#define ADOBE_IMAGE_T_HPP

/****************************************************************************************************/

#include <adobe/dictionary.hpp>
#include <adobe/memory.hpp>
#include <adobe/layout_attributes.hpp>
#include <adobe/future/platform_primitives.hpp>

#include <boost/function.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/noncopyable.hpp>

#include <string>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

struct image_t : boost::noncopyable
{
    /// model types for this widget
    typedef dictionary_t                                         controller_model_type;
    typedef boost::gil::rgba8_image_t                            view_model_type;

    typedef boost::function<void (const controller_model_type&)> setter_proc_type;

    image_t(const view_model_type& image);

	window_proc_t handle_event;

    void display(const view_model_type& value);

    void monitor(const setter_proc_type& proc);
    void enable(bool make_enabled);

    native_image_t                     control_m;
    view_model_type                    image_m;
    std::string                        image_path_m;
    setter_proc_type                   callback_m;
    dictionary_t                       metadata_m;
    bool                               enabled_m;

    platform_display_type                               prev_capture_m;
    bool                               tracking_m;
    point_t                            last_point_m;
    std::pair<long, long>              origin_m;

void on_mouse_moved(const point_t & cur_point);
void on_mouse_down(const point_t & cur_point);
void on_mouse_up(const point_t & cur_point);

};

/****************************************************************************************************/

void measure(image_t& value, extents_t& result);

void measure_vertical(image_t& value, extents_t& calculated_horizontal,
                      const place_data_t& placed_horizontal);

void place(image_t& value, const place_data_t& place_data);

void enable(image_t& value, bool make_enabled);

/****************************************************************************************************/

template <typename T> struct controller_model_type;
template <>
struct controller_model_type<adobe::image_t>
{ typedef adobe::image_t::controller_model_type type; };

template <typename T> struct view_model_type;
template <>
struct view_model_type<adobe::image_t>
{ typedef adobe::image_t::view_model_type type; };

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif

/****************************************************************************************************/
