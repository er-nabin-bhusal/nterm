#ifndef TEXTFORMATCONFIG_H
#define TEXTFORMATCONFIG_H

#include <QString>

namespace TextFormatConfig
{
    // Font sizes
    constexpr int HEADING_FONT_SIZE = 24;
    constexpr int NORMAL_FONT_SIZE = 14;
    constexpr int CODE_FONT_SIZE = 15;

    // Font families
    const QString DEFAULT_FONT_FAMILY = "Inter";
    const QString CODE_FONT_FAMILY = "Courier New";
    const QString HEADING_FONT_FAMILY = "Inter 24pt Black";

    // Colors
    const QString CODE_BLOCK_BACKGROUND = "#e0e0e0";
    const QString CODE_TEXT_COLOR = "#000000";

    // Margins
    constexpr int CODE_BLOCK_LEFT_MARGIN = 10;
    constexpr int CODE_BLOCK_RIGHT_MARGIN = 10;

    // Default text
    const QString DEFAULT_NOTE_TITLE = "Start with the Title...";
    const QString DEFAULT_CODE_BLOCK_TEXT = "Enter your code here...";
}

#endif // TEXTFORMATCONFIG_H