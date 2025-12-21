#pragma once
#include "runtime/core/meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated/reflection/quaternion.reflection.gen.h"
#include "_generated/reflection/transform.reflection.gen.h"
#include "_generated/reflection/vector3.reflection.gen.h"
#include "_generated/reflection/vector4.reflection.gen.h"
#include "_generated/reflection/axis_aligned.reflection.gen.h"
#include "_generated/reflection/matrix4.reflection.gen.h"
#include "_generated/reflection/vector2.reflection.gen.h"
#include "_generated/reflection/component.reflection.gen.h"
#include "_generated/reflection/camera_component.reflection.gen.h"
#include "_generated/reflection/camera_config.reflection.gen.h"
#include "_generated/reflection/global_rendering.reflection.gen.h"

namespace VKernel{
namespace Reflection{
    void TypeMetaRegister::metaRegister(){
        TypeWrappersRegister::Quaternion();
        TypeWrappersRegister::Transform();
        TypeWrappersRegister::Vector3();
        TypeWrappersRegister::Vector4();
        TypeWrappersRegister::AxisAligned();
        TypeWrappersRegister::Matrix4();
        TypeWrappersRegister::Vector2();
        TypeWrappersRegister::Component();
        TypeWrappersRegister::CameraComponent();
        TypeWrappersRegister::CameraConfig();
        TypeWrappersRegister::GlobalRendering();
    }
}
}

