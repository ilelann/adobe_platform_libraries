/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#ifndef ADOBE_WIDGET_EDIT_NUMBER_EDGE_HPP
#define ADOBE_WIDGET_EDIT_NUMBER_EDGE_HPP

/****************************************************************************************************/

#include <adobe/future/platform_primitives.hpp>

namespace adobe {

/****************************************************************************************************/

struct edit_number_t;

/****************************************************************************************************/

struct edit_number_platform_data_t
{
    explicit edit_number_platform_data_t(edit_number_t* edit_number) :
        control_m(edit_number),
        prev_capture_m(0),
        tracking_m(false)
    {
        last_point_m.x = 0;
        last_point_m.y = 0;
    }

    ~edit_number_platform_data_t();

    void initialize();

    label_message_t label_message;

    edit_number_t* control_m;
    platform_display_type           prev_capture_m;
    bool           tracking_m;
    point_t         last_point_m;
};

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

// ADOBE_WIDGET_EDIT_NUMBER_EDGE_HPP
#endif

/****************************************************************************************************/
