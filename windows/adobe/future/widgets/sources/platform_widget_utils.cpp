/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/widget_utils.hpp>

#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/windows_cast.hpp>
#include <adobe/name.hpp>
#include <adobe/unicode.hpp>

#include <boost/cstdint.hpp>

#include <vector>

// for native widget callback setup
#include <adobe/future/widgets/headers/platform_button.hpp>
#include <adobe/future/widgets/headers/platform_checkbox.hpp>
#include <adobe/future/widgets/headers/platform_image.hpp>
#include <adobe/future/widgets/headers/platform_popup.hpp>
#include <adobe/future/widgets/headers/platform_radio_button.hpp>
#include <adobe/future/widgets/headers/platform_reveal.hpp>
#include <adobe/future/widgets/headers/platform_slider.hpp>
#include <adobe/future/widgets/headers/platform_tab_group.hpp>
#include <adobe/future/widgets/headers/platform_toggle.hpp>

#include <adobe/future/image_slurp.hpp>

#ifdef ADOBE_PLATFORM_WIDGETS_WT

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace implementation {

/****************************************************************************************************/

void set_window_title(platform_display_type window, const display_string & title)
{
	// ilelann : write some visitor here ?
}

std::string get_window_title(platform_display_type window)
{
  // ilelann : write some visitor here ?
	return "GiveMeAName";
}

/****************************************************************************************************/


template <typename native_widget_t>
native_widget_t add_widget (platform_display_type parent, native_widget_t w)
{
	assert (w);
	if (auto c = dynamic_cast<Wt::WContainerWidget*> (parent))
	{
		c->addWidget (w);
	}
	else if (auto t = dynamic_cast<WTabWidgetWithPredefinedNames*> (parent))
	{
		t->addTabAndNameIt(w);
	}
	else
	{
    assert(false);
	}
	return w;
}

/****************************************************************************************************/

native_button_t make_button (platform_display_type parent, const std::string & label, bool is_default)
{
	return add_widget (parent, new Wt::WPushButton (label));
}

native_checkbox_t make_checkbox (platform_display_type parent, const std::string & label)
{
	return add_widget (parent, new Wt::WCheckBox (label));
}

native_label_t make_label (platform_display_type parent, const std::string & label)
{
	return add_widget (parent, new Wt::WText (label));
}

native_radio_button_t make_radio_button (platform_display_type parent, const std::string & label)
{
	return add_widget (parent, new Wt::WRadioButton (label));
}

native_slider_t make_slider (platform_display_type parent, bool is_vertical, std::size_t num_ticks, adobe::slider_style_t style)
{
	native_slider_t s = new Wt::WSlider (is_vertical ? Wt::Vertical : Wt::Horizontal);
	s->setMinimum(0);
	s->setMaximum(100);
	return add_widget (parent, s);
}

native_panel_t make_panel (platform_display_type parent)
{
	return add_widget (parent, new Wt::WPanel);
}

native_window_t make_window (platform_display_type parent, const std::string & label, const bool & has_size_box)
{
	return add_widget (parent, new Wt::WContainerWidget);
}

native_image_t make_image (platform_display_type parent)
{
  return add_widget (parent, new Wt::WImage);
}

native_reveal_t make_reveal (platform_display_type parent)
{
  return add_widget (parent, new Wt::WImage);
}

native_group_t make_group (platform_display_type parent, const std::string & label)
{
  return add_widget (parent, new Wt::WGroupBox (label));
}

native_tab_group_t make_tab_group (platform_display_type parent, const tab_group_t & tab_group)
{
  std::vector<std::string> v;
  for(const auto & p : tab_group.items_m)
  {
    v.push_back(p.name_m);
  }

  return add_widget (parent, new WTabWidgetWithPredefinedNames(v));
}

native_progress_bar_t make_progress_bar (platform_display_type parent, bool is_vertical, pb_style_t style)
{
  return add_widget (parent, new Wt::WProgressBar);
}

native_edit_text_t make_edit_text(platform_display_type parent, long rows, bool scrollable, bool password)
{
  auto res = new Wt::WTextArea();
  res->setRows(rows);
  return add_widget(parent, res);
}

native_separator_t make_separator(platform_display_type parent)
{
  return add_widget(parent, new Wt::WBreak);
}

native_label_t make_display_number (platform_display_type parent, const place_data_t &)
{
  return add_widget (parent, new Wt::WText);
}

native_toggle_t make_toggle(platform_display_type parent)
{
  return add_widget(parent, new Wt::WImage);
}

native_popup_t make_popup (platform_display_type parent, popup_t& control)
{
  auto res = new Wt::WComboBox;
  res->activated().connect(&control, &popup_t::on_sel_changed);
  return add_widget(parent, res);
}

/****************************************************************************************************/

bool is_control_focused(platform_display_type&);

/****************************************************************************************************/

void setup_callback_radio_button(adobe::radio_button_t & element)
{
	element.control_m->clicked().connect(&element, &adobe::radio_button_t::on_clicked);
}

void setup_callback_checkbox(adobe::checkbox_t & element)
{
	element.control_m->clicked().connect(&element, &adobe::checkbox_t::on_clicked);
}

void setup_callback_button(adobe::button_t & element)
{
	element.control_m->clicked().connect(&element, &adobe::button_t::on_clicked);
}

void setup_callback_slider(adobe::slider_t & element)
{
	element.control_m->valueChanged().connect(&element, &adobe::slider_t::on_new_value);
}

point_t point_from_coordinates (const Wt::Coordinates & coord)
{
  return {coord.x, coord.y};
}

void setup_callback_image(adobe::image_t & element)
{
  element.control_m->mouseMoved().connect([&element](const Wt::WMouseEvent & e){
   element.on_mouse_moved(point_from_coordinates(e.window()));
    });
  element.control_m->mouseWentDown().connect([&element](const Wt::WMouseEvent & e){
   element.on_mouse_down(point_from_coordinates(e.window()));
    });
  element.control_m->mouseWentUp().connect([&element](const Wt::WMouseEvent & e){
   element.on_mouse_up(point_from_coordinates(e.window()));
    });
}

void setup_callback_reveal(adobe::reveal_t & element)
{
  element.control_m->clicked().connect(&element, &adobe::reveal_t::on_clicked);
}

void setup_callback_group(adobe::group_t & element)
{
}

void setup_callback_tab_group(adobe::tab_group_t & element)
{
  element.control_m->currentChanged().connect(&element, &adobe::tab_group_t::on_tab_changed);
}

void setup_callback_toggle(adobe::toggle_t & element)
{
  element.control_m->clicked().connect(&element, &adobe::toggle_t::on_clicked);
}

void setup_callback_edit_text(adobe::edit_text_t & element)
{
}

void set_edit_text_text(native_edit_text_t control, const std::string &text)
{
  control->setText(text);
}

/****************************************************************************************************/

void set_control_bounds(platform_display_type control, const place_data_t& place_data)
{
    assert(control);
}

void set_control_alt_text(platform_display_type control, const std::string &)
{
    assert(control);
}

}

/****************************************************************************************************/

bool is_null_control (platform_display_type control)
{
	return NULL == control;
}

/****************************************************************************************************/

void set_font_checkbox		(platform_display_type w) {}
void set_font_dropdownbutton(platform_display_type w) {}
void set_font_edittext		(platform_display_type w) {}
void set_font_groupbox		(platform_display_type w) {}
void set_font_progressbar	(platform_display_type w) {}
void set_font_pushbutton	(platform_display_type w) {}
void set_font_radiobutton	(platform_display_type w) {}
void set_font_tabitem		(platform_display_type w) {}
void set_font_thumbtop		(platform_display_type w) {}
void set_font_thumbleft		(platform_display_type w) {}

/****************************************************************************************************/

extents_t calculate_edit_bounds(native_edit_text_t control, int cols, int rows)
{
  return {};
}

/****************************************************************************************************/

platform_display_type get_parent_control (platform_display_type child)
{
	return dynamic_cast<platform_display_type> (child->parent());
}

/****************************************************************************************************/

void set_control_visible(platform_display_type control, bool make_visible)
{
    assert(control);
	control->setHidden(!make_visible);
}

/****************************************************************************************************/

void set_control_enabled(platform_display_type control, bool make_enabled)
{
	assert(control);
	control->setDisabled(!make_enabled);
}

/****************************************************************************************************/

bool get_control_enabled(platform_display_type control)
{
    assert(control);
    return control->isEnabled();
}

/****************************************************************************************************/

void reset_image(Wt::WImage * window, Wt::WResource * resource, bool destroy_previous)
{
    assert(window);

    if (destroy_previous)
    {
      delete window->imageLink().resource();
    }

    window->setImageLink(Wt::WLink(resource));
}

/****************************************************************************************************/

void set_progress_bar_position(native_progress_bar_t bar, double position)
{
  bar->setValue(position);
}

/****************************************************************************************************/

void set_slider_value(native_slider_t slider, int value)
{
  slider->setValue(value);
}

/****************************************************************************************************/

void select_popup_string(native_popup_t control, const std::string & text)
{
  control->setValueText(text);
}

void select_popup_index(native_popup_t control, int index)
{
  control->setCurrentIndex(index);
}

int get_popup_index(native_popup_t control)
{
  return control->currentIndex();
}

void prepend_popup_string(native_popup_t control, const std::string & text)
{
  control->insertItem(0, text);
}

void append_popup_string(native_popup_t control, const std::string & text)
{
  control->addItem(text);
}

void delete_popup_index(native_popup_t control, int index)
{
  control->removeItem(index);
}

void clear_popup(native_popup_t control)
{
  control->clear();
}

/****************************************************************************************************/

void set_control_checked(platform_display_type control, check_state state)
{
	assert(control);
	Wt::WCheckBox * cb = dynamic_cast<Wt::WCheckBox*>(control);
	assert(cb);

	Wt::CheckState native_state(Wt::PartiallyChecked);
	if (state)
		native_state = Wt::Checked;
	else if (!state)
		native_state = Wt::Unchecked;
	
	cb->setCheckState(native_state);
}

/****************************************************************************************************/

bool is_key_return(const key_type & key)
{
	return Wt::Key_Enter == key;
}

/****************************************************************************************************/

bool is_key_escape(const key_type & key)
{
	return Wt::Key_Escape == key;
}

/****************************************************************************************************/

}

/****************************************************************************************************/

#else

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

adobe::platform_display_type tooltip_control()
{
    /*
        Tooltips on Windows are windows in and of themselves, and they have a queue of tools to which
        they are bound. In order to take advantage of this we have one global tooltip window, and each
        tool is bound to it with its alternative text string.
    */

    static HWND tooltip_s = CreateWindowEx(WS_EX_TOPMOST,
                                           TOOLTIPS_CLASS,
                                           NULL,
                                           WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           NULL,
                                           NULL,
                                           ::GetModuleHandle(NULL),
                                           NULL
                                           );
    static bool inited(false);

    if (!inited)
    {
        SetWindowPos(tooltip_s,
                     HWND_TOPMOST,
                     0,
                     0,
                     0,
                     0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        inited = true;
    }

    return tooltip_s;
}

/****************************************************************************************************/

std::string window_class(HWND window)
{
    if (::IsWindow(window))
    {
        std::vector<char> class_name(64, 0);

        int num_copied = ::GetClassNameA(window, &class_name[0], static_cast<int>(class_name.size()));

        return std::string(&class_name[0], num_copied);
    }

    return std::string();
}

/****************************************************************************************************/

void set_font(HWND window, int uxtheme_type)
{
    LOGFONTW        log_font = { 0 };
    HFONT           font = 0;

    //
    // Use the metrics to obtain the correct font for this widget.
    //
	adobe::metrics::set_window(window);

	if (adobe::metrics::get_font(uxtheme_type, log_font))
    {
        //
        // Create a font from the LOGFONT structure.
        //
        font = ::CreateFontIndirectW(&log_font);
    }
    assert(font);

    ::SendMessage(window, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
}

} // namespace

/****************************************************************************************************/

namespace hackery {

/****************************************************************************************************/

std::string convert_utf(const WCHAR* buffer, std::size_t size)
{
    assert(buffer);

    typedef     std::vector<char> buftype;
    int         length = WideCharToMultiByte(CP_UTF8, 0, buffer, static_cast<int>(size), 0, 0, 0, 0);
    buftype     tmpbuf(length + 1);
    //
    // Convert characters.
    //
    WideCharToMultiByte(CP_UTF8, 0, buffer, static_cast<int>(size), &(tmpbuf[0]), length, 0, 0);
    return std::string(&(tmpbuf[0]));
}

/****************************************************************************************************/

std::string convert_utf(const WCHAR* buffer)
{
    assert(buffer);

    std::size_t size(0);

    while (buffer[size] != WCHAR(0)) ++size;

    return convert_utf(buffer, size);
}

/****************************************************************************************************/

std::wstring convert_utf(const CHAR* buffer, std::size_t size)
{
    assert(buffer);

    typedef     std::vector<WCHAR> buftype;
    int         length = MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), 0, 0);
    buftype     tmpbuf(length + 1);
    //
    // Convert characters.
    //
    MultiByteToWideChar(CP_UTF8, 0, buffer, static_cast<int>(size), &(tmpbuf[0]), length);
    return std::wstring(&(tmpbuf[0]), tmpbuf.size());
}

/****************************************************************************************************/

std::wstring convert_utf(const CHAR* buffer)
{
    assert(buffer);

    std::size_t size(0);

    while (buffer[size] != CHAR(0)) ++size;

    return convert_utf(buffer, size);
}

/****************************************************************************************************/

} // namespace hackery

/****************************************************************************************************/

namespace {

/****************************************************************************************************/

boost::filesystem::path to_path(const std::vector<TCHAR>& path_buffer)
{
    // At this point we have a path to the save file location, but we don't know
    // if the return value is utf16 or utf8. Boost::filesystem at the moment requires
    // utf8 (or ASCII?) so we need to make the conversion at runtime if necessary.
    // That's what this code does.

    std::vector<char> cpath_buffer;

    const TCHAR* end = std::find(&path_buffer[0], &path_buffer[0] + path_buffer.size(), TCHAR(0));

    cpath_buffer.reserve(std::distance(&path_buffer[0], end));

    adobe::to_utf8(&path_buffer[0], end, std::back_inserter(cpath_buffer));

    cpath_buffer.push_back(0);

    // finally, construct the new path from the converted string buffer and return

    return boost::filesystem::path(&cpath_buffer[0], boost::filesystem::native);
}

/****************************************************************************************************/

} // namespace

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

namespace implementation {

/****************************************************************************************************/
	
native_button_t make_button	(platform_display_type parent, const std::string & label, bool is_default)
{
    DWORD win_style(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP | BS_NOTIFY);
	if (is_default)
        win_style |= BS_DEFPUSHBUTTON;

    return check_not_null (::CreateWindowExW(WS_EX_COMPOSITED /*| WS_EX_TRANSPARENT*/, L"BUTTON",
                                          hackery::convert_utf(label).c_str(),
                                          win_style,
                                          0, 0, 70, 20,
                                          parent,
                                          0,
                                          ::GetModuleHandle(NULL),
                                          NULL));
}

/****************************************************************************************************/

native_checkbox_t make_checkbox (platform_display_type parent, const std::string & label)
{
    return check_not_null (::CreateWindowExW(WS_EX_COMPOSITED | WS_EX_TRANSPARENT, L"BUTTON",
                                          ::hackery::convert_utf(label).c_str(),
                                          WS_CHILD | WS_VISIBLE | BS_3STATE | WS_TABSTOP | BS_NOTIFY,
                                          0, 0, 100, 20,
                                          parent,
                                          NULL,
                                          ::GetModuleHandle(NULL),
                                          NULL));
}

/****************************************************************************************************/

native_label_t make_label (platform_display_type parent, const std::string & label)
{
	return check_not_null (::CreateWindowExW(WS_EX_COMPOSITED, L"STATIC",
                                       hackery::convert_utf(label).c_str(),
                                       WS_CHILD | WS_VISIBLE,
                                       0, 0, 100, 20,
                                       parent,
                                       NULL,
                                       ::GetModuleHandle(NULL),
                                       NULL));
}

/****************************************************************************************************/

native_panel_t make_panel (platform_display_type parent)
{
	return check_not_null (::CreateWindowEx(   WS_EX_CONTROLPARENT | WS_EX_COMPOSITED,
                    _T("eve_panel"),
                    NULL,
                    WS_CHILD | WS_VISIBLE,
                    0, 0, 10, 10,
                    parent,
                    0,
                    ::GetModuleHandle(NULL),
                    NULL));
}

/****************************************************************************************************/

native_radio_button_t make_radio_button (platform_display_type parent, const std::string & label)
{
	return check_not_null (::CreateWindowExW(WS_EX_COMPOSITED | WS_EX_TRANSPARENT, L"BUTTON",
                                          hackery::convert_utf(label).c_str(),
                                          WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | WS_TABSTOP | BS_NOTIFY,
                                          0, 0, 100, 20,
                                          parent,
                                          0,
                                          ::GetModuleHandle(NULL),
                                          NULL));
}

/****************************************************************************************************/

native_slider_t make_slider (platform_display_type parent, bool is_vertical, std::size_t num_ticks, adobe::slider_style_t style)
{
    DWORD win32_style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;

    win32_style |= is_vertical ? TBS_VERT : TBS_HORZ;

    if (num_ticks)
    {
        switch (style)
        {
            case adobe::slider_points_up_s:    win32_style |= TBS_TOP;    break;
            case adobe::slider_points_left_s:  win32_style |= TBS_LEFT;   break;
            case adobe::slider_points_down_s:  win32_style |= TBS_BOTTOM; break;
            case adobe::slider_points_right_s: win32_style |= TBS_RIGHT;  break;
            default: break; // silences a GCC warning
        }
    }
    else
    {
        win32_style |= TBS_NOTICKS;
    }

    return check_not_null (::CreateWindowEx(WS_EX_COMPOSITED,
                                         TRACKBAR_CLASS,
                                         NULL,
                                         win32_style,
                                         0, 0, 20, 20,
                                         parent,
                                         0,
                                         ::GetModuleHandle(NULL),
                                         NULL));

}

/****************************************************************************************************/

native_window_t make_window (platform_display_type parent, const std::string & label, const bool & has_size_box)
{
    DWORD platform_style(WS_OVERLAPPED | WS_CAPTION | WS_BORDER/* | WS_SYSMENU*/);
    DWORD dialog_extended_style = WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_EX_COMPOSITED;

	if (has_size_box)
	{
		platform_style |= WS_SIZEBOX;
	}

	return check_not_null (::CreateWindowExW(dialog_extended_style,
                                         L"eve_dialog",
                                         hackery::convert_utf(label).c_str(),
                                         platform_style,    
                                         10, 10, 20, 20,
                                         parent,
                                         NULL,
                                         ::GetModuleHandle(NULL),
                                         NULL));
}

/****************************************************************************************************/

native_window_t make_image (platform_display_type parent)
{
    return check_not_null (::CreateWindowExA(WS_EX_COMPOSITED, "STATIC",
                                       NULL,
                                       WS_CHILD | WS_VISIBLE | SS_BITMAP,
                                       0, 0,
                                       hackery::cast<int>(value.image_m.width()),
                                       hackery::cast<int>(value.image_m.height()),
                                       parent,
                                       NULL,
                                       ::GetModuleHandle(NULL),
                                       NULL));

    value.handler_m->install<implementation_proc_name>(value.window_m);
}

/****************************************************************************************************/

native_reveal_t make_reveal (platform_display_type parent)
{
return check_not_null(::CreateWindowExW(  WS_EX_COMPOSITED | WS_EX_TRANSPARENT, L"STATIC",
                                    NULL,
                                    WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY,
                                    0, 0, 100, 20,
                                    parent,
                                    0,
                                    ::GetModuleHandle(NULL),
                                    NULL));  
}

/****************************************************************************************************/

native_group_t make_group (platform_display_type parent, const std::string & label)
{
    return check_not_null(::CreateWindowExW(WS_EX_CONTROLPARENT | WS_EX_COMPOSITED | WS_EX_TRANSPARENT,
                                          L"BUTTON",
                                          hackery::convert_utf(label).c_str(),
                                          WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                          0, 0, 10, 10,
                                          parent,
                                          0,
                                          ::GetModuleHandle(NULL),
                                          NULL));
}

/****************************************************************************************************/

native_tab_group_t make_tab_group (platform_display_type parent, const tab_group_t & element)
{
  auto res = check_not_null (::CreateWindowEx(WS_EX_COMPOSITED | WS_EX_CONTROLPARENT,
                                 WC_TABCONTROL,
                                 NULL,
                                 WS_CHILD | WS_VISIBLE, 
                                 0, 0, 10, 10,
                                 parent,
                                 0,
                                 ::GetModuleHandle(NULL),
                                 NULL));

    for (tab_set_t::iterator first(element.items_m.begin()), last(element.items_m.end()); first != last; ++first)
    {
        TCITEM  item = { TCIF_TEXT };

        std::wstring localized(hackery::convert_utf(first->name_m));
        item.pszText = const_cast<wchar_t*>(localized.c_str());
        item.cchTextMax = static_cast<int>(localized.size());

        ::SendMessage(res, TCM_INSERTITEM, ::SendMessage(res, TCM_GETITEMCOUNT, 0, 0), (LPARAM) &item);
    }

    return res;
}


native_progress_bar_t make_progress_bar (platform_display_type parent, bool is_vertical, pb_style_t bar_style)
{
    DWORD style(PBS_SMOOTH);
    if(is_vertical_m)
        style |= PBS_VERTICAL;
    if(bar_style == pb_style_indeterminate_bar_s)
        style |= PBS_MARQUEE;
    auto res = check_not_null(::CreateWindowExW(  WS_EX_COMPOSITED,
                    PROGRESS_CLASS,
                    NULL,
                    WS_CHILD | WS_VISIBLE | style,
                    0, 0, 10, 10,
                    parent,
                    0,
                    ::GetModuleHandle(NULL),
                    NULL));

    if(bar_style == pb_style_indeterminate_bar_s)
    {
        (void) ::SendMessageW((HWND) res,    // handle to destination control     
                        (UINT) PBM_SETMARQUEE,   // message ID     
                        (WPARAM)(BOOL) 1, // = (WPARAM) (bool) on or off;    
                        (LPARAM) 100);           // = (LPARAM) (UINT) time in milliseconds between updates
    }
    else
    {
        // REVISIT: this is a hack to match mac version
        (void) ::SendMessageW(       // returns LRESULT in lResult
        (HWND) res,             // handle to destination control
        (UINT) PBM_SETRANGE,           // message ID
        (WPARAM) 0,                    // = (WPARAM) 0; not used, must be zero
        (LPARAM) MAKELPARAM (0, 100)   // = (LPARAM) MAKELPARAM (nMinRange, nMaxRange)
        );
    }

/****************************************************************************************************/

namespace {
long get_edit_text_window_style(long rows, bool scrollable, bool password) const
{
    //
    // Make it so that the edit box automatically scrolls when the
    // user types beyond the visible region or clicks and drags
    // inside the text box.
    //
    // Also set a couple other flags we always want set.
    //
    long edit_style = ES_AUTOHSCROLL | ES_AUTOVSCROLL |
                      WS_CHILD | WS_TABSTOP | WS_VISIBLE;

    //
    // We can set the ES_MULTILINE style if we have multiple rows.
    // The ES_WANTRETURN flag tells the edit control to interpret
    // the return key as a newline/return, rather than to tell the
    // parent window about it.
    //
    if (rows > 1) 
        edit_style |= ES_MULTILINE | ES_WANTRETURN;

    //
    // The WS_VSCROLL and WS_HSCROLL styles apply to any window.
    //
    if (scrollable)
        edit_style |= WS_VSCROLL | WS_HSCROLL;

    //
    // The ES_PASSWORD style applies to an edit which needs to
    // hide the characters entered.
    //
    if (password)
        edit_style |= ES_PASSWORD;

    return edit_style;
}
}

native_edit_text_t make_edit_text(platform_display_type parent, long rows, bool scrollable, bool password)
{
return check_not_null(::CreateWindowEx( WS_EX_COMPOSITED | WS_EX_CLIENTEDGE,
                    _T("EDIT"),
                    NULL,
                    get_window_style(rows, scrollable, password),
                    0, 0, 100, 20,
                    parent,
                    0,
                    ::GetModuleHandle(NULL),
                    NULL));
}

/****************************************************************************************************/

native_separator_t make_separator(platform_display_type parent)
{
  return check_not_null(  ::CreateWindowEx(WS_EX_COMPOSITED,
                                         WC_STATIC,
                                         NULL,
                                         WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME,
                                         0, 0, 10, 10,
                                         parent,
                                         0,
                                         ::GetModuleHandle(NULL),
                                         NULL));
}

/****************************************************************************************************/

native_label_t make_display_number (platform_display_type parent, const place_data_t & bounds)
{
  return check_not_null(::CreateWindowExW(WS_EX_COMPOSITED, L"STATIC",
                                 NULL,
                                 WS_CHILD | WS_VISIBLE,
                                 left(bounds), top(bounds), width(bounds), height(bounds),
                                 parent,
                                 NULL,
                                 ::GetModuleHandle(NULL),
                                 NULL));
}

/****************************************************************************************************/

native_toggle_t make_toggle(platform_display_type parent)
{
    return check_not_null(::CreateWindowExW(  WS_EX_COMPOSITED | WS_EX_TRANSPARENT, L"STATIC",
                                    NULL,
                                    WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY,
                                    0, 0, 100, 20,
                                    parent,
                                    0,
                                    ::GetModuleHandle(NULL),
                                    NULL));
}

/****************************************************************************************************/

LRESULT CALLBACK popup_subclass_proc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR ptr, DWORD_PTR /* ref */)
{
    adobe::popup_t& control(*reinterpret_cast<adobe::popup_t*>(ptr));

    assert(control.control_m);

    //
    // Inform everybody that the selection is changing.
    //
    if (message == WM_COMMAND &&
        HIWORD(wParam) == CBN_SELCHANGE)
    {
        control.on_sel_changed();
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

native_popup_t make_popup (platform_display_type parent, popup_t& control)
{
  auto res = check_not_null(::CreateWindowEx(WS_EX_COMPOSITED, WC_COMBOBOX,
                                         NULL,
                                         WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP | WS_VSCROLL,
                                         0, 0, 0, 0,
                                         parent,
                                         0,
                                         ::GetModuleHandle(NULL),
                                         NULL));

    ::SetWindowSubclass(res, popup_subclass_proc, reinterpret_cast<UINT_PTR>(&control), 0);

}

/****************************************************************************************************/

native_image_resource_t make_image_resource(const char * path)
{
        boost::gil::rgba8_image_t image;

        adobe::image_slurp(path, image);

        return make_image_resource(image);; 
}

/****************************************************************************************************/

native_image_resource_t make_image_resource      (const boost::gil::rgba8_image_t & view)
{
  return adobe::to_bitmap(view);
}

/****************************************************************************************************/

bool is_control_focused(platform_display_type control)
{
 return ::GetFocus() == control; 
}


/****************************************************************************************************/

LRESULT CALLBACK radio_button_subclass_proc(HWND     window,
                                      UINT     message,
                                      WPARAM   wParam,
                                      LPARAM   lParam,
                                      UINT_PTR ptr,
                                      DWORD_PTR /* ref */)
{
    if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)
    {
	    adobe::radio_button_t& radio_button(*reinterpret_cast<adobe::radio_button_t*>(ptr));
		radio_button.on_clicked();
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_radio_button (radio_button_t & element)
{
	::SetWindowSubclass(element.control_m, &radio_button_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

LRESULT CALLBACK button_subclass_proc(HWND     window,
                                      UINT     message,
                                      WPARAM   wParam,
                                      LPARAM   lParam,
                                      UINT_PTR ptr,
                                      DWORD_PTR /* ref */)
{
    if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)
    {
	    adobe::button_t& button(*reinterpret_cast<adobe::button_t*>(ptr));
		button.on_clicked();
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_button(adobe::button_t & element)
{
	::SetWindowSubclass(element.control_m, &button_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

LRESULT CALLBACK checkbox_subclass_proc(HWND     window,
                                      UINT     message,
                                      WPARAM   wParam,
                                      LPARAM   lParam,
                                      UINT_PTR ptr,
                                      DWORD_PTR /* ref */)
{
    if (message == WM_COMMAND && HIWORD(wParam) == BN_CLICKED)
    {
	    adobe::checkbox_t& checkbox(*reinterpret_cast<adobe::checkbox_t*>(ptr));
		checkbox.on_clicked();
		return 0;
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_checkbox(adobe::checkbox_t & element)
{
	::SetWindowSubclass(element.control_m, &checkbox_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

LRESULT CALLBACK slider_subclass_proc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR ptr, DWORD_PTR /* ref */)
{
    adobe::slider_t& control(*reinterpret_cast<adobe::slider_t*>(ptr));

    assert(control.control_m);

    if (control.value_proc_m.empty() == false &&
        (message == WM_HSCROLL || message == WM_VSCROLL))
    {
        WORD submsg(LOWORD(wParam));

        if (submsg == TB_LINEUP        || submsg == TB_LINEDOWN   ||
            submsg == TB_PAGEUP        || submsg == TB_PAGEDOWN   ||
            submsg == TB_THUMBPOSITION || submsg == TB_THUMBTRACK ||
            submsg == TB_TOP           || submsg == TB_BOTTOM     ||
            submsg == TB_ENDTRACK)
        {
            long   new_position(static_cast<long>(::SendMessage(window, TBM_GETPOS, 0, 0)));
			adobe::slider_t::model_type new_value(control.format_m.at(new_position).cast<adobe::slider_t::model_type>());

			control.on_new_value (new_value);
        }
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}


void setup_callback_slider(adobe::slider_t & element)
{
	::SetWindowSubclass(element.control_m, &slider_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

point_t point_from_lparam(LPARAM lParam)
{
  POINTS cur_point(MAKEPOINTS(lparam));
  return {cur_point.x, cur_point.y};
}

LRESULT image_subclass_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR ptr, DWORD_PTR /* ref */)
{
    adobe::image_t& control(*reinterpret_cast<adobe::image_t*>(ptr));

    if (message == WM_NCHITTEST)
    {
        return HTCLIENT;
    }
    else if (message == WM_MOUSEMOVE && control.tracking_m)
    {
      control.on_mouse_moved(point_from_lparam(lparam))
    }
    else if (message == WM_LBUTTONDOWN)
    {
      control.on_mouse_down(point_from_lparam(lparam))
      control.prev_capture_m = ::SetCapture(control.control_m);
      return 0;
    }
    else if (message == WM_LBUTTONUP)
    {
      control.on_mouse_up(point_from_lparam(lparam));
      ::SetCapture(control.prev_capture_m);
      return 0;
    }

    return ::DefSubclassProc(window, message, wparam, lparam);
}

void setup_callback_image(adobe::image_t & element)
{
  ::SetWindowSubclassA(element.control_m, &image_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

LRESULT CALLBACK reveal_subclass_proc(HWND     window,
                                      UINT     message,
                                      WPARAM   wParam,
                                      LPARAM   lParam,
                                      UINT_PTR ptr,
                                      DWORD_PTR /* ref */)
{
    if (message == WM_COMMAND && HIWORD(wParam) == STN_CLICKED)
    {
      adobe::reveal_t& reveal(*reinterpret_cast<adobe::reveal_t*>(ptr));

      reveal.on_clicked();
        return 0;
    }

    // nevermind.
    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_reveal(adobe::image_t & element)
{
  ::SetWindowSubclass(element.control_m, &reveal_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

LRESULT CALLBACK group_subclass_proc(HWND     window,
                                     UINT     message,
                                     WPARAM   wParam,
                                     LPARAM   lParam,
                                     UINT_PTR /* ptr */,
                                     DWORD_PTR /* ref */)
{
    // Why does group_t have an empty set_theme and a funny event handler?
    // There seems to be a bug in Win32 with visual styles enabled where a
    // group box nested inside another group box has the wrong font if it
    // has ever been sent a WM_SETFONT message; at least, if it has been
    // sent a WM_SETFONT then it ignores the font selected into the DC in
    // the WM_CTLCOLORSTATIC handler.
    //
    // So: we don't send a WM_SETFONT by not implementing a proper set_theme.
    // We implement a WM_CTLCOLORSTATIC that selects a suitable font into the
    // group's HDC.
    //
    // Note that we probably *should* get the font using the metrics interface
    // rather than using the DEFAULT_GUI_FONT (as control_t::set_theme does).
    //

#if 0
    adobe::group_t& control(*reinterpret_cast<adobe::group_t*>(ptr));

    if (message == WM_CTLCOLORSTATIC ||
        message == WM_CTLCOLORBTN ||
        message == WM_CTLCOLOREDIT ||
        message == WM_CTLCOLORLISTBOX ||
        message == WM_CTLCOLORSCROLLBAR)
    {
        LOGFONTW log_font = { 0 };

        adobe::metrics::set_window(control.control_m);

        if (adobe::metrics::get_font(BP_GROUPBOX, log_font) == false)
            throw std::runtime_error("could not get the right font");

        HFONT font(::CreateFontIndirectW(&log_font));

        ::SelectObject(hackery::cast<HDC>(wParam), font);
        adobe::metrics::draw_parent_background(hackery::cast<HWND>(lParam), hackery::cast<HDC>(wParam));
    }
    else
#endif
    {
        LRESULT forward_result(0);

        if (adobe::forward_message(message, wParam, lParam, forward_result))
            return forward_result;
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_group(adobe::image_t & element)
{
    ::SetWindowSubclass(element.control_m, &group_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

LRESULT CALLBACK tab_group_subclass_proc(HWND     window,
                                         UINT     message,
                                         WPARAM   wParam,
                                         LPARAM   lParam,
                                         UINT_PTR ptr,
                                         DWORD_PTR /* ref */)
{
  adobe::tab_group_t& tab_group(*reinterpret_cast<adobe::tab_group_t*>(ptr));
    NMHDR*                         notice((NMHDR*) lParam);

    //
    // Here we report the selected tab changing to ui-core users.
    //

    if (message == WM_NOTIFY && notice && notice->hwndFrom == window)
    {
        if (notice->code == TCN_SELCHANGE)
        {
            long index(static_cast<long>(::SendMessage(tab_group.control_m, TCM_GETCURSEL, 0, 0)));
      tab_group.on_tab_changed (index);
        }

        return 0;
    }
    else
    {
        LRESULT forward_result(0);

    if (adobe::forward_message(message, wParam, lParam, forward_result))
            return forward_result;
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_tab_group(adobe::tab_group_t & element)
{
  ::SetWindowSubclass(element.control_m, &tab_group_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

LRESULT CALLBACK toggle_subclass_proc(HWND     window,
                                      UINT     message,
                                      WPARAM   wParam,
                                      LPARAM   lParam,
                                      UINT_PTR ptr,
                                      DWORD_PTR ref)
{
    adobe::toggle_t& toggle(*reinterpret_cast<adobe::toggle_t*>(ptr));

    if (message == WM_COMMAND && HIWORD(wParam) == STN_CLICKED)
    {
        toggle.on_clicked();
        return 0;
    }

    // nevermind.
    return DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_toggle(adobe::toggle_t & element)
{
  ::SetWindowSubclass(element.control_m, &toggle_subclass_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

/****************************************************************************************************/

void select_tab(native_tab_group_t tab_group, std::size_t index)
{
  ::SendMessage(tab_group, TCM_SETCURSEL, index, 0);
}

/****************************************************************************************************/

/****************************************************************************************************/
//
/// This callback handles incoming keypresses and uses the pre_edit signal to determine
/// if the key press should be allowed into the editbox or not.
///
/// The rationale for having a custom window procedure for edit controls is that there
/// is no way to handle single key-press events before they get processed by the edit
/// control and merged with the existing text (meaning we would have to recalculate the
/// change which occured).
//
LRESULT CALLBACK edit_text_window_proc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR ptr, DWORD_PTR ref)
{
    edit_text_t& edit_text(*reinterpret_cast<edit_text_t*>(ptr));
    bool         squelch(false);
    bool         keyevent(message == WM_KEYDOWN || message == WM_CHAR);
    wchar_t      character(keyevent ? static_cast<wchar_t>(wParam) : 0);

    // Looking for the arrow key code? It's been moved to edit_number::handle_key

    if (message == WM_MOUSEWHEEL && edit_text.pre_edit_proc_m)
    {
        if(GET_WHEEL_DELTA_WPARAM(wParam) > 0)
            character = VK_UP;
        else if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
            character = VK_DOWN;

        if (character)
        {
            char c(static_cast<char>(character));

            switch (character)
            {
                case VK_UP:   c = 30; break;
                case VK_DOWN: c = 31; break;
            }

            std::string str(&c, 1);

            edit_text.pre_edit_proc_m(str, squelch);
        }

        return 0;
    }
    else if (message == WM_CHAR)
    {
        std::string old_value(get_control_string(edit_text));
        DWORD       old_selection_start(0);
        DWORD       old_selection_end(0);

        ::SendMessage(edit_text.control_m,
                      EM_GETSEL,
                      (WPARAM) &old_selection_start,
                      (LPARAM) &old_selection_end);

        // pass the event to the control to get the new string
        LRESULT result = ::DefSubclassProc(window, message, wParam, lParam);

        std::string new_value(get_control_string(edit_text));

        // if the text string is the same, we're done here.
        if (new_value == old_value)
            return result;

        squelch = edit_text.rows_m == 1 &&
                  edit_text.max_cols_m > 0 &&
                  static_cast<std::size_t>(edit_text.max_cols_m) < new_value.size();

        // send the new text off to the prefilter, but only
        // if we haven't hit our max length quota (but only
        // in the case when we're one row high)

        if (squelch == false && edit_text.pre_edit_proc_m)
            edit_text.pre_edit_proc_m(new_value, squelch);

        if (squelch)
        {
            ::MessageBeep(-1);

            edit_text.set_field_text(old_value);

            // restore the selection location, too
            ::SendMessage(edit_text.control_m, EM_SETSEL, old_selection_start, old_selection_end);
        }
        else if (edit_text.post_edit_proc_m)
        {
            edit_text.value_m = new_value;

            edit_text.post_edit_proc_m(new_value);
        }

        return result;
    }

    return ::DefSubclassProc(window, message, wParam, lParam);
}

void setup_callback_edit_text(adobe::edit_text_t & element)
{
    ::SetWindowSubclass(element.control_m, edit_text_window_proc, reinterpret_cast<UINT_PTR>(&element), 0);
}

void set_edit_text_text(native_edit_text_t control, const std::string & text)
{
    ::SendMessageW(control, WM_SETTEXT, 0, (LPARAM) hackery::convert_utf(text.c_str()).c_str());
}

/****************************************************************************************************/

bool pick_file(boost::filesystem::path& path, platform_display_type dialog_parent)
{
    // This upper part sets up the OS-specific open file dialog
    // and invokes it so the user can pick a file to open

    std::vector<TCHAR> path_buffer(2048, 0);
    OPENFILENAME       params = { 0 };

    params.hwndOwner = dialog_parent;
    params.lStructSize = sizeof(params);
    params.lpstrFilter = L"Text File\0*.txt\0All Files\0*.*\0\0";
    params.lpstrFile = &path_buffer[0];
    params.nMaxFile = static_cast<DWORD>(path_buffer.size());
    params.Flags = OFN_DONTADDTORECENT |
                   OFN_LONGNAMES |
                   OFN_NOCHANGEDIR |
                   OFN_NONETWORKBUTTON |
                   OFN_PATHMUSTEXIST |
                   OFN_FILEMUSTEXIST;
    params.lpstrDefExt = L"txt";

    bool result(::GetOpenFileName(&params) != 0);

    if (result)
        path = to_path(path_buffer);

    return result;
}

/****************************************************************************************************/

bool pick_save_path(boost::filesystem::path& path, platform_display_type dialog_parent)
{
    // This upper part sets up the OS-specific save file dialog and
    // invokes it so the user can pick a save file location

    std::vector<TCHAR> path_buffer(2048, 0);
    OPENFILENAME       params = { 0 };

    params.hwndOwner = dialog_parent;
    params.lStructSize = sizeof(params);
    params.lpstrFilter = L"Text File\0*.txt\0All Files\0*.*\0\0";
    params.lpstrFile = &path_buffer[0];
    params.nMaxFile = static_cast<DWORD>(path_buffer.size());
    params.Flags = OFN_DONTADDTORECENT |
                   OFN_LONGNAMES |
                   OFN_NOCHANGEDIR |
                   OFN_NONETWORKBUTTON |
                   OFN_NOREADONLYRETURN |
                   OFN_PATHMUSTEXIST;
    params.lpstrDefExt = L"txt";

    bool result(::GetSaveFileName(&params) != 0);

    if (result)
        path = to_path(path_buffer);

    return result;
}

/****************************************************************************************************/

void set_control_alt_text(HWND control, const std::string& alt_text)
{
    HWND               tooltip(tooltip_control()); // handle to the ToolTip control
    TOOLINFO           tooltip_info;               // struct specifying info about tool in ToolTip control
    std::vector<TCHAR> text_buffer(alt_text.size() * 2, 0);

    /* Set up the text for filling in the tooltip structure below */
    to_utf16(&alt_text[0], &alt_text[0] + alt_text.size(), &text_buffer[0]);

    /* Basic initialization of the tooltip structure parameters */
    tooltip_info.cbSize = sizeof(TOOLINFO);
    tooltip_info.uFlags = TTF_SUBCLASS | TTF_IDISHWND | TTF_CENTERTIP;
    tooltip_info.hwnd = control;
    tooltip_info.hinst = ::GetModuleHandle(NULL);
    tooltip_info.uId = hackery::cast<UINT_PTR>(control);
    tooltip_info.lpszText = &text_buffer[0];

    // ToolTip control will cover the whole window
    ::GetClientRect (control, &tooltip_info.rect);

    /*
        First delete any previous references to this control from the TT's tool list. This
        allows for widgets with potentially multiple alt_texts (e.g., buttons and their
        different states) to display each one when appropriate. If there is no previous
        reference then this does nothing, which is fine.
    */
    SendMessage(tooltip, TTM_DELTOOL, 0, (LPARAM) (LPTOOLINFO) &tooltip_info);

    /* At the last, we bind the tooltip to the control */
    SendMessage(tooltip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &tooltip_info);
}

/****************************************************************************************************/

std::string get_window_title(HWND window)
{
    assert(window);

    int const buffer_size = ::GetWindowTextLengthW(window) + 1;

    std::vector<WCHAR> titlename(buffer_size, 0);

    int const text_length = ::GetWindowTextW(window, &titlename[0], buffer_size);

    assert(text_length < buffer_size);

    return std::string(hackery::convert_utf(&titlename[0]));
}

/****************************************************************************************************/

void set_window_title(platform_display_type window, const display_string & title)
{
    assert(window);

	::SetWindowTextW(window, hackery::convert_utf (title).c_str());
}

/****************************************************************************************************/

//void set_window_image (platform_display_type window, const image_type & image)
//{
//    assert(window);
//
//	::SetWindowTextW(window, hackery::convert_utf (title).c_str());
//}

/****************************************************************************************************/

void get_control_bounds(platform_display_type control, place_data_t& p)
{
    assert(control);

	RECT rect;
    ::GetWindowRect(control, &rect);

	from_native (rect, p);
}

/****************************************************************************************************/

void set_control_bounds(HWND control, const place_data_t& place_data)
{
    assert(control);

    ::MoveWindow(control, left(place_data), top(place_data), width(place_data), height(place_data), TRUE);
}

/****************************************************************************************************/

void throw_last_error_exception(const char* /* file */, long /* line */)
{
    DWORD error(::GetLastError());
    char  the_message[2048] = { 0 };

    ::FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        error,
        0,
        &the_message[0],
        sizeof(the_message),
        NULL);

    char* actual_error = the_message[0] ? the_message : "Unknown Error.";

    ::MessageBoxA(0, actual_error, "Error Message From Windows", MB_ICONEXCLAMATION | MB_APPLMODAL | MB_OK);

    throw std::runtime_error(the_message);
}

/****************************************************************************************************/

modifiers_t convert_modifiers(ULONG os_modifiers)
{
    modifiers_t result(modifiers_none_s);

#define ADOBE_MAPMOD(osmod, adobemod)   if (os_modifiers == (osmod)) result = result | (adobemod)
    //
    // Note that VK_MENU is the ALT key.
    //
    ADOBE_MAPMOD(VK_CAPITAL,    modifiers_caps_lock_s);
    ADOBE_MAPMOD(VK_CONTROL,    modifiers_any_control_s);
    ADOBE_MAPMOD(VK_SHIFT,      modifiers_any_shift_s);
    ADOBE_MAPMOD(VK_MENU,       modifiers_any_option_s);

#undef ADOBE_MAPMOD

    return result;
}

/****************************************************************************************************/

modifiers_t convert_modifiers(BYTE keyboard_state[256])
{
    modifiers_t result(modifiers_none_s);

    if (keyboard_state[VK_CAPITAL] & 0x80)  result |= modifiers_caps_lock_s;
    if (keyboard_state[VK_LSHIFT] & 0x80)   result |= modifiers_left_shift_s;
    if (keyboard_state[VK_RSHIFT] & 0x80)   result |= modifiers_right_shift_s;
    if (keyboard_state[VK_LCONTROL] & 0x80) result |= modifiers_left_control_s;
    if (keyboard_state[VK_RCONTROL] & 0x80) result |= modifiers_right_control_s;
    if (keyboard_state[VK_LMENU] & 0x80)    result |= modifiers_left_option_s;
    if (keyboard_state[VK_RMENU] & 0x80)    result |= modifiers_right_option_s;

    return result;
}

/****************************************************************************************************/

modifiers_t modifier_state()
{
    BYTE keyboard_state[256] = { 0 };

    ::GetKeyboardState(&keyboard_state[0]);

    return convert_modifiers(keyboard_state);
}

/****************************************************************************************************/

} // namespace implementation

/****************************************************************************************************/

bool forward_message(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& forward_result)
{
    static const std::string listview_class_k("SysListView32");

    if (message == WM_COMMAND)
    {
        HWND actual_window(reinterpret_cast<HWND>(lParam));

        if (actual_window == NULL)
            return false;

        if (window_class(actual_window) == listview_class_k)
            return false;

        forward_result = ::SendMessage(actual_window, message, wParam, lParam);

        return forward_result == 0;
    }
    else if (message == WM_NOTIFY)
    {
        NMHDR& notify_header(*reinterpret_cast<NMHDR*>(lParam));
        HWND   actual_window(notify_header.hwndFrom);

        if (actual_window == NULL)
            return false;

        //if (window_class(actual_window) == listview_class_k)
        //    return false;

        forward_result = ::SendMessage(actual_window, message, wParam, lParam);

        // From WM_NOTIFY docs:
        // "The return value is ignored except for notification messages that specify otherwise."
        // Lame.

        // note: this list is not exhaustive. Items should be added to the list
        //       as notification messages are found that require return values.

        bool notify_message_set_that_returns_stuff(message == NM_CUSTOMDRAW);

        return notify_message_set_that_returns_stuff;
    }
    else if (message == WM_HSCROLL || message == WM_VSCROLL)
    {
        HWND actual_window(reinterpret_cast<HWND>(lParam));

        if (actual_window == NULL)
            return false;

        if (window_class(actual_window) == listview_class_k)
            return false;

        forward_result = ::SendMessage(actual_window, message, wParam, lParam);

        return forward_result == 0;
    }

    return false;
}

/****************************************************************************************************/

bool context_menu(HWND parent,
                  long x, long y,
                  const name_t* first,
                  const name_t* last,
                  name_t& result)
{
    HMENU menu(::CreatePopupMenu());

    const name_t* the_first(first);

    long count(0);

    for (; first != last; ++first)
    {
        MENUITEMINFOA item = { 0 };
        std::size_t   length(std::strlen(first->c_str()));
        bool          is_separator(*first == static_name_t("-"));

        item.cbSize = sizeof(item);
        item.fMask = MIIM_FTYPE | MIIM_ID | (is_separator ? 0 : MIIM_STRING);
        item.fType = is_separator ? MFT_SEPARATOR : MFT_STRING;
        item.wID = ++count;
        item.dwTypeData = (LPSTR)first->c_str();
        item.cch = (UINT)length;

        ::InsertMenuItemA(menu, count, false, &item);
    }

    long choice(::TrackPopupMenu(menu,
                                 TPM_LEFTALIGN           |
                                     TPM_TOPALIGN        |
                                     TPM_LEFTBUTTON      |
                                     TPM_HORPOSANIMATION |
                                     TPM_VERPOSANIMATION |
                                     TPM_NONOTIFY        |
                                     TPM_RETURNCMD,
                                 x, y, 0, parent, 0));

    ::DestroyMenu(menu);

    if (choice == 0)
        return false;

    result = *(the_first + choice - 1);

    return true;
}

/****************************************************************************************************/

void set_font(HWND window, int uxtheme_type)
{
    LOGFONTW        log_font = { 0 };
    HFONT           font = 0;

    //
    // Use the metrics to obtain the correct font for this widget.
    //
    metrics::set_window(window);

    if (metrics::get_font(uxtheme_type, log_font))
    {
        //
        // Create a font from the LOGFONT structure.
        //
        font = ::CreateFontIndirectW(&log_font);
    }
    assert(font);

    ::SendMessage(window, WM_SETFONT, reinterpret_cast<WPARAM>(font), true);
}

/****************************************************************************************************/

void set_font_checkbox		(platform_display_type w) { return set_font (w, BP_CHECKBOX)		;}
void set_font_dropdownbutton(platform_display_type w) { return set_font (w, CP_DROPDOWNBUTTON)	;}
void set_font_edittext		(platform_display_type w) { return set_font (w, EP_EDITTEXT)		;}
void set_font_groupbox		(platform_display_type w) { return set_font (w, BP_GROUPBOX)		;}
void set_font_progressbar	(platform_display_type w) { return set_font (w, PP_BAR)				;}
void set_font_pushbutton	(platform_display_type w) { return set_font (w, BP_PUSHBUTTON)		;}
void set_font_radiobutton	(platform_display_type w) { return set_font (w, BP_RADIOBUTTON)		;}
void set_font_tabitem		(platform_display_type w) { return set_font (w, TABP_TABITEM)		;}
void set_font_thumbtop		(platform_display_type w) { return set_font (w, TKP_THUMBTOP)		;}
void set_font_thumbleft		(platform_display_type w) { return set_font (w, TKP_THUMBLEFT)		;}

/****************************************************************************************************/
//
/// This function will calculate the best bounds for an edit control. This is
/// a bit more complicated than it could be, because we can't get all of the
/// values we want from the UXTheme API. Fortunately we can get them from a
/// combination of User32 and UXTheme.
///
/// This function is used by the edit widget, as well as the popup widget
/// (which contains an edit widget).
///
/// \param  control the HWND of the edit control to obtain the best bounds for.
/// \param  cols    the number of columns (or characters across) the edit control should contain
/// \param  rows    the number of rows of text to contain.
///
/// \return the best bounds for the edit control, including baseline.
//
extents_t calculate_edit_bounds(native_edit_text_t control, int cols, int rows)
{
    extents_t result;
    //
    // First make sure that we have a valid control.
    //
    assert(control);
    //
    // We figure out the size by sending an EM_GETRECT message to the edit
    // control. This gives us the offset which it draws at inside it's
    // client rectangle.
    //
    RECT em_rect = {0, 0, 0, 0};
    SendMessage(control, EM_GETRECT, 0, (LPARAM)&em_rect);
    //
    // Next we need to find out where the client rectangle lies within the
    // bounds of the regular window.
    // 
    WINDOWINFO wi = { 0 };
    wi.cbSize = sizeof(wi);
    if (!GetWindowInfo(control, &wi)) return result;
    //
    // Finally we need to extract the font metrics from UxTheme, so that we
    // can correctly calculate the baseline.
    //
    TEXTMETRIC font_metrics;
    place_data_t extents;
    int border = 0;
    if (!metrics::set_window(control)) return result;
    if (!metrics::get_text_extents(EP_EDITTEXT, std::wstring(L"0", cols), extents)) return result;
    if (!metrics::get_font_metrics(EP_EDITTEXT, font_metrics)) return result;
    if (!metrics::get_integer(EP_EDITTEXT, TMT_BORDERSIZE, border)) return result;
    //
    // Now we can calculate the rectangle:
    //
    // Height: Top border + Bottom border + (rows * text height) + vertical text offset
    // Width : Left border + Right border + text extent
    // Baseline: Top border + font ascent + one if this has multiple lines...
    //
    RECT offsets;

    offsets.top = wi.rcClient.top - wi.rcWindow.top;
    offsets.left = wi.rcClient.left - wi.rcWindow.left;
    offsets.bottom = wi.rcWindow.bottom - wi.rcClient.bottom;
    offsets.right = wi.rcWindow.right - wi.rcClient.right;

    result.height() = offsets.top + offsets.bottom + (rows * font_metrics.tmHeight) + em_rect.top + border;
    result.width() = width(extents) + offsets.left + offsets.right;
    result.vertical().guide_set_m.push_back(offsets.top + font_metrics.tmAscent + border);

    return result;
}

/****************************************************************************************************/

bool is_null_control (platform_display_type control)
{
	return NULL == control;
}

/****************************************************************************************************/

platform_display_type get_parent_control (platform_display_type child)
{
	return ::GetParent (child);
}

/****************************************************************************************************/

void set_control_visible(platform_display_type control, bool make_visible)
{
    assert(control);

    ::ShowWindow(control, make_visible ? SW_SHOWNORMAL : SW_HIDE);
}

/****************************************************************************************************/

void set_control_enabled(platform_display_type control, bool make_enabled)
{
    assert(control);

    ::EnableWindow(control, make_enabled);
}

/****************************************************************************************************/

bool get_control_enabled(platform_display_type control)
{
    assert(control);

    return ::IsWindowEnabled(control) == TRUE;
}

/****************************************************************************************************/

void set_control_checked(platform_display_type control, check_state state)
{
	assert(control);

	WPARAM native_state(BST_INDETERMINATE);
	if (state)
		native_state = BST_CHECKED;
	else if (!state)
		native_state = BST_UNCHECKED;
	
	::SendMessage(control, BM_SETCHECK, native_state, 0);
}

/****************************************************************************************************/

void reset_image(HWND window, HBITMAP bitmap_handle, bool destroy_previous)
{
    HBITMAP old_bm_handle = reinterpret_cast<HBITMAP>(
        ::SendMessage(window, STM_SETIMAGE, IMAGE_BITMAP, hackery::cast<LPARAM>(bitmap_handle)));

    HRESULT result = S_OK;

    if (destroy_previous)
    {
      if(old_bm_handle) {
          result = ::DeleteObject(reinterpret_cast<HBITMAP>(old_bm_handle));
      }
    }
    if(result != S_OK) ADOBE_THROW_LAST_ERROR;

/*
    HBITMAP new_bm_handle = 
        reinterpret_cast<HBITMAP>(::SendMessage(window, STM_GETIMAGE, IMAGE_BITMAP, 0));
    
   This idea doesn't seem to work. Was trying to detect alpha pixel case so as to 
   be able to delete orgibal bitmap if it was in fact copied. But result comes back
   as S_FALSE, so need a different technique. 
    if(new_bm_handle != bitmap_handle)
        result = ::DeleteObject(bitmap_handle);
    if(result != S_OK) ADOBE_THROW_LAST_ERROR;
*/ 
    
}

/****************************************************************************************************/

void set_progress_bar_position(native_progress_bar_t bar, double position)
{
            SendMessageW((bar,     // handle to destination control     
                            (UINT) PBM_SETPOS,    // message ID     
                            (WPARAM)(int) position,  // = (WPARAM) (int) nNewPos;    
                            (LPARAM) 0);          // = 0; not used, must be zero );
}

/****************************************************************************************************/

void set_slider_value(native_slider_t slider, int value)
{
  ::SendMessage(slider, TBM_SETPOS, true, static_cast<LPARAM>(value)); 
}

/****************************************************************************************************/

void select_popup_string(native_popup_t control, const std::string & text)
{
  ::SendMessageW(control, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) hackery::convert_utf(text.c_str()).c_str());
}

void select_popup_index(native_popup_t control, int index)
{
  ::SendMessage(control, CB_SETCURSEL, index, 0);
}

int get_popup_index(native_popup_t control)
{
  return static_cast<int>(::SendMessage(control, CB_GETCURSEL, 0, 0));
}

void prepend_popup_string(native_popup_t control, const std::string & text)
{
  ::SendMessageA(control, CB_INSERTSTRING, 0, (LPARAM)text.c_str());
}

void append_popup_string(native_popup_t control, const std::string & text)
{
  if (::SendMessageW(control, CB_ADDSTRING, 0, (LPARAM) hackery::convert_utf(text).c_str()) == CB_ERR)
    ADOBE_THROW_LAST_ERROR;
}

void delete_popup_index(native_popup_t control, int index)
{
  ::SendMessage(control, CB_DELETESTRING, 0, 0);
}

void clear_popup(native_popup_t control)
{
  if (::SendMessage(control, CB_RESETCONTENT, 0, 0) == CB_ERR)
    ADOBE_THROW_LAST_ERROR;
}

/****************************************************************************************************/

LONG_PTR get_user_reference(HWND control)
{
    assert(control);

    return ::GetWindowLongPtr(control, GWLP_USERDATA);
}

/****************************************************************************************************/

bool is_key_return(const key_type & key)
{
	return VK_RETURN == key;
}

/****************************************************************************************************/

bool is_key_escape(const key_type & key)
{
	return VK_ESCAPE == key;
}

/****************************************************************************************************/

platform_display_type get_top_level_window(platform_display_type thing)
{
    return ::GetAncestor(thing, GA_ROOT);
}

/****************************************************************************************************/

} // namespace adobe

/****************************************************************************************************/

#endif