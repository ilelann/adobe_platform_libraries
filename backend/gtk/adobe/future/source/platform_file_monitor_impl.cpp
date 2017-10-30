/*
    Copyright 2016 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/file_monitor.hpp>

#include <boost/filesystem/operations.hpp>

#include <gio/gio.h>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace {
void process_change(GFileMonitor* monitor, GFile* file, GFile* other_file, GFileMonitorEvent event_type, gpointer user_data)
{
    auto owner      = static_cast<file_monitor_platform_data_t*>(user_data);
    auto last_write = boost::filesystem::last_write_time(g_file_get_path(file));
    if (owner->last_write_m != last_write)
    {
        owner->last_write_m = last_write;
        owner->proc_m(owner->path_m, adobe::file_monitor_contents_changed_k);
    }
}
} // namespace

/****************************************************************************************************/

file_monitor_platform_data_t::file_monitor_platform_data_t()
    : last_write_m{0}
{
}

file_monitor_platform_data_t::file_monitor_platform_data_t(
    const file_monitor_path_type&  path,
    const file_monitor_callback_t& proc)
    : path_m{path}
    , proc_m{proc}
    , last_write_m{0}
{
    connect();
}

file_monitor_platform_data_t::file_monitor_platform_data_t(
    const file_monitor_platform_data_t& rhs)
    : file_monitor_platform_data_t{rhs.path_m, rhs.proc_m}
{
}

file_monitor_platform_data_t::~file_monitor_platform_data_t() { disconnect(); }

file_monitor_platform_data_t& file_monitor_platform_data_t::
operator=(const file_monitor_platform_data_t& rhs)
{
    disconnect();

    path_m = rhs.path_m;
    proc_m = rhs.proc_m;

    connect();

    return *this;
}

void file_monitor_platform_data_t::set_path(
    const file_monitor_path_type& path)
{
    if (!boost::filesystem::exists(path) || path_m == path)
        return;

    disconnect();

    path_m = path;

    connect();
}

void file_monitor_platform_data_t::connect()
{
    if (!boost::filesystem::exists(path_m))
        return;

    last_write_m = boost::filesystem::last_write_time(path_m);

    change_handle_m.monitor_m         = g_file_monitor_file(g_file_new_for_path(path_m.native().c_str()),
                                                    G_FILE_MONITOR_NONE, NULL, NULL);
    change_handle_m.signal_handler_id = g_signal_connect(
        change_handle_m.monitor_m, "changed", G_CALLBACK(process_change), this);
}

void file_monitor_platform_data_t::disconnect()
{
    if (change_handle_m.monitor_m == 0 || last_write_m == 0)
        return;

    if (change_handle_m.monitor_m)
    {
        if (change_handle_m.signal_handler_id)
        {
            g_signal_handler_disconnect(change_handle_m.monitor_m,
                                        change_handle_m.signal_handler_id);
            change_handle_m.signal_handler_id = 0;
        }
        g_object_unref(change_handle_m.monitor_m);
        change_handle_m.monitor_m = nullptr;
    }
}

/****************************************************************************************************/

file_monitor_t::file_monitor_t() {}

file_monitor_t::file_monitor_t(const file_monitor_path_type&  path,
                               const file_monitor_callback_t& proc)
    : plat_m{path, proc}
{
}

void file_monitor_t::set_path(const file_monitor_path_type& path)
{
    plat_m.set_path(path);
}

void file_monitor_t::monitor(const file_monitor_callback_t& proc)
{
    plat_m.proc_m = proc;
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
