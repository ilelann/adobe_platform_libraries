#pragma once

#include <adobe/future/widgets/headers/edit_number.hpp>

namespace adobe {

    struct edit_number_t;

    namespace implementation
    {
        template<typename Backend>
        struct edit_number_data {
            explicit edit_number_data(edit_number_t *edit_number)
            {}
            //  :
            //     control_m(edit_number),
            //     prev_capture_m(0),
            //     tracking_m(false)
            // {
            //     last_point_m.x = 0;
            //     last_point_m.y = 0;
            // }

            ~edit_number_data(){}

            void initialize(){}

            // LRESULT label_message(UINT message, WPARAM wParam, LPARAM lParam);

            // edit_number_t* control_m;
            // HWND           prev_capture_m;
            // bool           tracking_m;
            // POINTS         last_point_m;
        };
    }
}