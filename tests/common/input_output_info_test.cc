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

#include "ppl/nn/common/input_output_info.h"
#include "tests/ir/graph_builder.h"
#include "gtest/gtest.h"
#include <vector>
using namespace std;
using namespace ppl::nn;
using namespace ppl::nn::test;
using namespace ppl::common;

class InputOutputInfoTest : public testing::Test {
protected:
    void SetUp() override {
        builder_.AddNode("a", ir::Node::Type("test", "op1", 1), {"input_of_a"}, {"output_of_a"});
        builder_.AddNode("b", ir::Node::Type("test", "op2", 1), {"output_of_a"}, {"output_of_b"});
        builder_.AddNode("c", ir::Node::Type("test", "op1", 1), {"output_of_b"}, {"output_of_c"});
        builder_.Finalize();

        auto topo = builder_.GetGraph()->topo.get();
        edge_objects_.reserve(topo->GetMaxEdgeId());
        for (edgeid_t i = 0; i < topo->GetMaxEdgeId(); ++i) {
            auto edge = topo->GetEdge(i);
            if (!edge) {
                continue;
            }
            edge_objects_.emplace_back(EdgeObject(edge, EdgeObject::T_EDGE_OBJECT));
        }
    }

protected:
    GraphBuilder builder_;
    vector<EdgeObject> edge_objects_;
};

TEST_F(InputOutputInfoTest, misc) {
    auto topo = builder_.GetGraph()->topo.get();

    auto node = topo->GetNode(0);
    EXPECT_EQ("a", node->GetName());

    InputOutputInfo info;
    info.SetNode(node);
    info.SetAcquireFunc([this](edgeid_t eid, uint32_t) -> EdgeObject* {
        return &edge_objects_[eid];
    });

    auto edge = topo->GetEdge("input_of_a");
    EXPECT_NE(nullptr, edge);
    EXPECT_EQ(&edge_objects_[0], info.GetInput<EdgeObject>(0));

    edge = topo->GetEdge("output_of_a");
    EXPECT_NE(nullptr, edge);
    EXPECT_EQ(&edge_objects_[1], info.GetOutput<EdgeObject>(0));
}
