/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include "express_viewer.hpp"
#include "report_exception.hpp"
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>

#include <adobe/xstring.hpp>

#include <boost/filesystem/convenience.hpp>

#include <adobe/future/behavior.hpp>

#include <gtk/gtk.h>

#include <sstream>
#include <ostream>
#include <string>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

enum Command
{
    ADOBE_ABOUT = 40001,
    ADOBE_QUIT,
    ADOBE_REFRESH_VIEW,
    ADOBE_REFRESH_SHEET,
    ADOBE_NORMAL_DIALOG_SIZE,
    ADOBE_SMALL_DIALOG_SIZE,
    ADOBE_MINI_DIALOG_SIZE,
    ADOBE_SHOW_WIDGET_FRAMES,
    ADOBE_CLEAR_WIDGET_FRAMES,
    ADOBE_SERIALIZE_WIDGETS,
    ADOBE_RUN_MODAL,

    ADOBE_LOCALIZATION_ENUS,
    ADOBE_LOCALIZATION_DEDE,
    ADOBE_LOCALIZATION_JAJP,
    ADOBE_LOCALIZATION_KOKR,
    ADOBE_LOCALIZATION_PGPG
};

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

template <Command command>
void process_command(adobe::application_t * app)
{
        switch(command)
        {
            case ADOBE_ABOUT:
                gtk_message_dialog_new (nullptr, 
                    GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, 
                "Adobe Begin Copyright 2005-2007 Adobe Systems Incorporated");
                break;

            case ADOBE_QUIT:
                gtk_main_quit();
                break;

            case ADOBE_REFRESH_VIEW:
                app->display_window();
                break;

            case ADOBE_REFRESH_SHEET:
                app->load_sheet();
                break;

            case ADOBE_NORMAL_DIALOG_SIZE:
                app->set_dialog_size( size_normal_s );
                app->display_window();
                break;

            case ADOBE_SMALL_DIALOG_SIZE:
                app->set_dialog_size( size_small_s );
                app->display_window();
                break;

            case ADOBE_MINI_DIALOG_SIZE:
                app->set_dialog_size( size_mini_s );
                app->display_window();
                break;

#ifndef NDEBUG
            case ADOBE_SHOW_WIDGET_FRAMES:
                app->frame_window();
                break;

            case ADOBE_CLEAR_WIDGET_FRAMES:
                app->clear_window_frames();
                break;
#else
            case ADOBE_SHOW_WIDGET_FRAMES:
            case ADOBE_CLEAR_WIDGET_FRAMES:
                system_beep();
                break;
#endif

            case ADOBE_SERIALIZE_WIDGETS:
#if 0
                app->serialize_connections();
#endif
                break;

            case ADOBE_RUN_MODAL:
                app->run_current_as_modal();
                break;

            case ADOBE_LOCALIZATION_ENUS:
                adobe::implementation::top_frame().attribute_set_m.insert(
                    std::make_pair(adobe::static_token_range("lang"), adobe::static_token_range("en_US")));
                app->display_window();
                break;

            case ADOBE_LOCALIZATION_DEDE:
                adobe::implementation::top_frame().attribute_set_m.insert(
                    std::make_pair(adobe::static_token_range("lang"), adobe::static_token_range("de_DE")));
                app->display_window();
                break;

            case ADOBE_LOCALIZATION_JAJP:
                adobe::implementation::top_frame().attribute_set_m.insert(
                    std::make_pair(adobe::static_token_range("lang"), adobe::static_token_range("ja_JP")));
                app->display_window();
                break;

            case ADOBE_LOCALIZATION_KOKR:
                adobe::implementation::top_frame().attribute_set_m.insert(
                    std::make_pair(adobe::static_token_range("lang"), adobe::static_token_range("ko_KR")));
                app->display_window();
                break;

            case ADOBE_LOCALIZATION_PGPG:
                adobe::implementation::top_frame().attribute_set_m.insert(
                    std::make_pair(adobe::static_token_range("lang"), adobe::static_token_range("pg_PG")));
                app->display_window();
                break;
        }
    }

template <Command command>
void process_command_ptr(GtkMenuItem *, gpointer user_data)
{
    process_command<command>(static_cast<adobe::application_t*>(user_data));
}
/****************************************************************************************************/

template <Command command>
void append_menu_item (GtkWidget* menu, const char * label, application_t * app)
{
    auto item = gtk_menu_item_new_with_label(label ? label : "-");
    g_signal_connect (item, "activate", G_CALLBACK (process_command_ptr<command>), app);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
};

void append_sub_menu (GtkWidget* menu, GtkWidget* submenu, const char * label)
{
    auto item = gtk_menu_item_new_with_label(label ? label : "-");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
};

/****************************************************************************************************/

void setup_main_window( application_t* app )
{
    auto window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Adobe Begin");

    auto vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // setup the menu system
    auto menu_bar = gtk_menu_bar_new();
    auto app_menu = gtk_menu_new();
    auto view_options_menu = gtk_menu_new();
    auto localization_menu = gtk_menu_new();

    append_menu_item<ADOBE_ABOUT>(app_menu, "About Adobe Begin", app);
    append_menu_item<ADOBE_QUIT>(app_menu, "Quit Adobe Begin", app);

    append_menu_item<ADOBE_REFRESH_VIEW>(view_options_menu, "Refresh View", app);
    append_menu_item<ADOBE_REFRESH_SHEET>(view_options_menu, "Refresh Sheet", app);
    append_menu_item<ADOBE_NORMAL_DIALOG_SIZE>(view_options_menu, "Normal Dialog Size", app);
    append_menu_item<ADOBE_SMALL_DIALOG_SIZE>(view_options_menu, "Small Dialog Size", app);
    append_menu_item<ADOBE_MINI_DIALOG_SIZE>(view_options_menu, "Mini Dialog Size", app);
    append_menu_item<ADOBE_SHOW_WIDGET_FRAMES>(view_options_menu, "Show Widget Frames", app);
    append_menu_item<ADOBE_CLEAR_WIDGET_FRAMES>(view_options_menu, "Clear Widget Frames", app);
    append_menu_item<ADOBE_SERIALIZE_WIDGETS>(view_options_menu, "Serialize Widgets", app);
    append_menu_item<ADOBE_RUN_MODAL>(view_options_menu, "Run Modal", app);

    append_menu_item<ADOBE_LOCALIZATION_ENUS>(localization_menu, "Standard English (en-us)", app);
    append_menu_item<ADOBE_LOCALIZATION_DEDE>(localization_menu, "German (de-de)", app);
    append_menu_item<ADOBE_LOCALIZATION_JAJP>(localization_menu, "Japanese (ja-jp)", app);
    append_menu_item<ADOBE_LOCALIZATION_KOKR>(localization_menu, "Korean (ko-kr)", app);
    append_menu_item<ADOBE_LOCALIZATION_PGPG>(localization_menu, "Pig Latin (pg-pg)", app);

    append_sub_menu(menu_bar, app_menu, "Adobe Begin");
    append_sub_menu(menu_bar, view_options_menu, "View Options");
    append_sub_menu(menu_bar, localization_menu, "Localization");

    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 3);

    // set root in display
    adobe::get_main_display().set_root(window);
    gtk_widget_show_all(window);
}

/****************************************************************************************************/

bool os_initialize( application_t* app )
{
    app->set_resource_directory("/home/ivan/src/adobe/adobe_platform_libraries/test/begin/resources");
    setup_main_window( app );
    return true;
}

/****************************************************************************************************/

void os_mainloop(adobe::application_t& )
{
    while(gtk_main_iteration())
    {
        adobe::general_deferred_proc_queue()();
    }
}

/****************************************************************************************************/

void os_end_mainloop()
{
    gtk_main_quit();
}

/****************************************************************************************************/

}

/****************************************************************************************************/

int main (int argc, char **argv)
{
    try
    {
        gtk_init (&argc, &argv);
        g_log_set_always_fatal(G_LOG_LEVEL_CRITICAL);
        g_log_set_always_fatal(G_LOG_LEVEL_WARNING);
        auto app = adobe::application_t::getInstance();
        if( app )
        {
            app->run();
        }
    }
    catch( ... )
    {
        adobe::report_exception();
    }

    return 0;
} 

