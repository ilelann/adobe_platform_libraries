#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/widget_attributes.hpp>

#include <functional>
#include <string>
#include <vector>

namespace adobe { namespace implementation {

template <typename Backend>
struct tab_group : widget_base<Backend> {
    typedef any_regular_t model_type;
    typedef std::function<void(const any_regular_t&)> tab_group_value_proc_t;

    struct tab_t {
        std::string name_m;
        any_regular_t value_m;
    };

    typedef std::vector<tab_t> tab_set_t;

    tab_group(const tab_t* first, const tab_t* last, theme_t theme)
            : items_m{first, last}, theme_m{theme}
    {}

    void display(const any_regular_t& new_value)
    {
        assert(!this->is_null());

        auto iter = std::find_if(
                std::begin(this->items_m),
                std::end(this->items_m),
                [&new_value](const tab_t & t){ return t.value_m == new_value;});

        if (iter == this->items_m.end()) return;

        Backend::set_tab_group_position(this->control_m, std::distance(this->items_m.begin(), iter));
    }

    void monitor(const tab_group_value_proc_t& proc)
    {
        this->value_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    tab_set_t items_m;
    theme_t theme_m;
    tab_group_value_proc_t value_proc_m;
#endif
};


        template <typename Backend>
        typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, tab_group<Backend>& element)
        {
            Backend::pre(element);

            auto names = std::vector<std::string>{};
            names.reserve(element.items_m.size());
            for (const auto& item : element.items_m) {
                names.push_back(item.name_m);
            }

            element.control_m = Backend::make_tab_group(parent, names, element);

            return Backend::post(display, parent, element);
        }
}}
