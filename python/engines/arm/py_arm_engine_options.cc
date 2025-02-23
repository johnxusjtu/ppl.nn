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

#include "ppl/nn/engines/arm/engine_options.h"
#include "pybind11/pybind11.h"

namespace ppl { namespace nn { namespace python {

void RegisterArmEngineOptions(pybind11::module* m) {
    pybind11::class_<arm::EngineOptions>(*m, "EngineOptions")
        .def(pybind11::init<>())
        .def_readwrite("mm_policy", &arm::EngineOptions::mm_policy)
        .def_readwrite("forward_precision", &arm::EngineOptions::forward_precision)
        .def_readwrite("graph_optimization_level", &arm::EngineOptions::graph_optimization_level)
        .def_readwrite("winograd_level", &arm::EngineOptions::winograd_level)
        .def_readwrite("dynamic_tuning_level", &arm::EngineOptions::dynamic_tuning_level)
        .def_readwrite("numa_node_id", &arm::EngineOptions::numa_node_id);

    m->attr("MM_COMPACT") = (uint32_t)arm::MM_COMPACT;
    m->attr("MM_MRU") = (uint32_t)arm::MM_MRU;

    m->attr("OPT_DISABLE_ALL") = (uint32_t)arm::OPT_DISABLE_ALL;
    m->attr("OPT_ENABLE_BASIC") = (uint32_t)arm::OPT_ENABLE_BASIC;
    m->attr("OPT_ENABLE_EXTENDED") = (uint32_t)arm::OPT_ENABLE_EXTENDED;
    m->attr("OPT_ENABLE_ALL") = (uint32_t)arm::OPT_ENABLE_ALL;

    m->attr("WG_OFF") = (uint32_t)arm::WG_OFF;
    m->attr("WG_ON") = (uint32_t)arm::WG_ON;
    m->attr("WG_ON_B2") = (uint32_t)arm::WG_ON_B2;
    m->attr("WG_ON_B4") = (uint32_t)arm::WG_ON_B4;

    m->attr("TUNING_OFF") = (uint32_t)arm::TUNING_OFF;
    m->attr("TUNING_SELECT_ALGO") = (uint32_t)arm::TUNING_SELECT_ALGO;
    m->attr("TUNING_SELECT_BLK_SIZE") = (uint32_t)arm::TUNING_SELECT_BLK_SIZE;
}

}}} // namespace ppl::nn::python
