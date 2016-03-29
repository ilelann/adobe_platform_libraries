#pragma once

#include <adobe/future/widgets/templates/platform_widgets.hpp>

#include <adobe/any_regular_fwd.hpp>
#include <adobe/widget_attributes.hpp>
#include <adobe/future/widgets/headers/slider_helper.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/pb_style.hpp>

#include <boost/gil/typedefs.hpp>

#include <string>
#include <vector>
#include <utility>

/****************************************************************************************************/
// Forward declarations

typedef unsigned int    guint;

struct _GtkWidget;
typedef struct _GtkWidget              GtkWidget;

struct _GdkPixbuf;
typedef struct _GdkPixbuf GdkPixbuf;

/****************************************************************************************************/

namespace adobe {

struct display_t;

namespace implementation {

    struct gtk {

        /****************************************************************************************************/

        using widget_handle = GtkWidget*;
        using image_resource = GdkPixbuf*;

        /****************************************************************************************************/

        using key_type = guint;

        static bool is_return(key_type k);
        static bool is_escape(key_type k);

        /****************************************************************************************************/

        static widget_handle make_button(widget_handle parent,
                                         const std::string& label,
                                         bool is_default,
                                         const std::string& alt_label,
                                         bool enabled,
                                         button<gtk> & button);

        static widget_handle make_checkbox(widget_handle parent,
                                           const std::string& label,
                                           const std::string& alt_text,
                                           checkbox<gtk> & checkbox);

        static widget_handle make_display_number(widget_handle parent, const std::string& alt_text);

        static widget_handle make_edit_text(widget_handle parent,
                                            long rows,
                                            bool scrollable,
                                            bool password,
                                            const std::string& alt_text);

        static widget_handle make_group(widget_handle parent,
                                        const std::string& label);

        static widget_handle make_image(widget_handle parent, image_resource img);

//        static widget_handle make_image(widget_handle parent,
//                                        image_resource img,
//                                        const std::string& alt_text);

#ifndef ADOBE_PLATFORM_NO_PRESETS
        static widget_handle make_presets_image(widget_handle parent,
                                                image_resource img,
                                                const std::string& alt_text,
                                                presets<gtk>& element);
#endif
        static widget_handle make_label(widget_handle parent,
                                        const std::string& label,
                                        const std::string& alt_text);

        static widget_handle make_panel(widget_handle parent);

        typedef std::pair<std::string, any_regular_t> menu_item_t;
        typedef std::vector<menu_item_t> menu_item_set_t;

        static widget_handle make_popup(widget_handle parent,
                                        const std::string& alt_text,
                                        const menu_item_set_t& menu_items,
                                        popup<gtk>& element);

        static widget_handle make_progress_bar(widget_handle parent, bool is_vertical, pb_style_t style);

        static widget_handle make_radio_button(widget_handle parent,
                                               const std::string& text,
                                               const std::string& alt_text,
                                               radio_button<gtk> &element);

        static widget_handle make_reveal(widget_handle parent,
                                         image_resource img,
                                         const std::string & al_text,
                                         reveal<gtk> & element);

        static widget_handle make_separator(widget_handle parent, bool vertical);

        static widget_handle make_slider(widget_handle parent,
                                         const std::string& alt_text,
                                         bool is_vertical,
                                         std::size_t num_ticks,
                                         slider_style_t style,
                                         std::size_t format_size,
                                         slider<gtk> & element);

        static widget_handle make_tab_group(widget_handle parent,
                                            const std::vector<std::string> & names,
                                            tab_group<gtk>&);

        static widget_handle make_toggle(widget_handle parent, const std::string& alt_text);

        static widget_handle make_window(widget_handle parent,
                                         const std::string& label,
                                         const bool& has_size_box);

        /****************************************************************************************************/

        static bool is_null(widget_handle widget);
        static void nullify(widget_handle& widget);
        static void nullify_when_destroyed(widget_handle* element);

        static void destroy_window (widget_handle widget);

        static bool is_focused(widget_handle widget);

        static std::string get_edit_text_text(widget_handle widget);
        static void set_edit_text_text(widget_handle widget, const std::string & text);

        static void enable (widget_handle widget, bool enabled);

        static void visible (widget_handle widget, bool visible);

        static void set_control_alt_text(widget_handle widget, const std::string & text);

        /****************************************************************************************************/

        static void set_button_label(widget_handle widget, const std::string & label);

        /****************************************************************************************************/

        static void toggle_button(widget_handle widget, bool toggled);

        /****************************************************************************************************/

        static void activate_radio_button(widget_handle widget, bool activated);

        /****************************************************************************************************/

        static void set_label_text(widget_handle widget, const std::string & label);

        /****************************************************************************************************/

        static void set_checkbox(widget_handle cb, bool active, bool inactive);

        /****************************************************************************************************/

        static void progress_bar_set_value(widget_handle progress_bar, std::size_t value, std::size_t size);

        /****************************************************************************************************/

        static void select_popup_string(widget_handle control,
                                        const std::string& text);

        static void select_popup_index(widget_handle control, int index);

        static void reset_popup(widget_handle control, const menu_item_set_t & items);

        /****************************************************************************************************/

        static void set_slider_position(widget_handle slider, std::size_t value);

        /****************************************************************************************************/

        static void set_tab_group_position(widget_handle bar, int position);

        /****************************************************************************************************/

        static image_resource image_resource_from_name(const char* name);

        static image_resource make_image_resource(const char* path);

        static void set_image(widget_handle control, image_resource img);

        /****************************************************************************************************/

        template <typename T>
        static void pre(T& element)
        {
            assert(is_null(element.control_m));
        }

        template <typename T>
        static widget_handle post(display_t& display, widget_handle& parent, T& element)
        {
            assert(!is_null(element.control_m));
            return display.insert(parent, element.control_m);
        }
    };
}

    using button_t = implementation::button<implementation::gtk>;
    using checkbox_t = implementation::checkbox<implementation::gtk>;
    using display_number_t = implementation::display_number<implementation::gtk>;
    using edit_text_t = implementation::edit_text<implementation::gtk>;
    using group_t = implementation::group<implementation::gtk>;
    using image_t = implementation::image<implementation::gtk>;
    using link_t = implementation::link<implementation::gtk>;
    using label_t = implementation::label<implementation::gtk>;
    using panel_t = implementation::panel<implementation::gtk>;
    using popup_t = implementation::popup<implementation::gtk>;
    using presets_t = implementation::presets<implementation::gtk>;
    using preview_t = implementation::preview<implementation::gtk>;
    using progress_bar_t = implementation::progress_bar<implementation::gtk>;
    using radio_button_t = implementation::radio_button<implementation::gtk>;
    using reveal_t = implementation::reveal<implementation::gtk>;
    using separator_t = implementation::separator<implementation::gtk>;
    using slider_t = implementation::slider<implementation::gtk>;
    using tab_group_t = implementation::tab_group<implementation::gtk>;
    using toggle_t = implementation::toggle<implementation::gtk>;
    using window_t = implementation::window<implementation::gtk>;
}
