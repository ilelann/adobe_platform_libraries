#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/edit_text_common.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct edit_text : widget_base<Backend>
{
    using model_type                = std::string;
    using edit_text_pre_edit_proc_t = std::function<void(const std::string&, bool&)>;
    using setter_type               = std::function<void(const model_type&)>;

    edit_text(const edit_text_ctor_block_t& block)
        : name_m{block.name_m, block.alt_text_m, 0, block.theme_m}
        , alt_text_m{block.alt_text_m}
        , rows_m{block.num_lines_m}
        , cols_m{block.min_characters_m}
        , max_cols_m{block.max_characters_m}
        , scrollable_m{block.scrollable_m}
        , password_m{block.password_m}
    {
    }

    // controller
    void monitor(setter_type proc)
    {
        post_edit_proc_m = proc;
    }

    // view
    void display(const model_type& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_edit_text_text(this->control_m, value);
    }

    void signal_pre_edit(edit_text_pre_edit_proc_t proc)
    {
        assert(!this->is_null());

        if (!this->pre_edit_proc_m)
            this->pre_edit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    label<Backend>            name_m;
    std::string               alt_text_m;
    long                      characters_m;
    long                      rows_m;
    long                      cols_m;
    long                      max_cols_m;
    bool                      scrollable_m;
    bool                      password_m;
    edit_text_pre_edit_proc_t pre_edit_proc_m;
    setter_type               post_edit_proc_m;
#endif
};

template <typename Backend>
bool is_focused(edit_text<Backend>& element)
{
    return Backend::is_focused(element.control_m);
}

template <typename Backend>
std::string get_field_text(edit_text<Backend>& element)
{
    return Backend::get_edit_text_text(element.control_m);
}

template <typename Backend>
void on_text_changed(const edit_text<Backend>& element, const std::string& value)
{
    if (!element.post_edit_proc_m)
        return;

    element.post_edit_proc_m(value);
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, edit_text<Backend>& element)
{
    Backend::pre(element);

    if (!element.name_m.name_m.empty())
    {
        do_insert(parent, element.name_m);
    }

    element.control_m = Backend::make_edit_text(parent,
                                                element.rows_m,
                                                element.scrollable_m,
                                                element.password_m,
                                                element.alt_text_m,
                                                element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
