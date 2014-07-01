/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_radio_button.hpp>

#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>


/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

radio_button_t::radio_button_t(const std::string&          name,
                               const std::string&          alt_text,
                               const any_regular_t& set_value,
                               theme_t              theme) :
    control_m(0),
    name_m(name),
    alt_text_m(alt_text),
    set_value_m(set_value),
    theme_m(theme)
{ }

/****************************************************************************************************/

void radio_button_t::on_clicked()
{
	if (!hit_proc_m.empty())
		hit_proc_m(set_value_m);
}

void radio_button_t::measure(extents_t& result)
{
#ifdef ADOBE_PLATFORM_WIDGETS_WT
	return;
#else
    result = metrics::measure_radiobutton(control_m);

    //
    // Get the text margins, and factor those into the bounds.
    //
    place_data_t margins;

    metrics::set_window(control_m);

    if (metrics::get_button_text_margins(BP_RADIOBUTTON, margins))
    {
        //
        // Add the width margins in. The height margins aren't important because
        // the widget is already large enough to contain big text (as calculated
        // by calculate_best_bounds).
        //
        result.width() += left(margins) + left(margins);
    }
#endif
}

/****************************************************************************************************/

void radio_button_t::place(const place_data_t& place_data)
{
    assert(control_m);

    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void radio_button_t::enable(bool make_enabled)
{
    assert(control_m);
    
	set_control_enabled(control_m, make_enabled);
}

/****************************************************************************************************/

void radio_button_t::display(const any_regular_t& value)
{
    assert(control_m);

    if (last_m == value)
        return;

    last_m = value;

	set_control_checked (control_m, last_m == set_value_m ? check_state(true) : check_state(false));
}

/****************************************************************************************************/

void radio_button_t::monitor(const setter_type& proc)
{
    assert(control_m);

    hit_proc_m = proc;
}

/****************************************************************************************************/

template <>
platform_display_type insert<radio_button_t>(display_t&             display,
                                                platform_display_type& parent,
                                                radio_button_t&     element)
{
	element.control_m = implementation::make_radio_button (parent, element.name_m);

    set_font_radiobutton(element.control_m);

    if (!element.alt_text_m.empty())
        implementation::set_control_alt_text(element.control_m, element.alt_text_m);

	implementation::setup_callback_radio_button(element);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
