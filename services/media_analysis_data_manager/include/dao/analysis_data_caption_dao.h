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

#ifndef OHOS_ANALYSIS_DATA_CAPTION_DAO_H
#define OHOS_ANALYSIS_DATA_CAPTION_DAO_H

#include <string>

namespace OHOS::Media::AnalysisData {
#define CONST_CAPTION_RESULT "caption"
#define CONST_CAPTION_VERSION "caption_version"
#define CONST_CAPTION_VECTOR "caption_vector"
#define CONST_CAPTION_ANALYSIS_VERSION "analysis_version"

class AnalysisDataCaptionDao {
private:
static inline const std::string RESET_TOTAL_CAPTION_BY_FILE_ID = "\
    UPDATE tab_analysis_total \
    SET \
        caption = 0 \
    WHERE \
        file_id = ? \
    AND \
        caption = 1;";

static inline const std::string DELETE_CAPTION_BY_FILE_ID = "\
    DELETE FROM tab_analysis_caption \
    WHERE \
        file_id = ?;";

public:
    AnalysisDataCaptionDao() = default;
    ~AnalysisDataCaptionDao() = default;

    static void FixCaptionAnalysisDataAfterEdit(const std::string &fileId);
};
} // namespace OHOS::Media::AnalysisData

#endif // OHOS_ANALYSIS_DATA_CAPTION_DAO_H