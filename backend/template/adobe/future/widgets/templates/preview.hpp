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

    typedef boost::function<void(name_t action, const any_regular_t &)> button_notifier_t;

    namespace implementation {

        template<typename Backend>
        struct preview : widget_base<Backend> {
            typedef dictionary_t controller_model_type;
            typedef boost::gil::rgba8_image_t view_model_type;
            typedef std::function<void(const controller_model_type &)> setter_proc_type;

            preview(const std::string &alt_text, theme_t theme)
                    : alt_text_m{alt_text}, sublayout_m{theme}, theme_m{theme} { }


            void measure(extents_t &result) {
                sublayout_m.measure(result);
            }

            void place(const place_data_t &place_data) {
                sublayout_m.place(place_data);
            }

            void display(const view_model_type &value) {
                sublayout_m.sublayout_sheet_set_update("image"_name, value);
            }

            void monitor(const setter_proc_type &) {
            }

            widget_node_t
            evaluate(const std::string &sheet_description,
                     const std::string &layout_description,
                     const dictionary_t &parameters,
                     const widget_node_t &parent,
                     const factory_token_t &token,
                     const widget_factory_t &factory,
                     const button_notifier_t &notifier,
                     behavior_t &behavior) {
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
            //view_model_type            preview_image_m;
            std::string alt_text_m;
            sublayout_t sublayout_m;
            theme_t theme_m;
#endif
        };

        template<typename Backend>
        typename Backend::widget_handle insert(display_t &display,
                                               typename Backend::widget_handle &parent,
                                               preview<Backend> &element) {
            return parent;
        }
    }

    template<typename Backend>
    struct view_model_type<implementation::preview<Backend> > {
        typedef typename implementation::preview<Backend>::view_model_type type;
    };

}
