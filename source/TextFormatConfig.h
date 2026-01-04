#ifndef TEXTFORMATCONFIG_H
#define TEXTFORMATCONFIG_H

#include <QString>

namespace TextFormatConfig
{
    // Font sizes
    constexpr int HEADING_FONT_SIZE = 24;
    constexpr int NORMAL_FONT_SIZE = 14;

    // Font families
    const QString DEFAULT_FONT_FAMILY = "Inter";
    const QString HEADING_FONT_FAMILY = "Inter 24pt Black";

    // Default text
    const QString DEFAULT_NOTE_TITLE = "Start with the Title...";
}

#endif // TEXTFORMATCONFIG_H