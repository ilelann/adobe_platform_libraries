#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/edit_text_common.hpp>
#include <adobe/future/widgets/headers/platform_label.hpp>

namespace adobe { namespace implementation {

template <typename Backend>
struct edit_text : widget_base<Backend> {
    typedef std::string model_type;
    typedef std::function<void(const std::string&, bool&)>
        edit_text_pre_edit_proc_t;
    typedef std::function<void(const model_type&)> setter_type;

    edit_text(const edit_text_ctor_block_t& block) :
            name_m{block.name_m, block.alt_text_m, 0, block.theme_m},
            alt_text_m{block.alt_text_m},
            field_text_m{},
            using_label_m{!block.name_m.empty()},
            rows_m{block.num_lines_m},
            cols_m{block.min_characters_m},
            max_cols_m{block.max_characters_m},
            scrollable_m{block.scrollable_m},
            password_m{block.password_m},
            edit_baseline_m{0},
            edit_height_m{0},
            static_baseline_m{0},
            static_height_m{0},
            type_2_debounce_m{false}
    {
    }

    label<Backend>& get_label()
    {
        return name_m;
    }

    // controller
    void monitor(setter_type proc)
    {
        if (!post_edit_proc_m)
            post_edit_proc_m = proc;
    }

    // view
    void display(const model_type& value)
    {
        if (value != value_m)
        {
            value_m = value;
            Backend::set_edit_text_text(this->control_m, value_m);
        }
    }

    void signal_pre_edit(edit_text_pre_edit_proc_t proc)
    {
        assert(!this->is_null());

        if (!this->pre_edit_proc_m)
            this->pre_edit_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    theme_t theme_m;
    label<Backend> name_m;
    std::string alt_text_m;
    std::wstring field_text_m;
    bool using_label_m;
    long characters_m;
    long rows_m;
    long cols_m;
    long max_cols_m;
    bool scrollable_m;
    bool password_m;
    long edit_baseline_m;
    long edit_height_m;
    long static_baseline_m;
    long static_height_m;
    edit_text_pre_edit_proc_t pre_edit_proc_m;
    setter_type post_edit_proc_m;
    //edit_text_label_hit_proc_t label_hit_proc_m;
    bool type_2_debounce_m;
    std::string value_m; // Used to debounce
#endif
};

template <typename Backend>
bool is_focused(edit_text<Backend> & element)
{
    return Backend::is_focused(element.control_m);
}

template <typename Backend>
std::string get_field_text(edit_text<Backend> & element)
{
    return Backend::get_edit_text_text(element.control_m);
}

template <typename Backend>
typename Backend::widget_handle do_insert(display_t& display, typename Backend::widget_handle& parent, edit_text<Backend>& element)
        {
            Backend::pre(element);

            if (element.using_label_m) {
                insert(display, parent, element.get_label());
            }

            element.control_m = Backend::make_edit_text(parent,
                                                        element.rows_m,
                                                        element.scrollable_m,
                                                        element.password_m,
                                                        element.alt_text_m);

            return Backend::post(display, parent, element);
        }

}}
