/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/*************************************************************************************************/

#ifndef ADOBE_FUTURE_LOCALE_HPP
#define ADOBE_FUTURE_LOCALE_HPP

/*************************************************************************************************/

#include <adobe/config.hpp>

#include <adobe/dictionary_fwd.hpp>
#include <adobe/name.hpp>

#include <boost/function.hpp>
#include <boost/signals2.hpp>

#include <adobe/future/platform_locale_data.hpp>

/*************************************************************************************************/

namespace adobe {

/*************************************************************************************************/

typedef boost::function<void (const dictionary_t& locale_data)> monitor_locale_proc_t;

boost::signals2::connection monitor_locale(const monitor_locale_proc_t& proc);

const dictionary_t& current_locale();

/*************************************************************************************************/

extern static_name_t key_locale_identifier;
extern static_name_t key_locale_decimal_point;
extern static_name_t key_locale_thousands_separator;

/*************************************************************************************************/

namespace implementation {

/*************************************************************************************************/

void signal_locale_change(const dictionary_t& new_locale_data);

/*************************************************************************************************/

} // namespace implementation

/*************************************************************************************************/

} // namespace adobe

/*************************************************************************************************/

#endif

/*************************************************************************************************/
