/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/platform_periodical_data.hpp>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

gboolean periodical_timer_callback (gpointer user_data)
{
    auto impl = static_cast<adobe::periodical_platform_data_t*>(user_data);

    assert(impl->fire_m);

    impl->fire_m();

    return false;
}

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

periodical_platform_data_t::periodical_platform_data_t(const fire_proc_t& fire_proc,
                                                       std::size_t        millisecond_delay) :
    fire_m(fire_proc)
{

    timer_ref_m = g_timeout_add (millisecond_delay, periodical_timer_callback, this);
}

/****************************************************************************************************/

periodical_platform_data_t::~periodical_platform_data_t()
{
    g_source_remove(timer_ref_m);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
