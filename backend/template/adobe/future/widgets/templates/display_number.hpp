#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/number_unit.hpp>
#include <adobe/future/number_formatter.hpp>
#include <adobe/widget_attributes.hpp>

namespace adobe {
namespace implementation {

inline std::string set_field_text(std::string                       label,
                                  double                            value,
                                  const std::vector<adobe::unit_t>& unit_set,
                                  std::wstring::size_type*          label_chars = 0)
{
    std::stringstream result;

    // this is called by a subview when its value changes.
    // we are only concerned with changing our actual value when
    // the view that changed is also the current view.

    std::string               suffix;
    adobe::number_formatter_t number_formatter;

    if (!unit_set.empty())
    {
        std::vector<adobe::unit_t>::const_iterator i(unit_set.begin());
        std::vector<adobe::unit_t>::const_iterator end(unit_set.end());

        while (i != end && i->scale_m_m <= value)
            ++i;

        if (i != unit_set.begin())
            --i;

        value = adobe::to_base_value(value, *i);

        suffix = i->name_m;

        number_formatter.set_format(i->format_m);
    }

    if (!label.empty())
    {
        result << label << " ";

        if (label_chars)
            *label_chars = label.length();
    }

    result << number_formatter.format(value);

    if (!suffix.empty())
        result << " " << suffix;

    return result.str();
}

template <typename Backend>
struct display_number : widget_base<Backend>
{
    using model_type = double;
    using unit_set_t = std::vector<unit_t>;

    template <class ForwardIterator>
    display_number(
        const std::string& name,
        const std::string& alt_text,
        ForwardIterator    first,
        ForwardIterator    last,
        theme_t,
        int)
        : name_m{name}
        , alt_text_m{alt_text}
        , unit_set_m{first, last}
    {
    }

    void display(const model_type& value)
    {
        assert(!this->is_null());
        typename Backend::debounce lock{this->control_m, this->connection_m};
        Backend::set_label_text(this->control_m, set_field_text(this->name_m, value, this->unit_set_m));
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string name_m;
    std::string alt_text_m;
    unit_set_t  unit_set_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle do_insert(typename Backend::widget_handle& parent, display_number<Backend>& element)
{
    Backend::pre(element);

    element.control_m = Backend::make_display_number(parent, element.alt_text_m);

    return Backend::post(parent, element.control_m);
}
} // namespace implementation
} // namespace adobe
