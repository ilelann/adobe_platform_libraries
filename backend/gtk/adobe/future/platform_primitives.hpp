#pragma once

/****************************************************************************************************/
// GTK forward declarations

typedef unsigned int guint;

struct _GtkWidget;
typedef struct _GtkWidget GtkWidget;
typedef unsigned long     gulong;

/****************************************************************************************************/

namespace adobe {

using platform_display_type = GtkWidget*;
using key_type              = guint;
} // namespace adobe
