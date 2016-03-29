/*
    Copyright 2015 Ivan Le Lann
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <adobe/future/platform_number_formatter_data.hpp>

#include <adobe/future/number_formatter.hpp>

/**************************************************************************************************/

namespace adobe {

/**************************************************************************************************/

void number_formatter_t::set_format(const std::string& format){}

/**************************************************************************************************/

std::string number_formatter_t::get_format() const { return std::string{};}

/**************************************************************************************************/

template <>
std::string number_formatter_t::format<any_regular_t>(const any_regular_t& x)
{
    if (x.type_info() == typeid(double)) {
        return std::to_string(x.cast<double>());
    }
    else return std::string{"formatter_format_number error"};
}

/**************************************************************************************************/

template <>
any_regular_t number_formatter_t::parse<any_regular_t>(const std::string& str, any_regular_t dummy)
{
    if (dummy.type_info() == typeid(double))
    {
        double d {};
        std::istringstream iss{str};
        iss >> d;
        return any_regular_t {d};
    }
    else return any_regular_t(std::string("formatter_format_number error"));
}

/**************************************************************************************************/

void number_formatter_t::monitor_locale(const dictionary_t& locale_data) {}

/**************************************************************************************************/

} // namespace adobe

/**************************************************************************************************/
