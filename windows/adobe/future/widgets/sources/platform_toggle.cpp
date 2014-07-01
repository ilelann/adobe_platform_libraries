/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_toggle.hpp>

#include <adobe/future/windows_graphic_utils.hpp>
#include <adobe/future/widgets/headers/button_helper.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

const adobe::toggle_t::image_type& current_image(adobe::toggle_t& toggle)
{
	if (adobe::get_control_enabled(toggle.control_m))
    {
        if (toggle.last_m == toggle.value_on_m)
            return toggle.image_on_m;
        else
            return toggle.image_off_m;
    }
    else // disabled_button
    {
        return toggle.image_disabled_m;
    }
}

/****************************************************************************************************/

adobe::native_image_resource_t current_bitmap(adobe::toggle_t& toggle)
{
	if (adobe::get_control_enabled(toggle.control_m))
    {
        if (toggle.last_m == toggle.value_on_m)
            return toggle.bitmap_on_m;
        else
            return toggle.bitmap_off_m;
    }
    else // disabled_button
    {
        return toggle.bitmap_disabled_m;
    }
}

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

toggle_t::toggle_t(const std::string&  alt_text,
                   const any_regular_t value_on,
                   const image_type&   image_on,
                   const image_type&   image_off,
                   const image_type&   image_disabled,
                   theme_t             theme) :
    control_m(0),
    theme_m(theme),
    alt_text_m(alt_text),
    image_on_m(image_on),
    image_off_m(image_off),
    image_disabled_m(image_disabled),
    value_on_m(value_on),
    bitmap_on_m(implementation::make_image_resource(image_on)),
    bitmap_off_m(implementation::make_image_resource(image_off)),
    bitmap_disabled_m(implementation::make_image_resource(image_disabled))
{ }

/****************************************************************************************************/

void toggle_t::on_clicked()
{
        if (setter_proc_m.empty())
            return;

        // toggle it.
        adobe::any_regular_t new_value =
            last_m == value_on_m ?
                adobe::any_regular_t(adobe::empty_t()) :
                value_on_m;

        setter_proc_m(new_value);
}

/****************************************************************************************************/

void toggle_t::measure(extents_t& result)
{
    assert(control_m);

    result = extents_t();

    const adobe::toggle_t::image_type& image(current_image(*this));

    result.height() = static_cast<long>(image.height());
    result.width() = static_cast<long>(image.width());
}

/****************************************************************************************************/

void toggle_t::place(const place_data_t& place_data)
{
    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void toggle_t::enable(bool make_enabled)
{
    assert(control_m);

	set_control_enabled(control_m, make_enabled);

    reset_image(control_m, current_bitmap(*this), false);
}

/****************************************************************************************************/

void toggle_t::display(const any_regular_t& to_value)
{
    assert(control_m);

    if (last_m == to_value)
        return;

    last_m = to_value;

    reset_image(control_m, current_bitmap(*this), false);
}

/****************************************************************************************************/

void toggle_t::monitor(const setter_type& proc)
{
    assert(control_m);

    setter_proc_m = proc;
}

/****************************************************************************************************/

template <>
platform_display_type insert<toggle_t>(display_t&             display,
                                       platform_display_type& parent,
                                       toggle_t&              element)
{
    assert(is_null_control(element.control_m));

    element.control_m = implementation::make_toggle (parent);

    set_font_edittext(element.control_m); // REVISIT (fbrereto) : a better type?

    implementation::setup_callback_toggle(element);

    if (!element.alt_text_m.empty())
        implementation::set_control_alt_text(element.control_m, element.alt_text_m);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
