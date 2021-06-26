QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    glm/detail/glm.cpp \
    globject.cpp \
    main.cpp \
    mainwindow.cpp \
    shader.cpp \
    square.cpp \
    squarecase.cpp \
    squarelightsource.cpp \
    stb_image.cpp

HEADERS += \
    camera.h \
    glm/common.hpp \
    glm/detail/_features.hpp \
    glm/detail/_fixes.hpp \
    glm/detail/_noise.hpp \
    glm/detail/_swizzle.hpp \
    glm/detail/_swizzle_func.hpp \
    glm/detail/_vectorize.hpp \
    glm/detail/compute_common.hpp \
    glm/detail/compute_vector_relational.hpp \
    glm/detail/func_common.inl \
    glm/detail/func_common_simd.inl \
    glm/detail/func_exponential.inl \
    glm/detail/func_exponential_simd.inl \
    glm/detail/func_geometric.inl \
    glm/detail/func_geometric_simd.inl \
    glm/detail/func_integer.inl \
    glm/detail/func_integer_simd.inl \
    glm/detail/func_matrix.inl \
    glm/detail/func_matrix_simd.inl \
    glm/detail/func_packing.inl \
    glm/detail/func_packing_simd.inl \
    glm/detail/func_trigonometric.inl \
    glm/detail/func_trigonometric_simd.inl \
    glm/detail/func_vector_relational.inl \
    glm/detail/func_vector_relational_simd.inl \
    glm/detail/qualifier.hpp \
    glm/detail/setup.hpp \
    glm/detail/type_float.hpp \
    glm/detail/type_half.hpp \
    glm/detail/type_half.inl \
    glm/detail/type_mat2x2.hpp \
    glm/detail/type_mat2x2.inl \
    glm/detail/type_mat2x3.hpp \
    glm/detail/type_mat2x3.inl \
    glm/detail/type_mat2x4.hpp \
    glm/detail/type_mat2x4.inl \
    glm/detail/type_mat3x2.hpp \
    glm/detail/type_mat3x2.inl \
    glm/detail/type_mat3x3.hpp \
    glm/detail/type_mat3x3.inl \
    glm/detail/type_mat3x4.hpp \
    glm/detail/type_mat3x4.inl \
    glm/detail/type_mat4x2.hpp \
    glm/detail/type_mat4x2.inl \
    glm/detail/type_mat4x3.hpp \
    glm/detail/type_mat4x3.inl \
    glm/detail/type_mat4x4.hpp \
    glm/detail/type_mat4x4.inl \
    glm/detail/type_mat4x4_simd.inl \
    glm/detail/type_quat.hpp \
    glm/detail/type_quat.inl \
    glm/detail/type_quat_simd.inl \
    glm/detail/type_vec1.hpp \
    glm/detail/type_vec1.inl \
    glm/detail/type_vec2.hpp \
    glm/detail/type_vec2.inl \
    glm/detail/type_vec3.hpp \
    glm/detail/type_vec3.inl \
    glm/detail/type_vec4.hpp \
    glm/detail/type_vec4.inl \
    glm/detail/type_vec4_simd.inl \
    glm/exponential.hpp \
    glm/ext.hpp \
    glm/ext/matrix_clip_space.hpp \
    glm/ext/matrix_clip_space.inl \
    glm/ext/matrix_common.hpp \
    glm/ext/matrix_common.inl \
    glm/ext/matrix_double2x2.hpp \
    glm/ext/matrix_double2x2_precision.hpp \
    glm/ext/matrix_double2x3.hpp \
    glm/ext/matrix_double2x3_precision.hpp \
    glm/ext/matrix_double2x4.hpp \
    glm/ext/matrix_double2x4_precision.hpp \
    glm/ext/matrix_double3x2.hpp \
    glm/ext/matrix_double3x2_precision.hpp \
    glm/ext/matrix_double3x3.hpp \
    glm/ext/matrix_double3x3_precision.hpp \
    glm/ext/matrix_double3x4.hpp \
    glm/ext/matrix_double3x4_precision.hpp \
    glm/ext/matrix_double4x2.hpp \
    glm/ext/matrix_double4x2_precision.hpp \
    glm/ext/matrix_double4x3.hpp \
    glm/ext/matrix_double4x3_precision.hpp \
    glm/ext/matrix_double4x4.hpp \
    glm/ext/matrix_double4x4_precision.hpp \
    glm/ext/matrix_float2x2.hpp \
    glm/ext/matrix_float2x2_precision.hpp \
    glm/ext/matrix_float2x3.hpp \
    glm/ext/matrix_float2x3_precision.hpp \
    glm/ext/matrix_float2x4.hpp \
    glm/ext/matrix_float2x4_precision.hpp \
    glm/ext/matrix_float3x2.hpp \
    glm/ext/matrix_float3x2_precision.hpp \
    glm/ext/matrix_float3x3.hpp \
    glm/ext/matrix_float3x3_precision.hpp \
    glm/ext/matrix_float3x4.hpp \
    glm/ext/matrix_float3x4_precision.hpp \
    glm/ext/matrix_float4x2.hpp \
    glm/ext/matrix_float4x2_precision.hpp \
    glm/ext/matrix_float4x3.hpp \
    glm/ext/matrix_float4x3_precision.hpp \
    glm/ext/matrix_float4x4.hpp \
    glm/ext/matrix_float4x4_precision.hpp \
    glm/ext/matrix_int2x2.hpp \
    glm/ext/matrix_int2x2_sized.hpp \
    glm/ext/matrix_int2x3.hpp \
    glm/ext/matrix_int2x3_sized.hpp \
    glm/ext/matrix_int2x4.hpp \
    glm/ext/matrix_int2x4_sized.hpp \
    glm/ext/matrix_int3x2.hpp \
    glm/ext/matrix_int3x2_sized.hpp \
    glm/ext/matrix_int3x3.hpp \
    glm/ext/matrix_int3x3_sized.hpp \
    glm/ext/matrix_int3x4.hpp \
    glm/ext/matrix_int3x4_sized.hpp \
    glm/ext/matrix_int4x2.hpp \
    glm/ext/matrix_int4x2_sized.hpp \
    glm/ext/matrix_int4x3.hpp \
    glm/ext/matrix_int4x3_sized.hpp \
    glm/ext/matrix_int4x4.hpp \
    glm/ext/matrix_int4x4_sized.hpp \
    glm/ext/matrix_integer.hpp \
    glm/ext/matrix_integer.inl \
    glm/ext/matrix_projection.hpp \
    glm/ext/matrix_projection.inl \
    glm/ext/matrix_relational.hpp \
    glm/ext/matrix_relational.inl \
    glm/ext/matrix_transform.hpp \
    glm/ext/matrix_transform.inl \
    glm/ext/matrix_uint2x2.hpp \
    glm/ext/matrix_uint2x2_sized.hpp \
    glm/ext/matrix_uint2x3.hpp \
    glm/ext/matrix_uint2x3_sized.hpp \
    glm/ext/matrix_uint2x4.hpp \
    glm/ext/matrix_uint2x4_sized.hpp \
    glm/ext/matrix_uint3x2.hpp \
    glm/ext/matrix_uint3x2_sized.hpp \
    glm/ext/matrix_uint3x3.hpp \
    glm/ext/matrix_uint3x3_sized.hpp \
    glm/ext/matrix_uint3x4.hpp \
    glm/ext/matrix_uint3x4_sized.hpp \
    glm/ext/matrix_uint4x2.hpp \
    glm/ext/matrix_uint4x2_sized.hpp \
    glm/ext/matrix_uint4x3.hpp \
    glm/ext/matrix_uint4x3_sized.hpp \
    glm/ext/matrix_uint4x4.hpp \
    glm/ext/matrix_uint4x4_sized.hpp \
    glm/ext/quaternion_common.hpp \
    glm/ext/quaternion_common.inl \
    glm/ext/quaternion_common_simd.inl \
    glm/ext/quaternion_double.hpp \
    glm/ext/quaternion_double_precision.hpp \
    glm/ext/quaternion_exponential.hpp \
    glm/ext/quaternion_exponential.inl \
    glm/ext/quaternion_float.hpp \
    glm/ext/quaternion_float_precision.hpp \
    glm/ext/quaternion_geometric.hpp \
    glm/ext/quaternion_geometric.inl \
    glm/ext/quaternion_relational.hpp \
    glm/ext/quaternion_relational.inl \
    glm/ext/quaternion_transform.hpp \
    glm/ext/quaternion_transform.inl \
    glm/ext/quaternion_trigonometric.hpp \
    glm/ext/quaternion_trigonometric.inl \
    glm/ext/scalar_common.hpp \
    glm/ext/scalar_common.inl \
    glm/ext/scalar_constants.hpp \
    glm/ext/scalar_constants.inl \
    glm/ext/scalar_int_sized.hpp \
    glm/ext/scalar_integer.hpp \
    glm/ext/scalar_integer.inl \
    glm/ext/scalar_packing.hpp \
    glm/ext/scalar_packing.inl \
    glm/ext/scalar_reciprocal.hpp \
    glm/ext/scalar_reciprocal.inl \
    glm/ext/scalar_relational.hpp \
    glm/ext/scalar_relational.inl \
    glm/ext/scalar_uint_sized.hpp \
    glm/ext/scalar_ulp.hpp \
    glm/ext/scalar_ulp.inl \
    glm/ext/vector_bool1.hpp \
    glm/ext/vector_bool1_precision.hpp \
    glm/ext/vector_bool2.hpp \
    glm/ext/vector_bool2_precision.hpp \
    glm/ext/vector_bool3.hpp \
    glm/ext/vector_bool3_precision.hpp \
    glm/ext/vector_bool4.hpp \
    glm/ext/vector_bool4_precision.hpp \
    glm/ext/vector_common.hpp \
    glm/ext/vector_common.inl \
    glm/ext/vector_double1.hpp \
    glm/ext/vector_double1_precision.hpp \
    glm/ext/vector_double2.hpp \
    glm/ext/vector_double2_precision.hpp \
    glm/ext/vector_double3.hpp \
    glm/ext/vector_double3_precision.hpp \
    glm/ext/vector_double4.hpp \
    glm/ext/vector_double4_precision.hpp \
    glm/ext/vector_float1.hpp \
    glm/ext/vector_float1_precision.hpp \
    glm/ext/vector_float2.hpp \
    glm/ext/vector_float2_precision.hpp \
    glm/ext/vector_float3.hpp \
    glm/ext/vector_float3_precision.hpp \
    glm/ext/vector_float4.hpp \
    glm/ext/vector_float4_precision.hpp \
    glm/ext/vector_int1.hpp \
    glm/ext/vector_int1_sized.hpp \
    glm/ext/vector_int2.hpp \
    glm/ext/vector_int2_sized.hpp \
    glm/ext/vector_int3.hpp \
    glm/ext/vector_int3_sized.hpp \
    glm/ext/vector_int4.hpp \
    glm/ext/vector_int4_sized.hpp \
    glm/ext/vector_integer.hpp \
    glm/ext/vector_integer.inl \
    glm/ext/vector_packing.hpp \
    glm/ext/vector_packing.inl \
    glm/ext/vector_reciprocal.hpp \
    glm/ext/vector_reciprocal.inl \
    glm/ext/vector_relational.hpp \
    glm/ext/vector_relational.inl \
    glm/ext/vector_uint1.hpp \
    glm/ext/vector_uint1_sized.hpp \
    glm/ext/vector_uint2.hpp \
    glm/ext/vector_uint2_sized.hpp \
    glm/ext/vector_uint3.hpp \
    glm/ext/vector_uint3_sized.hpp \
    glm/ext/vector_uint4.hpp \
    glm/ext/vector_uint4_sized.hpp \
    glm/ext/vector_ulp.hpp \
    glm/ext/vector_ulp.inl \
    glm/fwd.hpp \
    glm/geometric.hpp \
    glm/glm.hpp \
    glm/gtc/bitfield.hpp \
    glm/gtc/bitfield.inl \
    glm/gtc/color_space.hpp \
    glm/gtc/color_space.inl \
    glm/gtc/constants.hpp \
    glm/gtc/constants.inl \
    glm/gtc/epsilon.hpp \
    glm/gtc/epsilon.inl \
    glm/gtc/integer.hpp \
    glm/gtc/integer.inl \
    glm/gtc/matrix_access.hpp \
    glm/gtc/matrix_access.inl \
    glm/gtc/matrix_integer.hpp \
    glm/gtc/matrix_inverse.hpp \
    glm/gtc/matrix_inverse.inl \
    glm/gtc/matrix_transform.hpp \
    glm/gtc/matrix_transform.inl \
    glm/gtc/noise.hpp \
    glm/gtc/noise.inl \
    glm/gtc/packing.hpp \
    glm/gtc/packing.inl \
    glm/gtc/quaternion.hpp \
    glm/gtc/quaternion.inl \
    glm/gtc/quaternion_simd.inl \
    glm/gtc/random.hpp \
    glm/gtc/random.inl \
    glm/gtc/reciprocal.hpp \
    glm/gtc/round.hpp \
    glm/gtc/round.inl \
    glm/gtc/type_aligned.hpp \
    glm/gtc/type_precision.hpp \
    glm/gtc/type_precision.inl \
    glm/gtc/type_ptr.hpp \
    glm/gtc/type_ptr.inl \
    glm/gtc/ulp.hpp \
    glm/gtc/ulp.inl \
    glm/gtc/vec1.hpp \
    glm/gtx/associated_min_max.hpp \
    glm/gtx/associated_min_max.inl \
    glm/gtx/bit.hpp \
    glm/gtx/bit.inl \
    glm/gtx/closest_point.hpp \
    glm/gtx/closest_point.inl \
    glm/gtx/color_encoding.hpp \
    glm/gtx/color_encoding.inl \
    glm/gtx/color_space.hpp \
    glm/gtx/color_space.inl \
    glm/gtx/color_space_YCoCg.hpp \
    glm/gtx/color_space_YCoCg.inl \
    glm/gtx/common.hpp \
    glm/gtx/common.inl \
    glm/gtx/compatibility.hpp \
    glm/gtx/compatibility.inl \
    glm/gtx/component_wise.hpp \
    glm/gtx/component_wise.inl \
    glm/gtx/dual_quaternion.hpp \
    glm/gtx/dual_quaternion.inl \
    glm/gtx/easing.hpp \
    glm/gtx/easing.inl \
    glm/gtx/euler_angles.hpp \
    glm/gtx/euler_angles.inl \
    glm/gtx/extend.hpp \
    glm/gtx/extend.inl \
    glm/gtx/extended_min_max.hpp \
    glm/gtx/extended_min_max.inl \
    glm/gtx/exterior_product.hpp \
    glm/gtx/exterior_product.inl \
    glm/gtx/fast_exponential.hpp \
    glm/gtx/fast_exponential.inl \
    glm/gtx/fast_square_root.hpp \
    glm/gtx/fast_square_root.inl \
    glm/gtx/fast_trigonometry.hpp \
    glm/gtx/fast_trigonometry.inl \
    glm/gtx/float_notmalize.inl \
    glm/gtx/functions.hpp \
    glm/gtx/functions.inl \
    glm/gtx/gradient_paint.hpp \
    glm/gtx/gradient_paint.inl \
    glm/gtx/handed_coordinate_space.hpp \
    glm/gtx/handed_coordinate_space.inl \
    glm/gtx/hash.hpp \
    glm/gtx/hash.inl \
    glm/gtx/integer.hpp \
    glm/gtx/integer.inl \
    glm/gtx/intersect.hpp \
    glm/gtx/intersect.inl \
    glm/gtx/io.hpp \
    glm/gtx/io.inl \
    glm/gtx/log_base.hpp \
    glm/gtx/log_base.inl \
    glm/gtx/matrix_cross_product.hpp \
    glm/gtx/matrix_cross_product.inl \
    glm/gtx/matrix_decompose.hpp \
    glm/gtx/matrix_decompose.inl \
    glm/gtx/matrix_factorisation.hpp \
    glm/gtx/matrix_factorisation.inl \
    glm/gtx/matrix_interpolation.hpp \
    glm/gtx/matrix_interpolation.inl \
    glm/gtx/matrix_major_storage.hpp \
    glm/gtx/matrix_major_storage.inl \
    glm/gtx/matrix_operation.hpp \
    glm/gtx/matrix_operation.inl \
    glm/gtx/matrix_query.hpp \
    glm/gtx/matrix_query.inl \
    glm/gtx/matrix_transform_2d.hpp \
    glm/gtx/matrix_transform_2d.inl \
    glm/gtx/mixed_product.hpp \
    glm/gtx/mixed_product.inl \
    glm/gtx/norm.hpp \
    glm/gtx/norm.inl \
    glm/gtx/normal.hpp \
    glm/gtx/normal.inl \
    glm/gtx/normalize_dot.hpp \
    glm/gtx/normalize_dot.inl \
    glm/gtx/number_precision.hpp \
    glm/gtx/number_precision.inl \
    glm/gtx/optimum_pow.hpp \
    glm/gtx/optimum_pow.inl \
    glm/gtx/orthonormalize.hpp \
    glm/gtx/orthonormalize.inl \
    glm/gtx/pca.hpp \
    glm/gtx/pca.inl \
    glm/gtx/perpendicular.hpp \
    glm/gtx/perpendicular.inl \
    glm/gtx/polar_coordinates.hpp \
    glm/gtx/polar_coordinates.inl \
    glm/gtx/projection.hpp \
    glm/gtx/projection.inl \
    glm/gtx/quaternion.hpp \
    glm/gtx/quaternion.inl \
    glm/gtx/range.hpp \
    glm/gtx/raw_data.hpp \
    glm/gtx/raw_data.inl \
    glm/gtx/rotate_normalized_axis.hpp \
    glm/gtx/rotate_normalized_axis.inl \
    glm/gtx/rotate_vector.hpp \
    glm/gtx/rotate_vector.inl \
    glm/gtx/scalar_multiplication.hpp \
    glm/gtx/scalar_relational.hpp \
    glm/gtx/scalar_relational.inl \
    glm/gtx/spline.hpp \
    glm/gtx/spline.inl \
    glm/gtx/std_based_type.hpp \
    glm/gtx/std_based_type.inl \
    glm/gtx/string_cast.hpp \
    glm/gtx/string_cast.inl \
    glm/gtx/texture.hpp \
    glm/gtx/texture.inl \
    glm/gtx/transform.hpp \
    glm/gtx/transform.inl \
    glm/gtx/transform2.hpp \
    glm/gtx/transform2.inl \
    glm/gtx/type_aligned.hpp \
    glm/gtx/type_aligned.inl \
    glm/gtx/type_trait.hpp \
    glm/gtx/type_trait.inl \
    glm/gtx/vec_swizzle.hpp \
    glm/gtx/vector_angle.hpp \
    glm/gtx/vector_angle.inl \
    glm/gtx/vector_query.hpp \
    glm/gtx/vector_query.inl \
    glm/gtx/wrap.hpp \
    glm/gtx/wrap.inl \
    glm/integer.hpp \
    glm/mat2x2.hpp \
    glm/mat2x3.hpp \
    glm/mat2x4.hpp \
    glm/mat3x2.hpp \
    glm/mat3x3.hpp \
    glm/mat3x4.hpp \
    glm/mat4x2.hpp \
    glm/mat4x3.hpp \
    glm/mat4x4.hpp \
    glm/matrix.hpp \
    glm/packing.hpp \
    glm/simd/common.h \
    glm/simd/exponential.h \
    glm/simd/geometric.h \
    glm/simd/integer.h \
    glm/simd/matrix.h \
    glm/simd/neon.h \
    glm/simd/packing.h \
    glm/simd/platform.h \
    glm/simd/trigonometric.h \
    glm/simd/vector_relational.h \
    glm/trigonometric.hpp \
    glm/vec2.hpp \
    glm/vec3.hpp \
    glm/vec4.hpp \
    glm/vector_relational.hpp \
    globject.h \
    mainwindow.h \
    shader.h \
    square.h \
    squarecase.h \
    squarelightsource.h \
    stb_image.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    awesomeface.png \
    square.fs.glsl \
    square.vs.glsl \
    squarecase.fs.glsl \
    squarecase.vs.glsl \
    squarelightsource.fs.glsl \
    squarelightsource.vs.glsl \
    wall.jpg

copy_resource.files = $$DISTFILES
copy_resource.path = $$OUT_PWD
COPIES += copy_resource
