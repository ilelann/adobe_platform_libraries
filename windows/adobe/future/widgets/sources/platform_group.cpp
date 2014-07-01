/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_group.hpp>

#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/display.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

group_t::group_t(const std::string& name,
                 const std::string& alt_text,
                 theme_t     theme) :
    control_m(0),
    name_m(name),
    alt_text_m(alt_text),
    theme_m(theme)
{ }

/****************************************************************************************************/

void group_t::measure(extents_t& result)
{
    assert(control_m);

    if (name_m.empty())
    {
        result.height() = 15;
        result.width() = 15;

        return;
    }

    // REVISIT (fbrereto) : A lot of static metrics values added here

    result = measure_text(name_m, theme_m, get_parent_control(control_m));

    result.width() += 15;

    result.vertical().frame_m.first = result.height() + 7;

    result.height() = 5;
}

/****************************************************************************************************/

void group_t::place(const place_data_t& place_data)
{
    assert(control_m);

    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

template <>
platform_display_type insert<group_t>(display_t&             display,
                                             platform_display_type& parent,
                                             group_t&        element)
{
    assert(is_null_control(element.control_m));

  element.control_m = implementation::make_group (parent, element.name_m);

    set_font_groupbox(element.control_m);

    implementation::setup_callback_group(element);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
