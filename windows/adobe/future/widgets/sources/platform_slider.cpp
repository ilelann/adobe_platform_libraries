/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/platform_slider.hpp>
#include <adobe/future/widgets/headers/platform_widget_utils.hpp>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/


/****************************************************************************************************/

void initialize(adobe::slider_t& control, adobe::platform_display_type parent)
{
    assert(!control.control_m);

	control.control_m = adobe::implementation::make_slider (parent, control.is_vertical_m, control.num_ticks_m, control.style_m);

	if (control.is_vertical_m)
	    adobe::set_font_thumbtop(control.control_m);
	else
	    adobe::set_font_thumbleft(control.control_m);

	adobe::implementation::setup_callback_slider(control);
}

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

void slider_t::on_new_value (model_type new_value)
{
	if (new_value != value_m)
	{
		value_m = new_value;
		value_proc_m (value_m);
	}
}

slider_t::slider_t(const std::string&          alt_text,
                   bool                        is_vertical,
                   slider_style_t              style,
                   std::size_t                 num_ticks,
                   const value_range_format_t& format,
                   theme_t                     theme) :
    control_m(0),
    alt_text_m(alt_text),
    is_vertical_m(is_vertical),
    style_m(style),
    num_ticks_m(num_ticks),
    format_m(format),
    theme_m(theme)
{ }

/****************************************************************************************************/

void slider_t::measure(extents_t& result)
{
#ifdef ADOBE_PLATFORM_WIDGETS_WT
	return;
#else
	int theme_type(is_vertical_m ? TKP_THUMBTOP : TKP_THUMBLEFT);

    //
    // Get the size of the thumb, and then multiply it by the number of tick
    // marks.
    //
    SIZE thumb_size;

    if (!metrics::set_window(control_m))
        ADOBE_THROW_LAST_ERROR;

    if (!metrics::get_size(theme_type, TS_TRUE, thumb_size))
        ADOBE_THROW_LAST_ERROR;

    result.width()  = is_vertical_m ? thumb_size.cx : thumb_size.cx * 10;
    result.height() = is_vertical_m ? thumb_size.cy * 10 : thumb_size.cy;

    long extra(7 + (num_ticks_m ? 6 : 0));

    // Add extra space to accomodate the tick marks. Even in the case when they are not
    // drawn, they are still accounted for in the widget.
    (is_vertical_m ? result.width() : result.height()) += extra;

    //
    // Add on any border.
    //
    int border(0);
    metrics::get_integer(theme_type, TMT_BORDERSIZE, border);

    result.width() += 2 * border;
    result.height() += 2 * border;
#endif
}

/****************************************************************************************************/

void slider_t::place(const place_data_t& place_data)
{
    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void slider_t::enable(bool make_enabled)
{
    assert(control_m);

	set_control_enabled (control_m, make_enabled);
}

/****************************************************************************************************/

void slider_t::display(const model_type& value)
{
    value_m = value;
    
    long new_position(long(format_m.find(any_regular_t(value))));

    if (new_position != last_m)
    {
        last_m = new_position;

        implementation::set_slider_value(control_m, value);
    }
}

/****************************************************************************************************/

template <>
platform_display_type insert<slider_t>(display_t& display, platform_display_type& parent, slider_t& element)
{
    initialize(element, parent);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
