/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_MEDIALIBRARY_BUNDLE_MANAGER_H
#define OHOS_MEDIALIBRARY_BUNDLE_MANAGER_H

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace OHOS {
namespace Media {
#define EXPORT __attribute__ ((visibility ("default")))
class MediaLibraryBundleManager {
public:
    EXPORT MediaLibraryBundleManager() = default;
    EXPORT ~MediaLibraryBundleManager() = default;
    EXPORT static std::shared_ptr<MediaLibraryBundleManager> GetInstance();
    EXPORT void GetBundleNameByTokenId(const int64_t tokenId, std::string &bundleName);
    EXPORT void GetBundleNameByUID(const int32_t uid, std::string &bundleName);
    EXPORT std::string GetClientBundleName();
    EXPORT void Clear();

private:

    // BundleMessage is the pair of bundleName and whether bundle is system app
    const static int CAPACITY = 50;
    std::list<std::pair<int32_t, std::string>> cacheList_;
    std::unordered_map<int32_t, std::list<std::pair<int32_t, std::string>>::iterator> cacheMap_;
    std::unordered_map<int64_t, std::string> tokenIdToBundleNameCache_;
    std::mutex uninstallMutex_;

    static std::once_flag oc_;
    static std::shared_ptr<MediaLibraryBundleManager> instance_;
};
} // Media
} // OHOS
#endif // OHOS_MEDIALIBRARY_BUNDLE_MANAGER_H
