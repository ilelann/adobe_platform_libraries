#pragma once

#include <adobe/future/widgets/templates/widget_base.hpp>
#include <adobe/future/widgets/headers/display.hpp>

#include <adobe/future/widgets/headers/factory.hpp>
#include <adobe/future/widgets/headers/sublayout.hpp>

#include <adobe/controller_concept.hpp>
#include <adobe/view_concept.hpp>

#include <boost/function.hpp>
#include <boost/gil/typedefs.hpp>

namespace adobe {

using button_notifier_t = boost::function<void(name_t action, const any_regular_t&)>;

namespace implementation {

template <typename Backend>
struct preview : widget_base<Backend>
{
    using controller_model_type = dictionary_t;
    using view_model_type       = boost::gil::rgba8_image_t;
    using setter_proc_type      = std::function<void(const controller_model_type&)>;

    preview(const std::string& alt_text, theme_t theme)
        : alt_text_m{alt_text}
        , sublayout_m{theme}
    {
    }

    void measure(extents_t& result)
    {
        sublayout_m.measure(result);
    }

    void place(const place_data_t& place_data)
    {
        sublayout_m.place(place_data);
    }

    void display(const view_model_type& value)
    {
        sublayout_m.sublayout_sheet_set_update("image"_name, value);
    }

    void monitor(const setter_proc_type&)
    {
    }

    widget_node_t
    evaluate(const std::string&       sheet_description,
             const std::string&       layout_description,
             const dictionary_t&      parameters,
             const widget_node_t&     parent,
             const factory_token_t&   token,
             const widget_factory_t&  factory,
             const button_notifier_t& notifier,
             behavior_t&              behavior)
    {
        auto result = sublayout_m.evaluate(sheet_description,
                                           layout_description,
                                           parameters,
                                           parent,
                                           token,
                                           factory,
                                           notifier,
                                           behavior);

        sublayout_m.sublayout_sheet_update();

        return result;
    }

#ifndef ADOBE_NO_DOCUMENTATION
    std::string alt_text_m;
    sublayout_t sublayout_m;
#endif
};

template <typename Backend>
typename Backend::widget_handle insert(display_t&,
                                       typename Backend::widget_handle& parent,
                                       preview<Backend>&)
{
    return parent;
}
} // namespace implementation

template <typename Backend>
struct view_model_type<implementation::preview<Backend>>
{
    using type = typename implementation::preview<Backend>::view_model_type;
};
} // namespace adobe
