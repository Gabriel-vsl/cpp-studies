#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

class ImageProcessor
{
public:
    static QImage applyBrightnessContrast(const QImage &img, int brightness, double contrast);
};

#endif