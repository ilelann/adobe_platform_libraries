#pragma once

namespace adobe {

    struct edit_number_t;

    namespace implementation
    {
        template<typename Backend>
        struct edit_number_data {
            explicit edit_number_data(edit_number_t *edit_number)
            {}

            void initialize(){}
        };
    }
}
