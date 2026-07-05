/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include "analysis_data_caption_dao.h"
#include "medialibrary_unistore_manager.h"
#include "media_log.h"

namespace OHOS::Media::AnalysisData {
void AnalysisDataCaptionDao::FixCaptionAnalysisDataAfterEdit(const std::string &fileId)
{
    auto rdbStore = MediaLibraryUnistoreManager::GetInstance().GetRdbStore();
    CHECK_AND_RETURN_LOG(rdbStore != nullptr, "Fix caption analysis data failed, rdbStore is null.");
    CHECK_AND_RETURN_LOG(!fileId.empty(), "Fix caption analysis data failed, fileId is empty");

    const std::vector<NativeRdb::ValueObject> bindArgs = {fileId};

    std::string totalCaptionSql = RESET_TOTAL_CAPTION_BY_FILE_ID;
    int64_t changedRowCount = 0;
    auto totalErrCode = rdbStore->ExecuteForChangedRowCount(changedRowCount, totalCaptionSql, bindArgs);
    CHECK_AND_RETURN_LOG(totalErrCode == NativeRdb::E_OK,
        "Failed to reset tab_analysis_total caption status, fileId: %{public}s", fileId.c_str());

    CHECK_AND_RETURN_INFO_LOG(changedRowCount > 0, "No caption data need to clear");

    std::string captionSql = DELETE_CAPTION_BY_FILE_ID;
    auto cropErrCode = rdbStore->ExecuteSql(captionSql, bindArgs);
    CHECK_AND_RETURN_LOG(cropErrCode == NativeRdb::E_OK,
        "Failed to clear tab_analysis_caption data, fileId: %{public}s", fileId.c_str());

    MEDIA_DEBUG_LOG("Fix caption analysis data success, fileId: %{public}s", fileId.c_str());
}
} // namespace OHOS::Media::AnalysisData