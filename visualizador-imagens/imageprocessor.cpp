#include "imageprocessor.h"
#include <QColor>
#include <algorithm>

QImage ImageProcessor::applyBrightnessContrast(const QImage &img, int brightness, double contrast)
{
    QImage result = img.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < result.height(); y++)
    {
        for (int x = 0; x < result.width(); x++)
        {
            QColor c = QColor::fromRgb(result.pixel(x, y));

            int r = std::clamp(int(contrast * c.red() + brightness), 0, 255);
            int g = std::clamp(int(contrast * c.green() + brightness), 0, 255);
            int b = std::clamp(int(contrast * c.blue() + brightness), 0, 255);

            result.setPixel(x, y, qRgb(r, g, b));
        }
    }

    return result;
}