#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

layout(location = 0) flat in highp uint in_nodeid;
layout(location = 1) in highp vec4 in_pos;

layout(early_fragment_tests) in;

layout(location = 0) out highp uint out_node_id;
layout(location = 1) out highp vec4 out_pos;

void main()
{
    out_node_id = in_nodeid;
    out_pos     = in_pos;
}