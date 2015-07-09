/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#include <adobe/future/platform_locale_data.hpp>

#include <adobe/future/locale.hpp>
#include <adobe/name.hpp>
#include <adobe/string.hpp>

/****************************************************************************************************/

namespace {

/****************************************************************************************************/
#if ADOBE_PLATFORM_WIN
std::string get_locale_tidbit(LCTYPE type)
{
    auto current_locale = LOCALE_NAME_USER_DEFAULT;

    // first find out how much buffer space we'll need
    auto length = ::GetLocaleInfoEx(current_locale, type, NULL, 0);

    std::string buffer;
    buffer.reserve(static_cast<std::size_t>(length));

    // now do the actual information grab
    if (::GetLocaleInfoEx(current_locale, type, &buffer[0], length) == 0)
        throw std::runtime_error("GetLocaleInfoEx");

    return buffer;
}

#endif
/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace implementation {

/****************************************************************************************************/

void do_locale_check()
{
#if ADOBE_PLATFORM_WIN
    typedef std::numpunct<char> numpunct_type;

    static std::string old_locale_ident;

    std::string new_locale_ident(get_locale_tidbit(LOCALE_SENGLANGUAGE) + "_" + get_locale_tidbit(LOCALE_SENGCOUNTRY));

    if (old_locale_ident == new_locale_ident)
        return;

    old_locale_ident = new_locale_ident;

    // get the important stuff to push to the dictionary

    std::locale          locale(new_locale_ident.c_str());
    const numpunct_type& numpunct(std::use_facet<numpunct_type>(locale));
    char                 sep(numpunct.thousands_sep());
    std::string          thousands_separator(&sep, 1);
    char                 decimal(numpunct.decimal_point());
    std::string          decimal_point(&decimal, 1);

    // finally push the important stuff into the dictionary

    dictionary_t new_locale_data;

	new_locale_data.insert(std::make_pair(key_locale_identifier,          new_locale_ident));
    new_locale_data.insert(std::make_pair(key_locale_decimal_point,       decimal_point));
    new_locale_data.insert(std::make_pair(key_locale_thousands_separator, thousands_separator));

    adobe::implementation::signal_locale_change(new_locale_data);
#endif
}

/****************************************************************************************************/

} // namespace implementation

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/
