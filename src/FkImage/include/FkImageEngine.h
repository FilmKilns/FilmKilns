/*
* Copyright (c) 2018-present, filmkilns(email: filmkilns@outlook.com, github: https://github.com/filmkilns).
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_IMAGE_FKIMAGEENGINE_H
#define FK_IMAGE_FKIMAGEENGINE_H

#include "FkObject.h"
#include "FkLayerEngine.h"
#include "FkWorkspace.h"

FK_SUPER_CLASS(FkImageEngine, FkLayerEngine) {
FK_DEF_CLASS_TYPE_FUNC(FkImageEngine)

public:
    FkImageEngine(std::shared_ptr<FkEngine> &renderEngine, std::string &workspace, std::string name);

    FkImageEngine(const FkImageEngine &o) = delete;

    virtual ~FkImageEngine();

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkID newLayerWithFile(std::string path, FkID expectId = FK_ID_NONE);

    FkID save(FkID layerId, std::string file, FkResultCallback callback = nullptr);

protected:

    virtual FkResult _updateLayerWithSource(const std::shared_ptr<FkMessage> &msg) override;

private:
    FkResult _updateLayerWithFile(std::shared_ptr<FkMessage> &msg);

    FkResult _save(std::shared_ptr<FkMessage> &msg);

private:
    FkWorkspace workspace;
};

#endif //FK_IMAGE_FKIMAGEENGINE_H