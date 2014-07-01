/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/*************************************************************************************************/

#include <adobe/future/widgets/headers/platform_edit_text.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/placeable_concept.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/widgets/headers/platform_widget_utils.hpp>
#include <adobe/future/windows_cast.hpp>
#include <adobe/controller_concept.hpp>

const int gap=4;

/*************************************************************************************************/

namespace {

/*************************************************************************************************/

void fix_newlines(std::string& string)
{
    // check for cases where \n is not preceeded by \r

    std::string::size_type result(0);

    while (true)
    {
        result = string.find("\n", result);

        if (result == std::string::npos)
            break;

        if (result == 0 || string[result - 1] != '\r')
            string.insert(result, "\r");

        ++result;
    }
}

/*************************************************************************************************/

} // namespace

/*************************************************************************************************/

namespace adobe {

/*************************************************************************************************/

void edit_text_t::display(const model_type& value) // values that come in from Adam
{
    std::string string(value);

    if (string != value_m)
    {
        fix_newlines(string);

        value_m = string;

        set_field_text(string);
    }
}

/****************************************************************************************************/

edit_text_t::edit_text_t(const edit_text_ctor_block_t& block) : 
    name_m(block.name_m, block.alt_text_m, 0, block.theme_m),
    alt_text_m(block.alt_text_m),
    field_text_m(),
    using_label_m(!block.name_m.empty()),
    rows_m(block.num_lines_m),
    cols_m(block.min_characters_m),
    max_cols_m(block.max_characters_m),
    scrollable_m(block.scrollable_m),
    password_m(block.password_m),
    edit_baseline_m(0),
    edit_height_m(0),
    static_baseline_m(0),
    static_height_m(0),
    type_2_debounce_m(false)
{ 
}

void edit_text_t::measure(extents_t& result)
{
    assert(control_m);
    //
    // The calculate_edit_bounds function can figure out the size this edit box
    // should be, based on the number of rows and columns.
    //
    result = calculate_edit_bounds(control_m, cols_m, rows_m);
    //
    // Store the height and baseline so that we can correctly align the edit widget
    // in set_bounds.
    //
    edit_height_m = result.height();
    if (!result.vertical().guide_set_m.empty())
        edit_baseline_m = result.vertical().guide_set_m[0];
    //
    // If we have a label then we need to make extra space
    // for it.
    //
    if (!using_label_m) return;
    extents_t label_bounds;
    measure_label_text(get_label(), label_bounds, get_parent_control(control_m));
    //
    // Make sure that the height can accomodate both the label
    // and the edit widget.
    //
    align_slices(result.vertical(), label_bounds.vertical());
    //
    // We put the label on the left side of the edit box, and
    // place a point of interest at the end of the label, so
    // that colon alignment can be performed.
    //

    result.width() += gap + label_bounds.width();
    result.horizontal().guide_set_m.push_back(label_bounds.width());
    //
    // We use the height and baseline of the label to size and
    // align it in set_bounds.
    //
    static_height_m = label_bounds.height();
    static_baseline_m = label_bounds.vertical().guide_set_m[0];
}

/****************************************************************************************************/

void edit_text_t::place(const place_data_t& place_data)
{
    using adobe::place;

    assert(control_m);

    place_data_t local_place_data(place_data);

    long baseline = local_place_data.vertical().guide_set_m[0];

    if (using_label_m)
    {
        //
        // We're using a label. We need to extract the label width from the
        // points of interest in the given geometry, and make sure that we
        // correctly translate the label for baseline alignment.
        //
        place_data_t    label_place_data;
        label_place_data.horizontal().position_m = left(local_place_data);
        label_place_data.vertical().position_m = top(local_place_data);

        //
        // We stored the height of the label in best_bounds. The width of
        // the label can be discovered via the first horizontal point of
        // interest.
        //
        height(label_place_data) = static_height_m;
        width(label_place_data) = local_place_data.horizontal().guide_set_m[0];
        //
        // Translate the label vertically for baseline alignment with the
        // edit widget. We stored the label's baseline in best_bounds.
        //
        label_place_data.vertical().position_m += baseline - static_baseline_m;
        place(get_label(), label_place_data);

        local_place_data.horizontal().position_m += width(label_place_data) + gap;
        width(local_place_data) -= width(label_place_data) + gap;
    }
    //
    // We might need to translate the edit widget vertically, for baseline
    // alignment.
    //
    local_place_data.vertical().position_m += baseline - edit_baseline_m;

    // REVISIT (thw) : Do we need to adapt the height for baseline alignment?

    implementation::set_control_bounds(control_m, local_place_data);
}

/****************************************************************************************************/

label_t& edit_text_t::get_label()
{
    return name_m;
}

/****************************************************************************************************/

void edit_text_t::enable(bool active)
{
    using adobe::enable;

    assert(control_m);

    set_control_enabled (control_m, active);

    if (using_label_m)
        enable(get_label(), active);
}

/****************************************************************************************************/

void edit_text_t::set_theme(theme_t theme)
{
    theme_m = theme;

    set_font_edittext(control_m);

//    if (using_label_m)
//        get_label() = label_t(get_control_string(get_label()), alt_text_m, 0, theme_m);
}


/****************************************************************************************************/

void edit_text_t::set_field_text(const std::string& text)
{
    if (type_2_debounce_m)
        return;

    assert(control_m);

    // REVISIT (fbrereto) : Currently we require the line endings to be whatever the platform wants.
    //                      I'm not sure we want to make it the burden of the widget to handle all
    //                      forms of line endings.

    type_2_debounce_m = true;

    implementation::set_edit_text_text(control_m, text);

    type_2_debounce_m = false;
}

/****************************************************************************************************/

void edit_text_t::set_selection(long , long )
{
    assert(control_m);
}

/****************************************************************************************************/

void edit_text_t::signal_pre_edit(edit_text_pre_edit_proc_t proc)
{
    assert(control_m);

    if (!pre_edit_proc_m)
        pre_edit_proc_m = proc;
}

/****************************************************************************************************/

void edit_text_t::monitor(setter_type proc)
{
    if (!post_edit_proc_m)
        post_edit_proc_m = proc;
}

/****************************************************************************************************/

template <>
platform_display_type insert<edit_text_t>(display_t&             display,
                                                 platform_display_type& parent,
                                                 edit_text_t&    element)
{
    assert(is_null_control(element.control_m));

    if (element.using_label_m)
    {
        insert(display, parent, element.get_label());
    }

    element.control_m = implementation::make_edit_text(parent, element.rows_m, element.scrollable_m, element.password_m);

    element.set_theme(element.theme_m);

    implementation::setup_callback_edit_text(element);

    if (!element.alt_text_m.empty())
        implementation::set_control_alt_text(element.control_m, element.alt_text_m);

   return display.insert(parent, get_display(element));
}

/****************************************************************************************************/

} // namespace adobe

/*************************************************************************************************/
