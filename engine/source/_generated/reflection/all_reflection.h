#pragma once
#include "runtime/core/meta/reflection/reflection.h"
#include "_generated/serializer/all_serializer.h"
#include "_generated/reflection/level.reflection.gen.h"
#include "_generated/reflection/quaternion.reflection.gen.h"
#include "_generated/reflection/transform.reflection.gen.h"
#include "_generated/reflection/world.reflection.gen.h"
#include "_generated/reflection/vector3.reflection.gen.h"
#include "_generated/reflection/matrix4.reflection.gen.h"
#include "_generated/reflection/vector2.reflection.gen.h"
#include "_generated/reflection/motor_component.reflection.gen.h"
#include "_generated/reflection/color.reflection.gen.h"
#include "_generated/reflection/object.reflection.gen.h"
#include "_generated/reflection/vector4.reflection.gen.h"
#include "_generated/reflection/axis_aligned.reflection.gen.h"
#include "_generated/reflection/component.reflection.gen.h"
#include "_generated/reflection/render_object.reflection.gen.h"
#include "_generated/reflection/camera_component.reflection.gen.h"
#include "_generated/reflection/mesh.reflection.gen.h"
#include "_generated/reflection/mesh_component.reflection.gen.h"
#include "_generated/reflection/camera_config.reflection.gen.h"
#include "_generated/reflection/transform_component.reflection.gen.h"
#include "_generated/reflection/material.reflection.gen.h"
#include "_generated/reflection/global_rendering.reflection.gen.h"

namespace VKernel{
namespace Reflection{
    void TypeMetaRegister::metaRegister(){
        TypeWrappersRegister::Level();
        TypeWrappersRegister::Quaternion();
        TypeWrappersRegister::Transform();
        TypeWrappersRegister::World();
        TypeWrappersRegister::Vector3();
        TypeWrappersRegister::Matrix4();
        TypeWrappersRegister::Vector2();
        TypeWrappersRegister::MotorComponent();
        TypeWrappersRegister::Color();
        TypeWrappersRegister::Object();
        TypeWrappersRegister::Vector4();
        TypeWrappersRegister::AxisAligned();
        TypeWrappersRegister::Component();
        TypeWrappersRegister::RenderObject();
        TypeWrappersRegister::CameraComponent();
        TypeWrappersRegister::Mesh();
        TypeWrappersRegister::MeshComponent();
        TypeWrappersRegister::CameraConfig();
        TypeWrappersRegister::TransformComponent();
        TypeWrappersRegister::Material();
        TypeWrappersRegister::GlobalRendering();
    }
}
}

