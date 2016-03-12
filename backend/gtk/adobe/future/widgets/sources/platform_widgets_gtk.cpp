/*
    Copyright 2015 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_widgets_gtk.hpp>

#include <adobe/future/widgets/templates/button.hpp>
#include <adobe/future/widgets/templates/checkbox.hpp>
#include <adobe/future/widgets/templates/display_number.hpp>
#include <adobe/future/widgets/templates/edit_text.hpp>
#include <adobe/future/widgets/templates/group.hpp>

#ifndef ADOBE_PLATFORM_NO_IMAGE
#include <adobe/future/widgets/templates/image.hpp>
#endif

#include <adobe/future/widgets/templates/label.hpp>


#ifndef ADOBE_PLATFORM_NO_LINK
#include <adobe/future/widgets/templates/link.hpp>
#endif

#include <adobe/future/widgets/templates/panel.hpp>
#include <adobe/future/widgets/templates/popup.hpp>

#ifndef ADOBE_PLATFORM_NO_PRESETS
#include <adobe/future/widgets/templates/presets.hpp>
#endif

#include <adobe/future/widgets/templates/progress_bar.hpp>
#include <adobe/future/widgets/templates/radio_button.hpp>
#include <adobe/future/widgets/templates/reveal.hpp>
#include <adobe/future/widgets/templates/separator.hpp>
#include <adobe/future/widgets/templates/slider.hpp>
#include <adobe/future/widgets/templates/tab_group.hpp>
#include <adobe/future/widgets/templates/toggle.hpp>
#include <adobe/future/widgets/templates/window.hpp>

#include <adobe/future/resources.hpp>

#include <gtk/gtk.h>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

    bool completely_valid_number_string_given_current_locale(const std::string &value) {
        // TODO ilelann
        return false;
    }

    namespace implementation {

        namespace {
            GtkWidget * create_box()
            {
                auto handle = gtk_flow_box_new();
                gtk_flow_box_set_homogeneous (GTK_FLOW_BOX(handle), true);
                return handle;
            }
        }

        platform_display_type get_top_level_window(platform_display_type display_element) {
            return gtk_widget_get_toplevel(GTK_WIDGET(display_element));
        }


        bool
        gtk::is_return(key_type k) {
            return k == GDK_KEY_Return;
        }

        bool
        gtk::is_escape(key_type k) {
            return k == GDK_KEY_Escape;
        }

        bool
        gtk::is_focused(widget_handle widget) {
            return gtk_widget_is_focus(GTK_WIDGET(widget));
        }

        bool
        gtk::is_null(widget_handle widget) {
            return !widget;
        }

        void
        gtk::nullify(widget_handle &widget) {
            widget = nullptr;
        }

        void gtk::destroy_window(widget_handle widget) {
            gtk_widget_destroy(GTK_WIDGET(widget));
        }

        void
        gtk::enable(widget_handle widget, bool enabled) {
            assert(!is_null(widget) && "null widget");
            return gtk_widget_set_sensitive(GTK_WIDGET(widget), enabled);
        }

        void
        gtk::visible(widget_handle widget, bool visible) {
            (GTK_IS_WINDOW(widget) && visible) ?
            gtk_widget_show_all(widget) :
            gtk_widget_set_visible(widget, visible);
        }

        void
        gtk::set_control_alt_text(widget_handle widget, const std::string &text) {
            gtk_widget_set_tooltip_text(GTK_WIDGET(widget), text.c_str());
        }

        void
        gtk::set_button_label(widget_handle widget, const std::string &label) {
            gtk_button_set_label(GTK_BUTTON(widget), label.c_str());
        }

        void
        gtk::set_label_text(widget_handle widget, const std::string &label) {
            gtk_label_set_text(GTK_LABEL(widget), label.c_str());
        }

        /****************************************************************************************************/

        template <typename Target>
        Target& as (gpointer user_data)
        {
            return *(static_cast<Target*>(user_data));
        };

        /****************************************************************************************************/

        static void gtk_button_callback(GtkButton *, gpointer user_data) {
            // TODO ilelann
            // get modifiers state here
            on_button_clicked(as<button<gtk>>(user_data), modifiers_none_s);
        }

        gtk::widget_handle
        gtk::make_button(widget_handle,
                         const std::string &label,
                         bool is_default,
                         const std::string &alt_label,
                         bool enabled,
                         button<gtk> &button) {
            auto handle = gtk_button_new_with_label(label.c_str());
            set_control_alt_text(handle, alt_label);
            enable(handle, enabled);
            g_signal_connect(G_OBJECT(handle), "clicked", G_CALLBACK(gtk_button_callback), &button);
            return handle;
        }

        /****************************************************************************************************/

        static void gtk_checkbox_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
            on_checkbox_toggled(as<checkbox<gtk>>(user_data));
        }

        gtk::widget_handle
        gtk::make_checkbox(widget_handle,
                           const std::string &label,
                           const std::string &alt_text,
                           checkbox<gtk> &checkbox) {
            auto handle = gtk_check_button_new_with_label(label.c_str());
            set_control_alt_text(handle, alt_text);
            g_signal_connect(G_OBJECT(handle), "clicked", G_CALLBACK(gtk_checkbox_toggled), &checkbox);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_display_number(widget_handle parent, const std::string &alt_text) {
            auto handle = gtk_entry_new();
            set_control_alt_text(handle, alt_text);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_edit_text(widget_handle parent,
                            long rows,
                            bool scrollable,
                            bool password,
                            const std::string &alt_text) {
            widget_handle handle;
            if (password) {
                handle = gtk_entry_new();
                gtk_entry_set_visibility(GTK_ENTRY(handle), false);
            }
            else {
                handle = gtk_text_view_new();
            }
            set_control_alt_text(handle, alt_text);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_group(widget_handle parent,
                        const std::string &label) {
            auto handle = gtk_frame_new(label.c_str());
            auto box = create_box();
            gtk_container_add(GTK_CONTAINER(handle), GTK_WIDGET(box));
            return handle;
        }

        /****************************************************************************************************/

        static void delete_image_resource(GdkPixbuf *pixbuf) {
                g_object_unref(G_OBJECT(pixbuf));
            }

        using image_resource_deleter = void (*)(gtk::image_resource);

        gtk::image_resource
        gtk::make_image_resource(const char *path) {
            static std::unique_ptr<GdkPixbuf, image_resource_deleter> res{
                    gdk_pixbuf_new_from_file(path, nullptr),
                    delete_image_resource};
            return res.get();
        }

        gtk::image_resource
        gtk::image_resource_from_name(const char *name) {
            return make_image_resource(find_resource(name).c_str());
        }

        gtk::widget_handle
        gtk::make_image(widget_handle, image_resource img) {
            return gtk_image_new_from_pixbuf(img);
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_label(widget_handle,
                        const std::string &label,
                        const std::string &alt_text) {
            auto handle = gtk_label_new(label.c_str());
            set_control_alt_text(handle, alt_text.c_str());
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_panel(widget_handle parent) {
            auto frame = gtk_frame_new(NULL);
            auto box = create_box();
            gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
            return frame;
        }

        /****************************************************************************************************/

        static
        GtkListStore *
        make_popup_store(const gtk::menu_item_set_t &menu_items) {
            auto store = GTK_LIST_STORE(gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING));

            auto nitems = menu_items.size();
            for (std::size_t i = 0; i < nitems; ++i) {
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, i, menu_items[i].first.c_str(), -1);
            }
            return store;
        }

        gtk::widget_handle
        gtk::make_popup(widget_handle parent,
                        const std::string &alt_text,
                        const menu_item_set_t &menu_items,
                        popup<gtk> &element) {
            auto handle = gtk_combo_box_new_with_model(GTK_TREE_MODEL(make_popup_store(menu_items)));
            set_control_alt_text(handle, alt_text);
            return handle;
        }

        /****************************************************************************************************/

#ifndef ADOBE_PLATFORM_NO_PRESETS
        gtk::widget_handle
        gtk::make_presets_image(widget_handle parent, image_resource img, const std::string &alt_text,
                                presets<gtk> &element) {
            auto handle = make_image(parent, img);
            g_signal_connect(G_OBJECT(handle), "clicked", G_CALLBACK(gtk_presets_clicked), &element);
            return handle;
        }
#endif

        gtk::widget_handle
        gtk::make_progress_bar(widget_handle parent, bool is_vertical, pb_style_t) {
            auto handle = gtk_progress_bar_new();
            gtk_orientable_set_orientation(
                    GTK_ORIENTABLE(handle),
                    is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL);
            return handle;
        }

        /****************************************************************************************************/

        static void gtk_radio_button_toggled(GtkToggleButton *, gpointer user_data) {
            on_radio_button_toggled(as<radio_button<gtk>>(user_data));
        }

        gtk::widget_handle
        gtk::make_radio_button(widget_handle,
                               const std::string &text,
                               const std::string &alt_text,
                               radio_button<gtk> &element) {
            auto handle = gtk_radio_button_new_with_label(nullptr, text.c_str());
            set_control_alt_text(handle, alt_text);
            g_signal_connect(G_OBJECT(handle), "toggled", G_CALLBACK(gtk_radio_button_toggled), &element);
            return handle;
        }

        /****************************************************************************************************/

        static void gtk_reveal_clicked(GtkImage *, gpointer user_data) {
            on_reveal_clicked(as<reveal<gtk>>(user_data));
        }

        gtk::widget_handle
        gtk::make_reveal(widget_handle parent, image_resource img, const std::string &al_text, reveal<gtk> &element) {
            auto handle = make_image(parent, img);
            set_control_alt_text(handle, al_text);
            g_signal_connect(G_OBJECT(handle), "clicked", G_CALLBACK(gtk_reveal_clicked), &element);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_separator(widget_handle, bool vertical) {
            return gtk_separator_new(vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL);
        }

        /****************************************************************************************************/

        static void gtk_slider_moved(GtkRange * range, gpointer user_data) {
            on_slider_moved(as<slider<gtk>>(user_data), static_cast<long>(gtk_range_get_value (range)));
        }

        gtk::widget_handle gtk::make_slider(
                widget_handle,
                const std::string &alt_text,
                bool is_vertical,
                std::size_t,
                slider_style_t,
                std::size_t format_size,
                slider<gtk> &element) {
            assert(format_size != 0 && "zero format size in make_slider");

            auto handle = gtk_scale_new_with_range(
                    is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL,
                    0, static_cast<double>(format_size), 1);
            g_signal_connect(G_OBJECT(handle), "value-changed", G_CALLBACK(gtk_slider_moved), &element);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_tab_group(widget_handle parent, const std::vector<std::string> &names,
                            tab_group<gtk> &) {
            auto handle = gtk_notebook_new();
            for (const auto &name : names) {
                gtk_notebook_append_page(GTK_NOTEBOOK(handle), gtk_label_new(name.c_str()), gtk_label_new(name.c_str()));
            }
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_toggle(widget_handle, const std::string &alt_text) {
            auto handle = gtk_toggle_button_new();
            set_control_alt_text(handle, alt_text);
            return handle;
        }

        /****************************************************************************************************/

        gtk::widget_handle
        gtk::make_window(widget_handle parent, const std::string &label, const bool &has_size_box) {
            auto handle = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_title(GTK_WINDOW(handle), label.c_str());
            gtk_window_set_resizable(GTK_WINDOW(handle), has_size_box);
            auto box = create_box();
            gtk_container_add(GTK_CONTAINER(handle), GTK_WIDGET(box));
            return handle;
        }

        /****************************************************************************************************/

        void gtk::set_checkbox(widget_handle handle, bool active, bool inactive) {
            if (active) {
                gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(handle), false);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(handle), true);
            }
            else if (inactive) {
                gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(handle), false);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(handle), false);
            }
            else
                gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(handle), true);
        }

        /****************************************************************************************************/

        void
        gtk::set_edit_text_text(widget_handle handle, const std::string &text) {
            if (GTK_IS_ENTRY(handle)) {
                gtk_entry_set_text(GTK_ENTRY(handle), text.c_str());
            }
            else {
                auto buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(handle));
                gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), text.c_str(), text.size());
            }
        }

        std::string
        gtk::get_edit_text_text(widget_handle handle) {
            if (GTK_IS_ENTRY(handle)) {
                return gtk_entry_get_text(GTK_ENTRY(handle));
            }
            else {
                auto buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(handle));
                GtkTextIter begin, end;
                gtk_text_buffer_get_start_iter(buffer, &begin);
                gtk_text_buffer_get_end_iter(buffer, &end);

                auto temp = std::unique_ptr<gchar, void(*)(gpointer)>{
                        gtk_text_buffer_get_text(buffer, &begin, &end, TRUE),
                        g_free};
                return std::string {temp.get()};
            }
        }

        /****************************************************************************************************/

        void
        gtk::reset_popup(widget_handle handle, const menu_item_set_t &items) {
            gtk_combo_box_set_model(GTK_COMBO_BOX(handle), GTK_TREE_MODEL(make_popup_store(items)));
        }

        void
        gtk::select_popup_string(widget_handle handle,
                                 const std::string &text) {
            gtk_combo_box_set_active_id(GTK_COMBO_BOX(handle), text.c_str());
        }

        void
        gtk::select_popup_index(widget_handle handle, int index) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(handle), index);
        }

        /****************************************************************************************************/

        void gtk::progress_bar_set_value(widget_handle progress_bar, std::size_t value, std::size_t size) {
            assert(size != 0 && "zero size in progress_bar_set_value");
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), (static_cast<double>(value)) / size);
        }

        /****************************************************************************************************/

        void gtk::set_slider_position(widget_handle handle, std::size_t value) {
            gtk_range_set_value(GTK_RANGE(handle), static_cast<double>(value));
        }

        /****************************************************************************************************/

        void gtk::set_tab_group_position(widget_handle handle, int position) {
            gtk_notebook_set_current_page(GTK_NOTEBOOK(handle), position);
        }

        /****************************************************************************************************/

        void
        gtk::set_image(widget_handle handle, image_resource img) {
            gtk_image_set_from_pixbuf(GTK_IMAGE(handle), img);
        }

        /****************************************************************************************************/

        void
        gtk::toggle_button(widget_handle handle, bool toggled) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(handle), toggled);
        }

        /****************************************************************************************************/

        void
        gtk::activate_radio_button(widget_handle widget, bool activated) {
            // GtkRadioButton is a GtkToggleButton
            toggle_button(widget, activated);
        }

        /****************************************************************************************************/
    }

/****************************************************************************************************/

#define ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(w)\
template <> platform_display_type insert<w>(display_t& display, platform_display_type& parent, w& element)\
{\
    return do_insert(display, parent, element);\
}\
\

/****************************************************************************************************/

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(button_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(checkbox_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(display_number_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(edit_text_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(group_t)

#ifndef ADOBE_PLATFORM_NO_IMAGE
    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(image_t)
#endif

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(label_t)

#ifndef ADOBE_PLATFORM_NO_LINK
    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(link_t)
#endif

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(panel_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(popup_t)

#ifndef ADOBE_PLATFORM_NO_PRESETS
    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(presets_t)
#endif

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(progress_bar_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(radio_button_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(reveal_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(separator_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(slider_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(tab_group_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(toggle_t)

    ADOBE_PLATFORM_WIDGET_GTK_ROUTE_INSERT(window_t)

} // namespace adobe

/****************************************************************************************************/
