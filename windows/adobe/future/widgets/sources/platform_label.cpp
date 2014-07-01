/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_label.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/windows_cast.hpp>
#include <adobe/unicode.hpp>
#include <adobe/placeable_concept.hpp>
#include <string>
#include <cassert>


/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

label_t::label_t(const std::string& name,
                 const std::string& alt_text,
                 std::size_t        characters,
                 theme_t            theme) :
    control_m(0),
    characters_m(characters),
    theme_m(theme),
    alt_text_m(alt_text),
	name_m(name)
{
}

/****************************************************************************************************/

label_t::~label_t()
{
// ilelann TODO FIXME ? : on widget that destroy .. don't know why
#ifdef ADOBE_PLATFORM_WIDGETS_WIN 
	::DestroyWindow(control_m);
#endif
}

/****************************************************************************************************/

void place(label_t& value, const place_data_t& place_data)
{
    implementation::set_control_bounds(value.control_m, place_data);

    if (!value.alt_text_m.empty())
        implementation::set_control_alt_text(value.control_m, value.alt_text_m);
}

/****************************************************************************************************/

void measure(label_t& value, extents_t& result)
{
#ifdef ADOBE_PLATFORM_WIDGETS_WT
	return;
#else
    assert(value.control_m);

    metrics::set_window(value.control_m);
//    metrics::set_theme_name(L"Edit");
    //
    // If we don't have the type of this widget, then we should return a
    // zero sized rectangle. This is usually correct, and a good assumption
    // anyway.
    //
    int uxtheme_type = EP_EDITTEXT;
    //
    // We need the text dimensions to figure out what the width of the widget should
    // be.
    //
    place_data_t text_extents;
    place_data_t in_extents;
	width(in_extents) = 10000;
	height(in_extents) = 10000;
    
    if(value.characters_m){
        std::wstring wrap(value.characters_m * 2, L'0');
        bool have_extents = metrics::get_text_extents(uxtheme_type,
            wrap.c_str(), text_extents, &in_extents);
        assert(have_extents);
        in_extents = text_extents;
        height(in_extents) = 10000 - top(in_extents);
    }
    
    std::wstring wname;
    to_utf16(value.name_m.begin(), value.name_m.end(), back_inserter(wname));
    
    bool have_extents = metrics::get_text_extents(uxtheme_type,
        wname.c_str(), text_extents, &in_extents);

    //
    // Now we can calculate the size we want to return.
    //
    result.width() = width(text_extents);
    assert(result.width());
#endif
}

/****************************************************************************************************/

void measure_vertical(label_t& value, extents_t& calculated_horizontal, 
                      const place_data_t& placed_horizontal)
{
#ifdef ADOBE_PLATFORM_WIDGETS_WT
	return;
#else
    assert(value.control_m);

    place_data_t save_bounds;
    implementation::get_control_bounds(value.control_m, save_bounds);

    place_data_t static_bounds;

    top(static_bounds) = top(placed_horizontal);
    left(static_bounds) = left(placed_horizontal);
    width(static_bounds) = width(placed_horizontal);
    height(static_bounds) = 10000; // bottomless

    implementation::set_control_bounds(value.control_m, static_bounds);

    std::string title(implementation::get_control_string(value.control_m));

    std::wstring wtitle;
    to_utf16(title.begin(), title.end(), std::back_inserter(wtitle));
    place_data_t out_extent;

//    metrics::set_theme_name(L"Edit");
    //
    // If we don't have the type of this widget, then we should return a
    // zero sized rectangle. This is usually correct, and a good assumption
    // anyway.
    //
    int uxtheme_type = EP_EDITTEXT;
    //
    // Get the text metrics (and calculate the baseline of this widget)
    //
    TEXTMETRIC widget_tm;
    bool have_tm = metrics::get_font_metrics(uxtheme_type, widget_tm);

    assert(have_tm);

    const place_data_t in_extents = static_bounds;

    bool have_extents = metrics::get_text_extents(uxtheme_type,
        wtitle.c_str(), out_extent, &in_extents);
    
    assert(have_extents);

    extents_t::slice_t& vert = calculated_horizontal.vertical();
    vert.length_m = height(out_extent);
    // set the baseline for the text
 
    metrics::set_window(value.control_m);

    if (have_tm)
        // distance from top to baseline
        vert.guide_set_m.push_back(widget_tm.tmHeight - widget_tm.tmDescent);

    implementation::set_control_bounds(value.control_m, save_bounds);
#endif
}

/****************************************************************************************************/

void enable(label_t& value, bool make_enabled)
{
	set_control_enabled(value.control_m, make_enabled);
}

/****************************************************************************************************/

void initialize(label_t& label, platform_display_type parent)
{
    assert(is_null_control(label.control_m));

	label.control_m = implementation::make_label (parent, label.name_m);

    set_font_edittext(label.control_m);

    if (!label.alt_text_m.empty())
        implementation::set_control_alt_text(label.control_m, label.alt_text_m);
}

/****************************************************************************************************/

extents_t measure_text(const std::string& text, theme_t theme, platform_display_type temp_parent)
{
    label_t          label(text, std::string(), 0, theme);
    extents_t result;

    measure_label_text(label, result, temp_parent);

    return result;
}

/****************************************************************************************************/

void measure_label_text(const label_t& label, extents_t& result, platform_display_type temp_parent)
{
    label_t&  temp(const_cast<label_t&>(label));

    if (temp.control_m == NULL)
        initialize(temp, temp_parent);

    measure(temp, result);

    place_data_t p;
    p.horizontal().length_m = result.width();
    measure_vertical(temp, result, p);
}

/****************************************************************************************************/

std::string get_control_string(const label_t& widget)
{
    return implementation::get_control_string(widget.control_m);
}

/****************************************************************************************************/

platform_display_type get_display(const label_t& widget)
{
    return widget.control_m; 
}

/****************************************************************************************************/

// REVISIT: MM--we need to replace the display_t mechanism with concepts/any_*/
//              container idiom for event and drawing system.

template <>
platform_display_type insert<label_t>(display_t& display,
                                             platform_display_type& parent,
                                             label_t& element)
{
	platform_display_type parent_hwnd(parent);

    initialize(element, parent_hwnd);

    return display.insert(parent, get_display(element));
}


/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
