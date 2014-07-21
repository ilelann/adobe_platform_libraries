/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_preview.hpp>

#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace implementation {} // namespace implementation

/****************************************************************************************************/

preview_t::preview_t(const std::string& alt_text,
                     theme_t            theme) :
    alt_text_m(alt_text),
    sublayout_m(theme),
    theme_m(theme)
{ }

/****************************************************************************************************/

void preview_t::measure(extents_t& result)
{
    sublayout_m.measure(result);
}

/****************************************************************************************************/

void preview_t::place(const place_data_t& place_data)
{
    sublayout_m.place(place_data);
}

/****************************************************************************************************/

void preview_t::display(const view_model_type& value)
{
    sublayout_m.sublayout_sheet_set_update("image"_name, value);
}

/****************************************************************************************************/

void preview_t::enable(bool /*make_enabled*/)
{
}

/****************************************************************************************************/

void preview_t::monitor(const setter_proc_type& /*proc*/)
{
}

/****************************************************************************************************/

template <>
platform_display_type insert<preview_t>(display_t&,
                                        platform_display_type& parent,
                                        preview_t&)
{
    return parent;
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
