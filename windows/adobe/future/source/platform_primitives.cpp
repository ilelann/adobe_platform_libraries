#include <adobe/future/platform_primitives.hpp>

#ifdef ADOBE_PLATFORM_WIDGETS_WT

WTabWidgetWithPredefinedNames::WTabWidgetWithPredefinedNames(std::vector<std::string> names)
: _names(names)
{
}

void WTabWidgetWithPredefinedNames::addTabAndNameIt(platform_display_type widget)
{
	addTab(widget, _names.at(count()));
}
	
#endif