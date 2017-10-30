#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/templates/label.hpp>
#include <adobe/future/widgets/headers/popup_common_fwd.hpp>
#include <adobe/algorithm/find.hpp>

#include <vector>

namespace adobe {
namespace implementation {

template <typename Backend>
struct popup : widget_base<Backend>
{
    using menu_item_t     = std::pair<std::string, any_regular_t>;
    using menu_item_set_t = std::vector<menu_item_t>;

    using model_type           = any_regular_t;
    using setter_type          = popup_setter_type;
    using extended_setter_type = popup_extended_setter_type;

    popup(const std::string& name, const std::string& alt_text, const std::string& custom_item_name, const menu_item_t* first, const menu_item_t* last, theme_t theme)
        : name_m{name, alt_text, 0, theme}
        , alt_text_m{alt_text}
        , custom_item_name_m{custom_item_name}
        , menu_items_m{first, last}
    {
    }

    void reset_menu_item_set(const menu_item_set_t& menu_item_set)
    {
        Backend::reset_popup(this->control_m, menu_item_set);
    }

    void select_with_text(const std::string& text)
    {
        Backend::select_popup_string(this->control_m, text);
    }

    void display(const any_regular_t& item)
    {
        auto iter = find_if(this->menu_items_m, [&](const menu_item_t& i) { return i.second == item; });
        if (iter != end(this->menu_items_m))
        {
            assert(!this->is_null());
            typename Backend::debounce lock{this->control_m, this->connection_m};
            Backend::select_popup_index(this->control_m, distance(begin(this->menu_items_m), iter));
        }
    }

    void monitor(const setter_type& proc)
    {
        value_proc_m = proc;
    }

    void monitor_extended(const extended_setter_type& proc)
    {
        extended_value_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    label<Backend>       name_m;
    std::string          alt_text_m;
    std::string          custom_item_name_m;
    bool                 using_label_m;
    setter_type          value_proc_m;
    extended_setter_type extended_value_proc_m;
    menu_item_set_t      menu_items_m;
#endif
};

/****************************************************************************************************/

template <typename Backend>
void on_popup_selection_changed(popup<Backend>& element,
                                long            new_index,
                                modifiers_t     modifier_state)
{
    if (element.custom_m)
        --new_index;

    if (element.value_proc_m)
        element.value_proc_m(element.menu_items_m.at(new_index).second);

    if (element.extended_value_proc_m)
        element.extended_value_proc_m(element.menu_items_m.at(new_index).second, modifier_state);
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, popup<Backend>& element)
{
    Backend::pre(element);

    if (element.using_label_m)
    {
        do_insert(parent, element.name_m);
    }

    element.control_m = Backend::make_popup(
        parent,
        element.alt_text_m,
        element.menu_items_m,
        element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
