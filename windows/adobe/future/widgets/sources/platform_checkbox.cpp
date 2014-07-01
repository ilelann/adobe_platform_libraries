/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_checkbox.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/display.hpp>


/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

checkbox_t::checkbox_t( const std::string&			name,
                        const any_regular_t& true_value,
                        const any_regular_t& false_value,
                        theme_t						theme,
                        const std::string&			alt_text) :
    control_m(0),
    theme_m(theme),
    true_value_m(true_value),
    false_value_m(false_value),
    name_m(name),
    alt_text_m(alt_text)
{ }


/****************************************************************************************************/

void checkbox_t::on_clicked()
{
	if (hit_proc_m.empty())
		return;

	adobe::any_regular_t new_value(true_value_m);

	if (current_value_m == true_value_m)
		new_value = false_value_m;

	hit_proc_m(new_value);
}

/****************************************************************************************************/

void checkbox_t::measure(extents_t& result)
{
#ifdef ADOBE_PLATFORM_WIDGETS_WT
	return;
#else
    result = metrics::measure_checkbox(control_m);

    //
    // Get the text margins, and factor those into the bounds.
    //
    place_data_t margins;

    metrics::set_window(control_m);

    if (metrics::get_button_text_margins(BP_CHECKBOX, margins))
    {
        //
        // Add the width margins in. The height margins aren't important because
        // the widget is already large enough to contain big text (as calculated
        // by calculate_best_bounds).
        //
        result.width() += left(margins) + right(margins);
    }
#endif
}

/****************************************************************************************************/

void checkbox_t::place(const place_data_t& place_data)
{
    assert(control_m);
    
    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void checkbox_t::enable(bool make_enabled)
{
    assert(control_m);
    
	set_control_enabled (control_m, make_enabled);
}

/****************************************************************************************************/

void checkbox_t::display(const any_regular_t& new_value)
{
    assert(control_m);

    if (current_value_m == new_value) return;

    current_value_m = new_value;

	check_state state = boost::logic::indeterminate;
		
    if (current_value_m == true_value_m)
        state = true;
    else if (current_value_m == false_value_m)
        state = false;

	set_control_checked (control_m, state);
}

/****************************************************************************************************/

void checkbox_t::monitor(const setter_type& proc)
{
    assert(control_m);

    hit_proc_m = proc;
}

/****************************************************************************************************/

template <>
platform_display_type insert<checkbox_t>(display_t&             display,
                                                platform_display_type& parent,
                                                checkbox_t&     element)
{
	assert(adobe::is_null_control(element.control_m));

	element.control_m = adobe::implementation::make_checkbox (parent, element.name_m);

	implementation::setup_callback_checkbox(element);

    adobe::set_font_checkbox(element.control_m);

    if (!element.alt_text_m.empty())
        adobe::implementation::set_control_alt_text(element.control_m, element.alt_text_m);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
