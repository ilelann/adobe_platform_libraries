/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/****************************************************************************************************/

#ifndef ADOBE_PB_STYLE_HPP
#define ADOBE_PB_STYLE_HPP

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

/*!
\ingroup ui_core

\brief Specifies the style of progress bar to be presented to the user

\note
    Could probably be refactored to specify the intention for the
    progress bar instead of the end-look-and-feel of the progress bar.
*/

enum pb_style_t
{
    /// Typical progress bar intention; repeatedly updated to reflect
    /// the progress of an ongoing task
    pb_style_progress_bar_s,

    /// Updated once (typically) to reflect an amount a certain value
    /// relates to another value. For instance, the relevance of a
    /// search result to the search criterion
    pb_style_relevance_bar_s,

    /// "Barber"-style animation of a bar; repeatedly animated to
    /// reflect the fact that while we don't know how long the task
    /// will take to complete, we can tell you that the app isn't
    /// frozen, it's just thinking hard
    pb_style_indeterminate_bar_s
};

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif

/****************************************************************************************************/
