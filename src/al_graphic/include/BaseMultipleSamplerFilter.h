/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_BASEMULTIPLESAMPLERFILTER_H
#define HARDWAREVIDEOCODEC_BASEMULTIPLESAMPLERFILTER_H

#include "Filter.h"
#include "AlPngDecoder.h"
#include <string>

using namespace std;

class BaseMultipleSamplerFilter : public Filter {
public:
    BaseMultipleSamplerFilter(char **names, char **samplers, int size);

    virtual ~BaseMultipleSamplerFilter();

    virtual bool init(int w, int h) override;

    void bindResources() override;

    virtual BaseDrawer *getDrawer()=0;

    void releaseStringArray(char **array, int size);

private:
    char **names = nullptr;
    char **samplers = nullptr;
    int size = 0;
    GLuint *textures = nullptr;
    GLint *textureLocations = nullptr;

    GLuint loadTexture(string path);
};


#endif //HARDWAREVIDEOCODEC_BASEMULTIPLESAMPLERFILTER_H
