#pragma once

#include <adobe/future/widgets/templates/edit_number_data.hpp>

namespace adobe
{
    namespace implementation {
        struct gtk;
    }

    struct edit_number_platform_data_t
            : implementation::edit_number_data<implementation::gtk> {
        using base = implementation::edit_number_data<implementation::gtk>;
        using base::base;
    };
}
