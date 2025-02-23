// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "ppl/nn/oputils/onnx/reshape_roialign.h"
#include "ppl/nn/runtime/tensor_impl.h"
#include "ppl/nn/common/logger.h"
using namespace ppl::common;
using namespace ppl::nn::onnx;

namespace ppl { namespace nn { namespace onnx {

RetCode ReshapeROIAlign(InputOutputInfo* info, const void* arg) {
    if (info->GetInputCount() != 3 || info->GetOutputCount() != 1) {
        LOG(DEBUG) << "ERROR: input count[" << info->GetInputCount() << "] != 3 or output count["
                   << info->GetOutputCount() << "] != 1.";
        return RC_INVALID_VALUE;
    }

    auto param = (const RoiAlignParam*)arg;
    auto x = info->GetInput<TensorImpl>(0)->GetShape();
    auto rois = info->GetInput<TensorImpl>(1)->GetShape();
    auto batch_indices = info->GetInput<TensorImpl>(2)->GetShape();
    auto output = info->GetOutput<TensorImpl>(0)->GetShape();

    if (x->GetDimCount() != 4) {
        LOG(DEBUG) << "ERROR: input[0]'s dim count[" << x->GetDimCount() << "] != 4.";
        return RC_INVALID_VALUE;
    }
    if (rois->GetDimCount() != 2) {
        LOG(DEBUG) << "ERROR: input[1]'s dim count[" << rois->GetDimCount() << "] != 2.";
        return RC_INVALID_VALUE;
    }
    if (rois->GetDim(1) != 4) {
        LOG(DEBUG) << "ERROR: rois dim[1]'s value[" << rois->GetDim(1) << "] != 4.";
        return RC_INVALID_VALUE;
    }
    if (batch_indices->GetDimCount() != 1) {
        LOG(DEBUG) << "ERROR: batch_indices' dim count[" << batch_indices->GetDimCount() << "] != 1.";
        return RC_INVALID_VALUE;
    }
    const uint32_t num_rois = rois->GetDim(0);
    const uint32_t channels = x->GetDim(1);
    if (batch_indices->GetDim(0) != num_rois) {
        LOG(DEBUG) << "ERROR: batch_indices' dim[0]'s value[" << batch_indices->GetDim(0) << "] != num_rois["
                   << num_rois << "].";
        return RC_INVALID_VALUE;
    }

    output->Reshape({num_rois, channels, param->output_height, param->output_width});
    return RC_SUCCESS;
}

}}} // namespace ppl::nn::onnx
