#pragma once
#include "runtime/core/meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated/reflection/camera_component.reflection.gen.h"
#include "_generated/reflection/component.reflection.gen.h"

namespace VKernel{
namespace Reflection{
    void TypeMetaRegister::metaRegister(){
        TypeWrappersRegister::CameraComponent();
        TypeWrappersRegister::Component();
    }
}
}

