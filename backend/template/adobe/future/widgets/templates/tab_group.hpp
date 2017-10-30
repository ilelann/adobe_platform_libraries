#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <functional>
#include <string>
#include <vector>
#include <iterator>

namespace adobe {
namespace implementation {

template <typename Backend>
struct tab_group : widget_base<Backend>
{
    using model_type             = any_regular_t;
    using tab_group_value_proc_t = std::function<void(const any_regular_t&)>;

    struct tab_t
    {
        std::string   name_m;
        any_regular_t value_m;
    };

    using tab_set_t = std::vector<tab_t>;

    tab_group(const tab_t* first, const tab_t* last, theme_t)
        : items_m{first, last}
    {
    }

    void display(const any_regular_t& value)
    {
        assert(!this->is_null());

        auto iter = find_if(
            begin(this->items_m),
            end(this->items_m),
            [&value](const tab_t& t) { return t.value_m == value; });

        if (iter == end(this->items_m))
            return;

        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_tab_group_position(this->control_m, distance(begin(this->items_m), iter));
    }

    void monitor(const tab_group_value_proc_t& proc)
    {
        this->value_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    tab_set_t              items_m;
    tab_group_value_proc_t value_proc_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, tab_group<Backend>& element)
{
    Backend::pre(element);

    std::vector<std::string> names;
    names.reserve(element.items_m.size());
    for (const auto& item : element.items_m)
    {
        names.push_back(item.name_m);
    }

    element.control_m = Backend::make_tab_group(parent, names, element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
