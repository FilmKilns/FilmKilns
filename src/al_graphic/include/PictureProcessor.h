//
// Created by mingyi.li on 2018/12/25.
//

#ifndef HARDWAREVIDEOCODEC_PICTUREPROCESSOR_H
#define HARDWAREVIDEOCODEC_PICTUREPROCESSOR_H

#include "Object.h"
#include "UnitPipeline.h"
#include "HwAbsFilter.h"
#include "HwWindow.h"

class PictureProcessor : public Object {
public:
    PictureProcessor();

    virtual ~PictureProcessor();

    void prepare(HwWindow *win);

    void updateWindow(HwWindow *win);

    void show(const string file);

    void setFilter(HwAbsFilter *filter);

    void invalidate();

private:
    const string ALIAS_OF_IMAGE = "IMAGE";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    UnitPipeline *pipeline = nullptr;
};


#endif //HARDWAREVIDEOCODEC_PICTUREPROCESSOR_H
