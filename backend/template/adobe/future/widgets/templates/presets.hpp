#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/platform_popup.hpp>

#include <adobe/array.hpp>
#include <adobe/dictionary.hpp>

#include <functional>

namespace adobe {

typedef std::function<dictionary_t()> model_snapshot_proc_t;

namespace implementation {

template <typename Backend>
struct presets : widget_base<Backend> {
    typedef dictionary_t model_type;
    typedef typename popup<Backend>::setter_type setter_type;

    /*!
    \param name             Preset file name (xyz becomes xyz.preset)
    \param domain           Domain of the widget (usually the name of the application)
    \param alt_text         Additional help text for the widget when the user pauses over it
    \param bind_set         Array of name/string pairs that area values which will be saved/restored by presets
    \param localization_set Set of named arguments that contain the basic string keys for
                            localization of widget-private resources (e.g., the keys used
                            in the add/delete subdialogs to the preset widget).
    \param theme            Theme for the widget

    \note
        If the presets is set to a value other than <code>true_value</code> or
        <code>false_value</code>, the presets gets a 'dash' (undefined) state.
*/
    presets(const std::string& name,
            const std::string& domain,
            const std::string& alt_text,
            const array_t& bind_set,
            const dictionary_t& localization_set,
            theme_t theme);
/*
    localization_set_m(localization_set),
            category_popup_m(implementation::localization_value(localization_set,
                             key_preset_category_popup_name, "Category:"),
    implementation::localization_value(localization_set,
            key_preset_category_popup_alt_text,
    "Select a category of presets for this dialog"),
    implementation::localization_value(localization_set,
            key_preset_custom_category_name, "Custom"),
    0, 0, theme),
    popup_m(implementation::localization_value(localization_set,
                                               key_preset_preset_popup_name, "Preset:"),
    implementation::localization_value(localization_set,
            key_preset_preset_popup_alt_text,
    "Select a preset for settings in this dialog"),
    implementation::localization_value(localization_set,
            key_preset_custom_category_name, "Custom"),
    0, 0, theme),
    theme_m(theme),
            bind_set_m(bind_set),
            name_m(name),
            domain_m(domain),
            alt_text_m(alt_text),
            selected_m(false),
    type_2_debounce_m(false),
    custom_m(false)
*/

    /*!
    \param result The ideal extent values for this widget given its current attributes
*/
    //void measure(extents_t& result);

    /*!
    \param place_data Position information to apply to the widget
*/
    //void place(const place_data_t& place_data);

    /*!
    \param value Preset settings to which we need to be set
*/
    void display(const model_type& value);

    /*!
    \param value Set of additional preset data to be added to the preset listings
*/
    void display_additional_preset_set(const array_t& value);

    /*!
    \param proc  Procedure called to imbue the model with preset data
*/
    void monitor(const setter_type& proc)
    {
        proc_m = proc;
    }

    /*!
    \param proc  Procedure called when a model snapshot is needed
*/
    void snapshot_callback(const model_snapshot_proc_t& proc)
    {
        snapshot_proc_m = proc;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    dictionary_t localization_set_m;
    popup<Backend> category_popup_m; // the category popup
    popup<Backend> popup_m; // the actual preset popup
    theme_t theme_m;
    array_t bind_set_m; // the set of parameters to be stored in the preset
    std::string name_m;
    std::string domain_m;
    std::string alt_text_m;
    bool selected_m;
    long popup_height_m;
    model_snapshot_proc_t snapshot_proc_m;
    dictionary_t last_m;
    bool type_2_debounce_m;
    setter_type proc_m;
    bool custom_m;

    // these store the various preset sources as arrays
    array_t dynamic_preset_set_m;
    array_t default_preset_set_m;
    array_t user_preset_set_m;
    array_t composite_m;

    using widget_base<Backend>::control_m;
#endif

    void do_imbue(const typename popup<Backend>::model_type& value)
    {
        if (this->type_2_debounce_m)
            return;

        if (this->proc_m)
            this->proc_m(value);
    }
};


    template <typename Backend>
    typename Backend::image_resource presets_bitmap_normal()
    {
        static auto res = Backend::image_resource_from_name("preset_button_u_n.tga");
        return res;
    }

    template <typename Backend>
    typename Backend::image_resource presets_bitmap_clicked()
    {
        static auto res = Backend::image_resource_from_name("preset_button_d_n.tga");
        return res;
    }

    template <typename Backend>
    typename Backend::image_resource presets_bitmap_disabled()
    {
        static auto res = Backend::image_resource_from_name("preset_button_u_d.tga");
        return res;
    }

    template <typename Backend>
    void enable(presets<Backend>& element, bool make_enabled)
    {
        Backend::set_image(element.control_m,
                           make_enabled ? presets_bitmap_normal<Backend>() : presets_bitmap_disabled<Backend>());
    }

    template <typename Backend>
    void on_presets_clicked(presets<Backend>& element);

    template <typename Backend>
    typename Backend::widget_handle do_insert(display_t& display,
                                           typename Backend::widget_handle& parent,
                                           presets<Backend>& element)
    {
        Backend::pre(element);

        insert(display, parent, element.category_popup_m);

        insert(display, parent, element.popup_m);
        element.popup_m.monitor([&](const any_regular_t& v) {element.do_imbue(v); });

        element.control_m = Backend::make_presets_image(parent, presets_bitmap_normal<Backend>(), element.alt_text_m, element);

        return Backend::post(display, parent, element);
    }
}}
