/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/algorithm.hpp>
#include <adobe/algorithm/find_match.hpp>

#include <adobe/future/widgets/headers/platform_tab_group.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>
#include <adobe/future/widgets/headers/display.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

tab_group_t::tab_group_t(const tab_t* first, const tab_t* last, theme_t theme)
: control_m(NULL), theme_m(theme), value_proc_m(), items_m(first, last)
{
}

/****************************************************************************************************/

void tab_group_t::on_tab_changed (int index)
{
	if (!value_proc_m.empty())
		value_proc_m(items_m[index].value_m);
}

/****************************************************************************************************/

void tab_group_t::measure(extents_t& result)
{
    assert(control_m);

    // REVISIT (fbrereto) : A lot of static metrics values added here

    for (tab_set_t::iterator first(items_m.begin()), last(items_m.end()); first != last; ++first)
    {
        extents_t attrs;
        measure_label_text(label_t(first->name_m, std::string(), 0, theme_m), attrs, get_parent_control(control_m));

        result.width() += attrs.width() + 18;
        result.height() = (std::max)(result.height(), attrs.height());
    }

    result.vertical().frame_m.first = result.height() + 7;

    result.height() = 5;
}

/****************************************************************************************************/

void tab_group_t::place(const place_data_t& place_data)
{
    implementation::set_control_bounds(control_m, place_data);
}

/****************************************************************************************************/

void tab_group_t::display(const any_regular_t& new_value)
{
    assert(control_m);

    tab_set_t::iterator iter(find_match(items_m, new_value, compare_members(&tab_t::value_m, std::equal_to<any_regular_t>())));

    if (iter == items_m.end()) return;

    implementation::select_tab(control_m, iter - items_m.begin());
}

/****************************************************************************************************/

void tab_group_t::monitor(const tab_group_value_proc_t& proc)
{
    assert(control_m);

    if (!value_proc_m)
        value_proc_m = proc;
}

/****************************************************************************************************/

template <>
platform_display_type insert<tab_group_t>(display_t&             display,
                                                platform_display_type&  parent,
                                                tab_group_t&     element)
{
    assert(is_null_control(element.control_m));

    element.control_m = implementation::make_tab_group(parent, element);

    set_font_tabitem(element.control_m);

    implementation::setup_callback_tab_group(element.control_m);

    return display.insert(parent, element.control_m);
}

/****************************************************************************************************/

} //namespace adobe
