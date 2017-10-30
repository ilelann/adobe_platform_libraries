/*
    Copyright 2016 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/platform_periodical_data.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

periodical_platform_data_t::periodical_platform_data_t(
    const fire_proc_t& fire_proc,
    std::size_t        millisecond_delay)
    : fire_m(fire_proc)
{
    // TODO ilelann
}

/****************************************************************************************************/

periodical_platform_data_t::~periodical_platform_data_t()
{
    // TODO ilelann
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
