#pragma once

namespace adobe {

    struct edit_number_t;
    struct place_data_t;
    struct extents_t;

    namespace implementation {
        template<typename Backend>
        struct widget_base {

            widget_base()
                    : control_m{} { }

            widget_base(const widget_base&) = delete;
            widget_base(widget_base&&) = delete;
            widget_base&operator=(const widget_base&) = delete;
            widget_base&operator=(widget_base&&) = delete;

            typename Backend::widget_handle control_m;

            void measure(extents_t &result) { }

            void place(const place_data_t &place_data) { }

            void enable(bool make_enabled) { Backend::enable(control_m, make_enabled); }

            void measure_vertical(extents_t &, const place_data_t &) { }

            void set_visible(bool visible) {
                Backend::visible(control_m, visible);
            }

        protected:
            bool is_null() { return Backend::is_null(control_m); }
            bool enabled() { return Backend::enabled(control_m); }

            void nullify() { Backend::nullify(control_m); }
        };
    }

}
