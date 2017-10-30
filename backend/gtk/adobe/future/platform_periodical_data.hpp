/*
    Copyright 2016 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#ifndef ADOBE_PERIODICAL_IMPL_HPP
#define ADOBE_PERIODICAL_IMPL_HPP

/****************************************************************************************************/

#include <functional>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

struct periodical_platform_data_t
{
    using fire_proc_t = std::function<void()>;

    periodical_platform_data_t(const fire_proc_t& fire_proc,
                               std::size_t        millisecond_delay);

    ~periodical_platform_data_t();

private:
    fire_proc_t fire_m;
};

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

// ADOBE_PERIODICAL_IMPL_HPP
#endif

/****************************************************************************************************/
