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

#define MLOG_TAG "BundleManager"

#include "medialibrary_bundle_manager.h"

#include <memory>
#include <mutex>

#include "ipc_skeleton.h"
#include "permission_utils.h"

using namespace std;

namespace OHOS {
namespace Media {

std::once_flag MediaLibraryBundleManager::oc_;
shared_ptr<MediaLibraryBundleManager> MediaLibraryBundleManager::instance_ = nullptr;
shared_ptr<MediaLibraryBundleManager> MediaLibraryBundleManager::GetInstance()
{
    std::call_once(oc_, []() {
        instance_ = std::make_shared<MediaLibraryBundleManager>();
    });
    return instance_;
}

void MediaLibraryBundleManager::GetBundleNameByUID(const int32_t uid, string &bundleName)
{
    PermissionUtils::GetClientBundle(uid, bundleName);
    if (bundleName.empty()) {
        return;
    }

    auto it = cacheMap_.find(uid);
    if (it != cacheMap_.end()) {
        cacheList_.erase(it->second);
    }
    cacheList_.push_front(make_pair(uid, bundleName));
    cacheMap_[uid] = cacheList_.begin();
    if (cacheMap_.size() > CAPACITY) {
        int32_t deleteKey = cacheList_.back().first;
        cacheMap_.erase(deleteKey);
        cacheList_.pop_back();
    }
}

void MediaLibraryBundleManager::GetBundleNameByTokenId(const int64_t tokenId, string &bundleName)
{
    auto it = tokenIdToBundleNameCache_.find(tokenId);
    if (it != tokenIdToBundleNameCache_.end()) {
        bundleName = it->second;
        return;
    }
    bundleName = GetClientBundleName();
    tokenIdToBundleNameCache_[tokenId] = bundleName;
}

/**
 * if it is called by SA, bundlename is null. we should not log everytime
 */
std::string MediaLibraryBundleManager::GetClientBundleName()
{
    lock_guard<mutex> lock(uninstallMutex_);
    int32_t uid = IPCSkeleton::GetCallingUid();
    auto iter = cacheMap_.find(uid);
    if (iter == cacheMap_.end()) {
        string bundleName;
        GetBundleNameByUID(uid, bundleName);
        return bundleName;
    }
    cacheList_.splice(cacheList_.begin(), cacheList_, iter->second);
    return iter->second->second;
}

void MediaLibraryBundleManager::Clear()
{
    lock_guard<mutex> lock(uninstallMutex_);
    cacheList_.clear();
    cacheMap_.clear();
}
} // Media
} // OHOS
