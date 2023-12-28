#pragma once

#include <Core/Id.h>

namespace Core {
namespace Internal {

// Run the settings dialog and wait for it to finish.
// Returns if the changes have been applied.
bool executeSettingsDialog(QWidget *parent, Id initialPage);

} // namespace Internal
} // namespace Core
