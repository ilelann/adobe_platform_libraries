/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_reveal.hpp>

#include <adobe/empty.hpp>
#include <adobe/future/image_slurp.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/windows_graphic_utils.hpp>
#include <adobe/future/windows_cast.hpp>
#include <adobe/placeable_concept.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

native_image_resource_t bitmap_showing()
{
    static native_image_resource_t bitmap_s = implementation::make_image_resource("windows_reveal_down.tga");
    return bitmap_s;
}

/****************************************************************************************************/

native_image_resource_t bitmap_hidden()
{
    static native_image_resource_t bitmap_s = implementation::make_image_resource("windows_reveal_up.tga");
    return bitmap_s;
}

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/


reveal_t::reveal_t(const std::string&			name,
                   const any_regular_t&	show_value,
                   theme_t						theme,
                   const std::string&			alt_text) :
    control_m(0),
    theme_m(theme),
    name_m(name, std::string(), 0, theme),
    using_label_m(!name.empty()),
    show_value_m(show_value),
    alt_text_m(alt_text)
{
}

/****************************************************************************************************/

void reveal_t::measure(extents_t& result)
{
    using adobe::measure;

    // REVISIT (fbrereto) : hardwired defaults
    result.width() = 16;
    result.height() = 17;

    if (!using_label_m)
        return;

    extents_t label_extents;

    measure(name_m, label_extents);

    place_data_t label_place;

    width(label_place) = label_extents.width();
    height(label_place) = label_extents.height();

    measure_vertical(name_m, label_extents, label_place);

    result.width() += 4 /* gap */ + label_extents.width();
    result.height() = (std::max)(result.height(), label_extents.height());
}

/****************************************************************************************************/

void reveal_t::on_clicked()
{
            if (hit_proc_m.empty())
            return;

        // toggle it.
        adobe::any_regular_t new_value =
            current_value_m == show_value_m ?
                adobe::any_regular_t(adobe::empty_t()) :
                show_value_m;

        hit_proc_m(new_value);
}

/****************************************************************************************************/

void reveal_t::place(const place_data_t& place_data)
{
    using adobe::place;

    assert(control_m);

    if (!using_label_m)
    {
        implementation::set_control_bounds(control_m, place_data);
    }
    else
    {
        place_data_t tmp(place_data);

        // REVISIT (fbrereto) : hardwired defaults
        width(tmp) = 16;
        height(tmp) = 17;

        implementation::set_control_bounds(control_m, tmp);

        width(tmp) = width(place_data) - 16 - 4 /* gap */;
        left(tmp) = left(place_data) + 16 + 4 /* gap */;

        place(name_m, tmp);
    }
}

/****************************************************************************************************/

void reveal_t::display(const any_regular_t& new_value)
{
    assert(control_m);

    if (current_value_m == new_value)
        return;

    current_value_m = new_value;

    reset_image(control_m, (current_value_m == show_value_m) ? bitmap_showing() : bitmap_hidden(), false);
}

/****************************************************************************************************/

void reveal_t::monitor(const setter_type& proc)
{
    assert(control_m);

    hit_proc_m = proc;
}

/****************************************************************************************************/      

// REVISIT: MM--we need to replace the display_t mechanism with concepts/any_*/container idiom for event and drawing system.

template <>
platform_display_type insert<reveal_t>(display_t&             display,
                                              platform_display_type& parent,
                                              reveal_t&       element)
{
    assert(is_null_control(element.control_m));

    element.control_m = implementation::make_reveal(parent);

    set_font_edittext(element.control_m);

    implementation::setup_callback_reveal(element);

    if (!element.alt_text_m.empty())
        implementation::set_control_alt_text(element.control_m, element.alt_text_m);

    if (element.using_label_m){
        insert(display, parent, element.name_m);
    }

	return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

}


/****************************************************************************************************/
