#define MandatoryCitation(ClassName, _s_registered) volatile bool dummy_##ClassName = Games::ClassName::##_s_registered;

#include "runtime/Games/the_celestial_console/control_cabin.h"
MandatoryCitation(ControlCabin, _s_registered);
#include "runtime/Games/the_celestial_console/Panels/main_panel.h"
MandatoryCitation(MainPanel, _s_registered);
#include "runtime/Games/the_celestial_console/Panels/control_panel.h"
MandatoryCitation(ControPanel, _s_registered);