#pragma once

/****************************************************************************************************/
// Forward declarations

typedef unsigned int    guint;

struct _GtkWidget;
typedef struct _GtkWidget              GtkWidget;

/****************************************************************************************************/


namespace adobe {
using platform_display_type = GtkWidget*;

using key_type = guint;
}
