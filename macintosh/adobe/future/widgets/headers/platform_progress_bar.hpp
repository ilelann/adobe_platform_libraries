/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#ifndef ADOBE_PROGRESS_BAR_HPP
#define ADOBE_PROGRESS_BAR_HPP

#include <adobe/config.hpp>

#include <boost/utility.hpp>

#include <adobe/any_regular.hpp>
#include <adobe/extents.hpp>
#include <adobe/layout_attributes.hpp>
#include <adobe/pb_style.hpp>
#include <adobe/widget_attributes.hpp>
#include <adobe/future/widgets/headers/widget_utils.hpp>
#include <adobe/future/widgets/headers/value_range_format.hpp>

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

/*!
    \ingroup apl_widgets_carbon
    
    \brief Progress bar widget
*/
struct progress_bar_t : boost::noncopyable
{
    /// the model type for this widget
    typedef ::SInt32 model_type;

    /// constructor for this widget
    progress_bar_t(pb_style_t                  bar_style, 
                   bool                        is_vertical,
                   const value_range_format_t& format,
                   unsigned long               min,
                   unsigned long               max,
                   theme_t                     theme);
   
    /*!
        @name Placeable Concept Operations
        @{

        See the \ref concept_placeable concept and \ref placeable.hpp for more information.
    */
    void measure(extents_t& result);

    void place(const place_data_t& place_data);
    ///@}

    /*!
        @name View Concept Operations
        @{

        See the \ref concept_view concept and \ref view.hpp for more information.
    */
    void display(const ::SInt32& value);
    ///@}

#ifndef ADOBE_NO_DOCUMENTATION
    ::ControlRef                control_m;
    mutable metric_extractor_t  metrics_m;

    pb_style_t                  bar_style_m;
    bool                        is_vertical_m;
    value_range_format_t        format_m;
    theme_t                     theme_m;

    double                      last_m;
    double                      value_m;
#endif
};

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif

/****************************************************************************************************/
