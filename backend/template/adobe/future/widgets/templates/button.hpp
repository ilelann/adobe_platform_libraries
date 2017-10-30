#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/button_helper.hpp>

namespace adobe {
namespace implementation {

template <typename Backend>
struct button : widget_base<Backend>
{
    using model_type = any_regular_t;

    button(bool                             is_default,
           bool                             is_cancel,
           modifiers_t                      modifier_mask,
           const button_state_descriptor_t* first,
           const button_state_descriptor_t* last,
           theme_t)
        : state_set_m{first, last}
        , modifier_mask_m{modifier_mask}
        , is_default_m{is_default}
        , is_cancel_m{is_cancel}
        , enabled_m{true}
    {
    }

    void set(modifiers_t modifiers, const model_type& value)
    {
        auto state = current_state(modifiers);

        if (state->value_m != value)
            state->value_m = value;
    }

    void set_contributing(modifiers_t modifiers, const dictionary_t& value)
    {
        current_state(modifiers)->contributing_m = value;
    }

    any_regular_t underlying_handler()
    {
        return any_regular_t{this->control_m};
    }

    bool handle_key(typename Backend::key_type key, bool pressed, modifiers_t modifiers)
    {
        if (pressed == false)
            return false;

        auto state = current_state(modifiers);

        Backend::set_button_label(this->control_m, state->name_m);

        Backend::set_control_alt_text(this->control_m, state->alt_text_m);

        if (state->hit_proc_m.empty() || this->enabled_m == false)
            return false;

        if (Backend::is_return(key) && this->is_default_m) // return
        {
            state->hit_proc_m(state->value_m, state->contributing_m);
        }
        else if (Backend::is_escape(key) && this->is_cancel_m) // escape
        {
            state->hit_proc_m(state->value_m, state->contributing_m);
        }
        else
        {
            return false;
        }

        return true;
    }

    void enable(bool make_enabled)
    {
        enabled_m = make_enabled;
        if (!this->is_null())
        {
            widget_base<Backend>::enable(make_enabled);
        }
    }

#ifndef ADOBE_NO_DOCUMENTATION
    button_state_set_t state_set_m;
    modifiers_t        modifier_mask_m;
    bool               is_default_m;
    bool               is_cancel_m;
    bool               enabled_m;

private:
    button_state_set_t::iterator current_state(modifiers_t modifiers)
    {
        auto state = button_modifier_state(state_set_m, modifier_mask_m, modifiers);

        return (state == state_set_m.end()) ? button_default_state(state_set_m) : state;
    }
#endif
};

template <typename Backend>
void on_button_clicked(const button<Backend>& element, const modifiers_t& modifiers)
{
    auto state = adobe::button_modifier_state(
        element.state_set_m, element.modifier_mask_m, modifiers);

    if (state == element.state_set_m.end())
        state = adobe::button_default_state(element.state_set_m);

    if (state->hit_proc_m)
        state->hit_proc_m(state->value_m, state->contributing_m);
}

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, button<Backend>& element)
{
    Backend::pre(element);

    auto state = button_default_state(element.state_set_m);

    element.control_m = Backend::make_button(parent,
                                             state->name_m,
                                             element.is_default_m,
                                             state->alt_text_m,
                                             element.enabled_m,
                                             element);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
