/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://stlab.adobe.com/licenses.html)
*/

/****************************************************************************************************/

#include <adobe/future/widgets/headers/platform_panel.hpp>
#include <adobe/future/widgets/headers/display.hpp>
#include <adobe/future/widgets/headers/platform_metrics.hpp>
#include <adobe/future/windows_cast.hpp>

/****************************************************************************************************/

#ifndef ADOBE_PLATFORM_WIDGETS_WT

namespace {

/****************************************************************************************************/
//
/// This function is a hack to create a background brush which can be used by
/// children of panels on tabs. The brush is generated by creating a bitmap
/// the size of the panel, and drawing the panel background into the bitmap
/// before converting the bitmap into a patterned brush.
///
/// \param  panel   the panel window to make a brush for.
//
HBRUSH make_background_brush(HWND panel)
{
    HDC dc = GetDC(panel);
    SetBkMode(dc, TRANSPARENT);
    RECT dimensions = {0, 0, 0, 0};
    GetClientRect(panel, &dimensions);
    //
    // Make a memory device context and bitmap.
    //
    HDC memory_dc = CreateCompatibleDC(dc);
    if (!memory_dc) ADOBE_THROW_LAST_ERROR;
    HBITMAP memory_bmp = CreateCompatibleBitmap(dc, dimensions.right, dimensions.bottom);
    if (!memory_bmp) ADOBE_THROW_LAST_ERROR;
    //
    // Select the new bitmap into the memory DC.
    //
    HGDIOBJ bmp_old = SelectObject(memory_dc, memory_bmp);
    //
    // Draw the background into the bitmap.
    //
    adobe::metrics::draw_parent_background( panel, memory_dc );
    //
    // Create the patterned brush, and verify that it was created.
    //
    HBRUSH b = CreatePatternBrush(memory_bmp);
    if (!b) ADOBE_THROW_LAST_ERROR;
    //
    // Select out our bitmap.
    //
    SelectObject(memory_dc, bmp_old);
    DeleteObject(memory_bmp);
    DeleteDC(memory_dc);
    //
    // Return the created brush.
    //
    return b;
}

/****************************************************************************************************/

//
/// This function is the event handler for the panel widget. The panel is a
/// container which does it's best to be transparent. It will forward events
/// it recieves from it's child controls on to them using
/// forward_events_to_control.
//

LRESULT CALLBACK panel_window_procedure(HWND panel, UINT message, WPARAM wParam, LPARAM lParam)
{
    //
    // First lets see if one of our children can handle this message.
    //
    LRESULT forward_result(0);
    if (adobe::forward_message(message, wParam, lParam, forward_result))
    {
        return forward_result;
    }
    else if (adobe::metrics::using_styles() == false)
    {
        //
        // Only DIY if visual styles are on.
        //

        return ::DefWindowProc(panel, message, wParam, lParam);
    }
    else if (message == WM_PAINT)
    {
        //
        // We try to paint ourselves if visual styles are active. If we are
        // a panel on top of a tab control then we must make sure that we get
        // the tab background.
        //

        PAINTSTRUCT ps;
        HDC window_dc = BeginPaint(panel, &ps);
        adobe::metrics::draw_parent_background(panel, window_dc);
        EndPaint(panel, &ps);
        return 0;
    }
    else if (message == WM_CTLCOLORSTATIC)
    {
        //
        // REVISIT (ralpht): Don't do anything if the panel's parent isn't a tab control.
        //
		adobe::platform_display_type parent = adobe::get_parent_control(panel);
        if (parent) {
            std::vector<WCHAR> classname(1024);
            *(&classname[::GetClassNameW(parent, &classname[0], 1024)]) = 0;
            if (std::wstring(&classname[0]) != std::wstring(WC_TABCONTROL))
                return DefWindowProc(panel, message, wParam, lParam);
        }
        //
        // Usually we store the brush as user data inside the HWND.
        //
        HBRUSH b = hackery::cast<HBRUSH>(::GetWindowLongPtr(panel, GWLP_USERDATA));
        if (!b)
        {
            //
            // There is no brush. Either the brush was deleted by
            // the WM_SIZE handler or this is the first time a
            // brush has been requested. We need to create a new
            // brush and set it into the panel HWND.
            //
            b = make_background_brush(panel);
            ::SetWindowLongPtr(panel, GWLP_USERDATA, hackery::cast<LONG>(b));
        }
        //
        //
        // Set the child's DC drawing mode to transparent, and give
        // it an empty brush.
        //
        RECT child_dim; RECT panel_dim;
        HDC dc = (HDC)wParam;
        HWND child = (HWND)lParam;

        SetBkMode(dc, TRANSPARENT);
        GetWindowRect(child, &child_dim);
        GetWindowRect(panel, &panel_dim);
        SetBrushOrgEx(dc, -(child_dim.left - panel_dim.left),
            -(child_dim.top - panel_dim.top), 0);
        return (LRESULT)b;
    }
    else if (message == WM_CREATE)
    {
        //
        // Make sure that we don't use uninitialized memory as a brush
        // handle.
        //
        SetWindowLongPtr(panel, GWLP_USERDATA, 0);
    }
    else if (message == WM_SIZE || message == WM_DESTROY)
    {
        //
        // We store the brush which we give to children in the panel_t.
        // We only need this brush if visual styles are enabled -- the
        // brush contains the background bitmap which controls need to
        // draw behind themselves.
        //
        // We regenerate the background brush when a control requests it
        // (rather than when we resize, we don't want to make resizing
        // slow). In the size handler we just release the current brush
        // and set the brush to zero.
        //
        HBRUSH b = hackery::cast<HBRUSH>(::GetWindowLong(panel, GWLP_USERDATA));

        if (b)
            DeleteObject(b);
    }

    return DefWindowProc(panel, message, wParam, lParam);
}

/****************************************************************************************************/

} //end anonymous namespace 

#endif // ndef ADOBE_PLATFORM_WIDGETS_WT

/****************************************************************************************************/

namespace adobe {

/****************************************************************************************************/

panel_t::panel_t(const any_regular_t& show_value, theme_t theme) :
    control_m(0),
    theme_m(theme),
    show_value_m(show_value)
{
    static bool inited(false);

    if (!inited)
    {
        inited = true;

#ifndef ADOBE_PLATFORM_WIDGETS_WT
		// REVISIT (fbrereto) : init_once all this stuff
        INITCOMMONCONTROLSEX control_info = { sizeof(INITCOMMONCONTROLSEX), 0x0000FFFF };
        ::InitCommonControlsEx(&control_info);

        WNDCLASSW wc;

        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = panel_window_procedure;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = ::GetModuleHandle(NULL);
        wc.hIcon = NULL; // LoadIcon(wc.hInstance, MAKEINTRESOURCE(RES_APP_ICON));
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"eve_panel";

        RegisterClassW(&wc);
#endif
    }
}

/****************************************************************************************************/

void panel_t::display(const any_regular_t& value)
{
    bool visible (value == show_value_m);

    set_visible(visible);
}

/*************************************************************************************************/

void panel_t::measure(extents_t& result)
{ 
    result = extents_t(); 
}

/*************************************************************************************************/

void panel_t::place(const place_data_t& place_data)
{    
    implementation::set_control_bounds(control_m, place_data);
}

/*************************************************************************************************/

void panel_t::set_visible(bool make_visible)
{ 
    set_control_visible(control_m, make_visible); 
}

/****************************************************************************************************/

template <>
platform_display_type insert<panel_t>(display_t&             display,
                                             platform_display_type&  parent,
                                             panel_t&     element)
{
    assert(!element.control_m);
	element.control_m = implementation::make_panel (parent);

    return display.insert(parent, element.control_m);
}

/*************************************************************************************************/

} //namespace adobe

/*************************************************************************************************/


