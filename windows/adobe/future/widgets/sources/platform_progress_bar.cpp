/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_progress_bar.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/platform_widget_utils.hpp>
#include <adobe/empty.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

void progress_bar_t::display(const any_regular_t& value)
{
    value_m = empty(value) ? 0 : value.cast<double>();
    
    std::size_t new_position(format_m.find(value));

    if (new_position != last_m)
    {
        last_m = new_position;
        if(bar_style_m != pb_style_indeterminate_bar_s)
        {
            implementation::set_progress_bar_position (control_m, last_m);
        }        
    }
}

/*************************************************************************************************/

void progress_bar_t::measure(extents_t& result)
{
    assert(control_m);

    //
    // REVISIT (ralpht): This is easy to pull out of UxTheme!
    //

    result.height() = is_vertical_m ? 100 : 15;
    result.width() = is_vertical_m ? 15 : 100;
}

/*************************************************************************************************/

void progress_bar_t::place(const place_data_t& place_data)
{
    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

progress_bar_t::progress_bar_t(pb_style_t bar_style, 
                               bool is_vertical,
                               const value_range_format_t& format,
                               unsigned long /*min*/,
                               unsigned long /*max*/,
                               theme_t theme)    
          :
    control_m(0),
    bar_style_m(bar_style),
    is_vertical_m(is_vertical),
    format_m(format),
    theme_m(theme)
    { }

/****************************************************************************************************/

void progress_bar_t::set_min_value(long /* min_value */)
{
}

/****************************************************************************************************/

void progress_bar_t::set_max_value(long /* max_value */)
{
}

/****************************************************************************************************/

template <>
platform_display_type insert<progress_bar_t>(display_t&              display,
                                                    platform_display_type&  parent,
                                                    progress_bar_t&  element)
{
    assert(is_null_control(element.control_m));

    element.control_m = implementation::make_progress_bar(parent, element.is_vertical_m, element.bar_style_m);

    set_font_progressbar(element.control_m);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

}

