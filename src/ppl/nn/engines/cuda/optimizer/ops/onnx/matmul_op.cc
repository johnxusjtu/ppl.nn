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

#include "ppl/nn/engines/cuda/optimizer/ops/onnx/matmul_op.h"

#include "ppl/nn/common/logger.h"
#include "ppl/nn/engines/cuda/kernels/onnx/matmul_kernel.h"
#include "ppl/nn/oputils/onnx/reshape_matmul.h"

using namespace std;
using namespace ppl::common;
using namespace ppl::nn::onnx;

namespace ppl { namespace nn { namespace cuda {

RetCode MatMulOp::Init(const OptKernelOptions& options) {
    param_.param.num_output = 1;
    param_.param.bias_term = 0; // 0 or 1
    param_.param.alpha = 1;
    param_.param.beta = 1;
    param_.param.transA = 0;
    param_.param.transB = 0;
    param_.param.N = 1; // for converted mat B

    infer_type_func_ = [](InputOutputInfo* info, std::vector<CudaTensorQuant>* quant, datatype_t type) -> RetCode {
        type = ppl::common::DATATYPE_FLOAT16;
        ppl::common::RetCode status;
        if (type == DATATYPE_UNKNOWN) {
            status = InferInheritedType(info);
        } else if (type == DATATYPE_INT8) {
            status = CopyQuantType(info, quant);
        } else {
            status = InferDefaultType(info, type);
        }
        return status;
    };

    infer_dims_func_ = [](InputOutputInfo* info) -> RetCode {
        return onnx::ReshapeMatMul(info, nullptr);
    };

    return RC_SUCCESS;
}

RetCode MatMulOp::Finalize(const OptKernelOptions& options) {
    param_ = *((CudaGemmParam*)options.param);

    auto status = SetCommonParam(options);
    if (status != RC_SUCCESS) {
        LOG(ERROR) << "load common param failed: " << GetRetCodeStr(status);
        return status;
    }

    return RC_SUCCESS;
}

void MatMulOp::CopyParam(void*& param) {
    if (param == nullptr) {
        param = new CudaGemmParam();
    }
    *(CudaGemmParam*)param = param_;
    return;
}

KernelImpl* MatMulOp::CreateKernelImpl() const {
    return CreateKernelImplWithParam<MatMulKernel>(&param_);
}

}}} // namespace ppl::nn::cuda
