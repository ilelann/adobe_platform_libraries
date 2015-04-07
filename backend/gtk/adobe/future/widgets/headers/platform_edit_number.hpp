#pragma once

#include <adobe/future/widgets/templates/edit_number_data.hpp>
#include <adobe/future/widgets/headers/platform_widgets_gtk.hpp>

namespace adobe
{
    struct edit_number_platform_data_t
            : implementation::edit_number_data<implementation::gtk> {
        using base = implementation::edit_number_data<implementation::gtk>;
        using base::base;
    };
}