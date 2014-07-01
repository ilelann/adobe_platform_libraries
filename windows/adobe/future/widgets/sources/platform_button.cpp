/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_button.hpp>

#include <adobe/future/widgets/headers/platform_label.hpp>

#include <adobe/future/widgets/headers/button_helper.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/platform_widget_utils.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

button_t::button_t(bool                             is_default,
                   bool                             is_cancel,
                   modifiers_t               modifier_mask,
                   const button_state_descriptor_t* first,
                   const button_state_descriptor_t* last,
                   theme_t                          theme) :
    control_m(0),
    theme_m(theme),
    state_set_m(first, last),
    modifier_mask_m(modifier_mask),
    modifiers_m(modifiers_none_s),
    is_default_m(is_default),
    is_cancel_m(is_cancel),
    enabled_m(true)
{ }

/****************************************************************************************************/

void button_t::on_clicked ()
{
	adobe::button_state_set_t::iterator state(adobe::button_modifier_state(state_set_m,
                                                                           modifier_mask_m,
                                                                           modifiers_m));

        if (state == state_set_m.end())
            state = adobe::button_default_state(state_set_m);

        if (!state->hit_proc_m.empty())
            state->hit_proc_m(state->value_m, state->contributing_m);
}

/****************************************************************************************************/

void button_t::measure(extents_t& result)
{
    // We want the size of the button to be state independant

    // get current size
    result = metrics::measure_pushbutton(control_m);

    button_state_set_t::iterator state(button_modifier_state(state_set_m,
                                                                           modifier_mask_m,
                                                                           modifiers_m));

    // substracting current text size
    if (state == state_set_m.end())
        state = button_default_state(state_set_m);

	extents_t cur_text_extents(measure_text(state->name_m, theme_m, get_parent_control(control_m)));

    result.width() -= cur_text_extents.width();
    result.height() -= cur_text_extents.height();

    // get max text size. adding it gives the state independant size
    long width_additional(0);
    long height_additional(0);

    for (button_state_set_t::iterator iter(state_set_m.begin()), last(state_set_m.end()); iter != last; ++iter)
    {
        extents_t tmp(measure_text(iter->name_m, theme_m, get_parent_control(control_m)));

        width_additional = std::max<int>(width_additional, tmp.width());
        height_additional = std::max<int>(height_additional, tmp.height());
    }

    result.width() += width_additional;
    result.height() += height_additional;

    result.width() = std::max<int>(result.width(), 70L);
}

/****************************************************************************************************/

void button_t::place(const place_data_t& place_data)
{
    assert(control_m);

    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void button_t::enable(bool make_enabled)
{
    enabled_m = make_enabled;
    if(control_m)
        set_control_enabled (control_m, make_enabled);
}

/****************************************************************************************************/

void button_t::set(modifiers_t modifiers, const model_type& value)
{
    button_state_set_t::iterator state(button_modifier_state(state_set_m, modifier_mask_m, modifiers));

    if (state == state_set_m.end())
        state = button_default_state(state_set_m);

    if (state->value_m != value)
        state->value_m = value;
}

/****************************************************************************************************/

void button_t::set_contributing(modifiers_t modifiers, const dictionary_t& value)
{
    button_state_set_t::iterator state(button_modifier_state(state_set_m, modifier_mask_m, modifiers));

    if (state == state_set_m.end())
        state = button_default_state(state_set_m);

    state->contributing_m = value;
}

/****************************************************************************************************/

bool button_t::handle_key(key_type key, bool pressed, modifiers_t modifiers)
{
    if (pressed == false)
        return false;

    modifiers_m = modifiers /*modifier_state()*/;

    //
    // Look up the state which this modifier should trigger.
    //
    button_state_set_t::iterator state(button_modifier_state(state_set_m,
                                                                           modifier_mask_m,
                                                                           modifiers_m));

    if (state == state_set_m.end())
        state = button_default_state(state_set_m);

    //
    // Set the window text.
    //
    implementation::set_control_string(control_m, state->name_m);

    //
    // Set the alt text if need be.
    //
    if (!state->alt_text_m.empty())
        implementation::set_control_alt_text(control_m, state->alt_text_m);

    if (state->hit_proc_m.empty() || enabled_m == false)
        return false;

    if (is_key_return(key) && is_default_m) // return
    {
        state->hit_proc_m(state->value_m, state->contributing_m);
    }
    else if (is_key_escape(key) && is_cancel_m) // escape
    {
        state->hit_proc_m(state->value_m, state->contributing_m);
    }
    else
    {
        return false;
    }

    return true;
}

/****************************************************************************************************/

template <>
platform_display_type insert<button_t>(display_t&             display,
                                                platform_display_type& parent,
                                                button_t&     element)
{
    assert(is_null_control(element.control_m));

	button_state_set_t::iterator state(button_default_state(element.state_set_m));

	element.control_m = implementation::make_button (parent, state->name_m, element.is_default_m);

	set_font_pushbutton(element.control_m);

	implementation::setup_callback_button(element);

    if (!state->alt_text_m.empty())
        implementation::set_control_alt_text(element.control_m, state->alt_text_m);

    platform_display_type result(display.insert(parent, element.control_m));
    set_control_enabled (element.control_m, element.enabled_m);
    return result;
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
