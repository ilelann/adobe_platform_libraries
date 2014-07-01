/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_image.hpp>

#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_widget_utils.hpp>

#include <adobe/future/windows_cast.hpp>
#include <adobe/future/windows_graphic_utils.hpp>
#include <adobe/memory.hpp>
#include <adobe/unicode.hpp>

#include <string>
#include <cassert>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

const long fixed_width = 250;
const long fixed_height = fixed_width;

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

image_t::image_t(const view_model_type& image) :
    control_m(0),
    image_m(image),
    enabled_m(false),
    tracking_m(false)
{
    metadata_m.insert(dictionary_t::value_type("delta_x"_name, any_regular_t(0)));
    metadata_m.insert(dictionary_t::value_type("delta_y"_name, any_regular_t(0)));
    metadata_m.insert(dictionary_t::value_type("dragging"_name, any_regular_t(false)));
    metadata_m.insert(dictionary_t::value_type("x"_name, any_regular_t(0)));
    metadata_m.insert(dictionary_t::value_type("y"_name, any_regular_t(0)));
}

/****************************************************************************************************/

void image_t::on_mouse_moved(const point_t & cur_point)
{
      if (tracking_m)
      {
        if (last_point_m.y != cur_point.y ||
            last_point_m.x != cur_point.x)
        {
            double x(0);
            double y(0);
            double delta_x(last_point_m.x - cur_point.x);
            double delta_y(last_point_m.y - cur_point.y);

            get_value(metadata_m, "x"_name, x);
            get_value(metadata_m, "y"_name, y);

            metadata_m.insert(dictionary_t::value_type("delta_x"_name, any_regular_t(delta_x)));
            metadata_m.insert(dictionary_t::value_type("delta_y"_name, any_regular_t(delta_y)));
            metadata_m.insert(dictionary_t::value_type("dragging"_name, any_regular_t(true)));
            metadata_m.insert(dictionary_t::value_type("x"_name, any_regular_t(x + delta_x)));
            metadata_m.insert(dictionary_t::value_type("y"_name, any_regular_t(y + delta_y)));

            callback_m(metadata_m);
        }
        last_point_m = cur_point;
      }
}

/****************************************************************************************************/

void image_t::on_mouse_down(const point_t & cur_point)
    {
        tracking_m = true;
        last_point_m = cur_point;
    }

/****************************************************************************************************/

void image_t::on_mouse_up(const point_t &)
    {
        tracking_m = false;

        metadata_m.insert(dictionary_t::value_type("delta_x"_name, any_regular_t(0)));
        metadata_m.insert(dictionary_t::value_type("delta_y"_name, any_regular_t(0)));
        metadata_m.insert(dictionary_t::value_type("dragging"_name, any_regular_t(false)));

    callback_m(metadata_m);
    }

/****************************************************************************************************/

void image_t::display(const view_model_type& value)
{
    image_m = value;

    if (image_m.width() <= fixed_width)
        origin_m.first = 0;
    else
        origin_m.first = static_cast<long>((image_m.width() - fixed_width) / 2);

    if (image_m.height() <= fixed_height)
       origin_m.second = 0;
    else
        origin_m.second = static_cast<long>((image_m.height() - fixed_height) / 2);

    reset_image(control_m, implementation::make_image_resource(image_m), true);
}

/****************************************************************************************************/


void image_t::monitor(const setter_proc_type& proc)
{
    callback_m = proc;
}

/****************************************************************************************************/

void image_t::enable(bool make_enabled)
{
    enabled_m = make_enabled;
    set_control_enabled(control_m, make_enabled);
}

/****************************************************************************************************/

void place(image_t& value, const place_data_t& place_data)
{
    implementation::set_control_bounds(value.control_m, place_data);

    if (value.callback_m)
    {
        dictionary_t old_metadata(value.metadata_m);

        double width(std::min<double>(fixed_width, value.image_m.width()));
        double height(std::min<double>(fixed_height, value.image_m.height()));

        value.metadata_m.insert(dictionary_t::value_type("width"_name, any_regular_t(width)));
        value.metadata_m.insert(dictionary_t::value_type("height"_name, any_regular_t(height)));

        if (old_metadata != value.metadata_m)
            value.callback_m(value.metadata_m);
    }
}

/****************************************************************************************************/

void measure(image_t& value, extents_t& result)
{
    if (value.callback_m)
        result.horizontal().length_m = fixed_width;
    else
        result.horizontal().length_m = (long)value.image_m.width();
}

/****************************************************************************************************/

void measure_vertical(image_t& value, extents_t& result, const place_data_t& placed_horizontal)
{
    if (value.callback_m)
        result.vertical().length_m = fixed_height;
    else
    {
        // NOTE (fbrereto) : We calculate and use the aspect ratio here to
        //                   maintain a proper initial height and width in
        //                   the case when the image grew based on how it
        //                   is being laid out.

        float aspect_ratio(value.image_m.height() / static_cast<float>(value.image_m.width()));

        result.vertical().length_m = static_cast<long>(placed_horizontal.horizontal().length_m * aspect_ratio);
    }
}

/****************************************************************************************************/

void enable(image_t& value, bool make_enabled)
{
    value.enable(make_enabled);
}

/****************************************************************************************************/

template <>
platform_display_type insert<image_t>(display_t&             display,
                                             platform_display_type& parent,
                                             image_t&        element)
{
    assert(is_null_control(element.control_m));

    element.control_m = implementation::make_image (parent);

    reset_image(element.control_m, implementation::make_image_resource(element.image_m), true);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
