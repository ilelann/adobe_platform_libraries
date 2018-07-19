/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/*************************************************************************************************/

#ifndef ADOBE_FUTURE_PERIODICAL_HPP
#define ADOBE_FUTURE_PERIODICAL_HPP

/*************************************************************************************************/

#include <adobe/config.hpp>

#include <adobe/future/platform_periodical_data.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <cassert>

/*************************************************************************************************/

namespace adobe {

/*************************************************************************************************/

class periodical_t
{
public:
    typedef boost::function<void ()> periodical_proc_t;

    explicit periodical_t(const periodical_proc_t& proc = periodical_proc_t(),
                          std::size_t              millisecond_delay = 200) :
        is_blocked_m(false),
        proc_m(proc),
        data_m(boost::bind(&periodical_t::fire, boost::ref(*this)), millisecond_delay)
    {
    }

    ~periodical_t()
    { /* the necessary stuff will auto-destruct */ }

    inline void monitor(const periodical_proc_t& proc)
    { proc_m = proc; }

    inline void fire() const
    {
        if (proc_m.empty() == false && is_blocked_m == false)
            proc_m();
    }

    bool is_blocked_m; // if true, the proc will not fire when the time comes

private:
    periodical_proc_t          proc_m;
    periodical_platform_data_t data_m;
};

/*************************************************************************************************/

} // namespace adobe

/*************************************************************************************************/

#endif

/*************************************************************************************************/
