/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#ifndef ADOBE_PLATFORM_PRIMITIVES_EDGE_HPP
#define ADOBE_PLATFORM_PRIMITIVES_EDGE_HPP

/****************************************************************************************************/

#include <boost/logic/tribool.hpp>
#include <string>

#if defined ADOBE_PLATFORM_WIDGETS_WT

 namespace Wt
 {
 	class WWidget;
 	class WCheckBox;
 	class WContainerWidget;
 	class WFormWidget;
 	class WPanel;
 	class WPushButton;
 	class WRadioButton;
 	class WSlider;
 	class WText;
 	class WTextArea;
 	class WImage;
 	class WTabWidget;
 	class WProgressBar;
 	class WGroupBox;
 	class WResource;
 	class WBreak;
 	class WComboBox;
}

#include <Wt/WGlobal>

#elif defined ADOBE_PLATFORM_WIDGETS_WIN
#   define WINDOWS_LEAN_AND_MEAN 1
#   include <windows.h>
#	include <boost/gil/gil_all.hpp>
#	include <uxtheme.h>
#	include <tchar.h>

#if (NTDDI_VERSION >= NTDDI_LONGHORN)
#include <vssym32.h>
#else
	//Yuck:
#	include <tmschema.h>
#	define SCHEME_STRINGS 1
#	include <tmschema.h>
	//End yuck
#endif
#else
#error "Unknown platform"
#endif

/*************************************************************************************************/

namespace adobe {

/*************************************************************************************************/

#if defined ADOBE_PLATFORM_WIDGETS_WT
	typedef Wt::WWidget* platform_display_type;
	typedef int timer_handle_t;
	typedef void window_proc_t();
	typedef int label_message_t;
	typedef void raw_callback_t ();
	typedef void win_proc_implementation_t ();

	typedef std::string image_type;
	typedef struct { int x; int y;} point_t;

	typedef Wt::Key key_type;

	// widget handles
	typedef Wt::WCheckBox*			native_checkbox_t;
	typedef Wt::WText*				native_label_t;
	typedef Wt::WPanel*				native_panel_t;
	typedef Wt::WPushButton*		native_button_t;
	typedef Wt::WRadioButton*		native_radio_button_t;
	typedef Wt::WSlider*			native_slider_t;
	typedef Wt::WText*				native_label_t;
	typedef Wt::WImage*				native_image_t;
	typedef Wt::WImage*				native_reveal_t;
	typedef Wt::WTextArea*			native_edit_text_t;
	typedef Wt::WContainerWidget*	native_window_t;
	typedef Wt::WGroupBox*			native_group_t;
	typedef Wt::WProgressBar*		native_progress_bar_t;
	typedef Wt::WBreak*				native_separator_t;
	typedef Wt::WText*				native_display_number_t;
	typedef Wt::WImage*				native_toggle_t;
	typedef Wt::WComboBox*				native_popup_t;

	struct WTabWidgetWithPredefinedNames;

	typedef WTabWidgetWithPredefinedNames*			native_tab_group_t;

	typedef Wt::WResource*			native_image_resource_t;

#else // if defined ADOBE_PLATFORM_WIDGETS_WIN

	typedef HWND platform_display_type;
	typedef UINT_PTR timer_handle_t;
//	typedef WNDPROC window_proc_t;
	typedef LRESULT window_proc_t (HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	
	typedef LRESULT label_message_t (UINT message, WPARAM wParam, LPARAM lParam);
	typedef LRESULT raw_callback_t (HWND window, UINT message, WPARAM wparam, LPARAM lparam, WNDPROC next_proc);
	typedef LRESULT win_proc_implementation_t (HWND window, UINT message, WPARAM wparam, LPARAM lparam, const char* proc_name);

	typedef boost::gil::rgba8_image_t image_type;
	typedef POINTS point_t;

	typedef long key_type;

	// widget handles
	typedef HWND native_checkbox_t;
	typedef HWND native_label_t;
	typedef HWND native_panel_t;
	typedef HWND native_button_t;
	typedef HWND native_radio_button_t;
	typedef HWND native_label_t;
	typedef HWND native_slider_t;
	typedef HWND native_tab_group_t;
	typedef HWND native_image_t;
	typedef HWND native_window_t;
	typedef HWND native_reveal_t;
	typedef HWND native_edit_text_t;	
	typedef HWND native_group_t;
	typedef HWND native_progress_bar_t;
	typedef HWND native_separator_t;
	typedef HWND native_display_number_t;
	typedef HWND native_toggle_t;
	typedef HWND native_popup_t;

	typedef HBITMAP native_image_resource_t;

#endif

	// temp typedef to track my changes
	struct place_data_t;
	typedef place_data_t place_data_t; // RECT
	typedef wchar_t wide_char_t;
	typedef boost::logic::tribool check_state;

/*************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif

/****************************************************************************************************/
