#pragma once

#define CBT_DECLARE_HANDLE(name) typedef struct name##__ { int unused; } *name

// cbtRayTestClosest
#define CBT_COLLISION_FILTER_DEFAULT 1
#define CBT_COLLISION_FILTER_STATIC 2
#define CBT_COLLISION_FILTER_KINEMATIC 4
#define CBT_COLLISION_FILTER_DEBRIS 8
#define CBT_COLLISION_FILTER_SENSOR_TRIGGER 16
#define CBT_COLLISION_FILTER_CHARACTER 32
#define CBT_COLLISION_FILTER_ALL -1

// cbtRayTestClosest
#define CBT_RAYCAST_FLAG_NONE 0
#define CBT_RAYCAST_FLAG_TRIMESH_SKIP_BACKFACES 1
#define CBT_RAYCAST_FLAG_TRIMESH_KEEP_UNFLIPPED_NORMALS 2
#define CBT_RAYCAST_FLAG_USE_SUB_SIMPLEX_CONVEX_TEST 4 // default, faster but less accurate
#define CBT_RAYCAST_FLAG_USE_USE_GJK_CONVEX_TEST 8

// cbtBodySetAnisotropicFriction
#define CBT_ANISOTROPIC_FRICTION_DISABLED 0
#define CBT_ANISOTROPIC_FRICTION 1
#define CBT_ANISOTROPIC_ROLLING_FRICTION 2

// cbtShapeGetType, cbtShapeAllocate
#define CBT_SHAPE_TYPE_BOX 0
#define CBT_SHAPE_TYPE_BOX_2D 17
#define CBT_SHAPE_TYPE_SPHERE 8
#define CBT_SHAPE_TYPE_CAPSULE 10
#define CBT_SHAPE_TYPE_CONE 11
#define CBT_SHAPE_TYPE_CYLINDER 13
#define CBT_SHAPE_TYPE_STATIC_PLANE 28
#define CBT_SHAPE_TYPE_COMPOUND 31
#define CBT_SHAPE_TYPE_TRIANGLE_MESH 21

// cbtConGetType, cbtConAllocate
#define CBT_CONSTRAINT_TYPE_POINT2POINT 3
#define CBT_CONSTRAINT_TYPE_HINGE 4
#define CBT_CONSTRAINT_TYPE_CONETWIST 5
#define CBT_CONSTRAINT_TYPE_SLIDER 7
#define CBT_CONSTRAINT_TYPE_GEAR 10
#define CBT_CONSTRAINT_TYPE_D6_SPRING_2 12

// cbtConSetParam
#define CBT_CONSTRAINT_PARAM_ERP 1
#define CBT_CONSTRAINT_PARAM_STOP_ERP 2
#define CBT_CONSTRAINT_PARAM_CFM 3
#define CBT_CONSTRAINT_PARAM_STOP_CFM 4

// cbtBodyGetActivationState, cbtBodySetActivationState
#define CBT_ACTIVE_TAG 1
#define CBT_ISLAND_SLEEPING 2
#define CBT_DISABLE_DEACTIVATION 4
#define CBT_DISABLE_SIMULATION 5

// cbtShapeCapsuleCreate, cbtShapeCylinderCreate, cbtShapeConeCreate, cbtConSetParam
#define CBT_LINEAR_AXIS_X 0
#define CBT_LINEAR_AXIS_Y 1
#define CBT_LINEAR_AXIS_Z 2
#define CBT_ANGULAR_AXIS_X 3
#define CBT_ANGULAR_AXIS_Y 4
#define CBT_ANGULAR_AXIS_Z 5

// cbtCon6DofSpring2Create
#define CBT_ROTATE_ORDER_XYZ 0
#define CBT_ROTATE_ORDER_XZY 1
#define CBT_ROTATE_ORDER_YXZ 2
#define CBT_ROTATE_ORDER_YZX 3
#define CBT_ROTATE_ORDER_ZXY 4
#define CBT_ROTATE_ORDER_ZYX 5

#define CBT_FALSE 0
#define CBT_TRUE 1

typedef float CbtVector3[3];
typedef int CbtBool;

#ifdef __cplusplus
extern "C" {
#endif

CBT_DECLARE_HANDLE(CbtWorldHandle);
CBT_DECLARE_HANDLE(CbtShapeHandle);
CBT_DECLARE_HANDLE(CbtBodyHandle);
CBT_DECLARE_HANDLE(CbtConstraintHandle);

typedef void (*CbtDrawLineCallback)(const CbtVector3 p0, const CbtVector3 p1, const CbtVector3 color, void* user_data);
typedef void (*CbtDrawContactPointCallback)(
    const CbtVector3 point,
    const CbtVector3 normal,
    float distance,
    int life_time,
    const CbtVector3 color,
    void* user_data
);
typedef void (*CbtReportErrorWarningCallback)(const char* str, void* user_data);

typedef struct CbtDebugDrawCallbacks {
    CbtDrawLineCallback drawLine;
    CbtDrawContactPointCallback drawContactPoint;
    CbtReportErrorWarningCallback reportErrorWarning;
    void* user_data;
} CbtDebugDrawCallbacks;

typedef struct CbtRayCastResult {
    CbtVector3 hit_normal_world;
    CbtVector3 hit_point_world;
    float hit_fraction;
    CbtBodyHandle body;
} CbtRayCastResult;

//
// World
//
CbtWorldHandle cbtWorldCreate(void);
void cbtWorldDestroy(CbtWorldHandle world_handle);
void cbtWorldSetGravity(CbtWorldHandle world_handle, const CbtVector3 gravity);
int cbtWorldStepSimulation(
    CbtWorldHandle world_handle,
    float time_step,
    int max_sub_steps, // 1
    float fixed_time_step // 1.0 / 60.0
);

void cbtWorldAddBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle);
void cbtWorldAddConstraint(
    CbtWorldHandle world_handle,
    CbtConstraintHandle con_handle,
    CbtBool disable_collision_between_linked_bodies // CBT_FALSE
);

void cbtWorldRemoveBody(CbtWorldHandle world_handle, CbtBodyHandle body_handle);
void cbtWorldRemoveConstraint(CbtWorldHandle world_handle, CbtConstraintHandle constraint_handle);

int cbtWorldGetNumBodies(CbtWorldHandle world_handle);
int cbtWorldGetNumConstraints(CbtWorldHandle world_handle);
CbtBodyHandle cbtWorldGetBody(CbtWorldHandle world_handle, int body_index);
CbtConstraintHandle cbtWorldGetConstraint(CbtWorldHandle world_handle, int con_index);

// Returns CBT_TRUE when hits something, CBT_FALSE otherwise
CbtBool cbtRayTestClosest(
    CbtWorldHandle world_handle,
    const CbtVector3 ray_from_world,
    const CbtVector3 ray_to_world,
    int collision_filter_group,
    int collision_filter_mask,
    unsigned int flags,
    CbtRayCastResult* result
);

void cbtWorldDebugSetCallbacks(CbtWorldHandle world_handle, const CbtDebugDrawCallbacks* callbacks);
void cbtWorldDebugDraw(CbtWorldHandle world_handle);
void cbtWorldDebugDrawLine(
    CbtWorldHandle world_handle,
    const CbtVector3 p0,
    const CbtVector3 p1,
    const CbtVector3 color
);
void cbtWorldDebugDrawSphere(
    CbtWorldHandle world_handle,
    const CbtVector3 position,
    float radius,
    const CbtVector3 color
);

//
// Shape
//
CbtShapeHandle cbtShapeAllocate(int shape_type);
void cbtShapeDeallocate(CbtShapeHandle shape_handle);

void cbtShapeDestroy(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsCreated(CbtShapeHandle shape_handle);
int cbtShapeGetType(CbtShapeHandle shape_handle);

void cbtShapeBoxCreate(CbtShapeHandle shape_handle, float half_extent_x, float half_extent_y, float half_extent_z);

void cbtShapeBox2dCreate(CbtShapeHandle shape_handle, float half_extent_x, float half_extent_y);

void cbtShapeSphereCreate(CbtShapeHandle shape_handle, float radius);
void cbtShapeSphereSetUnscaledRadius(CbtShapeHandle shape_handle, float radius);
float cbtShapeSphereGetRadius(CbtShapeHandle shape_handle);

void cbtShapePlaneCreate(CbtShapeHandle shape_handle, const CbtVector3 normal, float distance);

void cbtShapeCapsuleCreate(CbtShapeHandle shape_handle, float radius, float height, int axis);
int cbtShapeCapsuleGetUpAxis(CbtShapeHandle shape_handle);
float cbtShapeCapsuleGetHalfHeight(CbtShapeHandle shape_handle);
float cbtShapeCapsuleGetRadius(CbtShapeHandle shape_handle);

void cbtShapeCylinderCreate(CbtShapeHandle shape_handle, float half_extent_x, float half_extent_y, float half_extent_z, int axis);

void cbtShapeConeCreate(CbtShapeHandle shape_handle, float radius, float height, int axis);

void cbtShapeCompoundCreate(
    CbtShapeHandle shape_handle,
    CbtBool enable_dynamic_aabb_tree, // CBT_TRUE
    int initial_child_capacity // 0
);
void cbtShapeCompoundAddChild(
    CbtShapeHandle shape_handle,
    const CbtVector3 local_transform[4],
    CbtShapeHandle child_shape_handle
);
void cbtShapeCompoundRemoveChild(CbtShapeHandle shape_handle, CbtShapeHandle child_shape_handle);
void cbtShapeCompoundRemoveChildByIndex(CbtShapeHandle shape_handle, int child_shape_index);
int cbtShapeCompoundGetNumChilds(CbtShapeHandle shape_handle);
CbtShapeHandle cbtShapeCompoundGetChild(CbtShapeHandle shape_handle, int child_shape_index);

void cbtShapeTriMeshCreateBegin(CbtShapeHandle shape_handle);
void cbtShapeTriMeshCreateEnd(CbtShapeHandle shape_handle);
void cbtShapeTriMeshDestroy(CbtShapeHandle shape_handle);
void cbtShapeTriMeshAddIndexVertexArray(
    CbtShapeHandle shape_handle,
    int num_triangles,
    const void* triangle_base,
    int triangle_stride,
    int num_vertices,
    const void* vertex_base,
    int vertex_stride
);

CbtBool cbtShapeIsPolyhedral(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConvex2d(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConvex(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsNonMoving(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsConcave(CbtShapeHandle shape_handle);
CbtBool cbtShapeIsCompound(CbtShapeHandle shape_handle);

void cbtShapeCalculateLocalInertia(CbtShapeHandle shape_handle, float mass, CbtVector3 inertia);

void cbtShapeSetUserPointer(CbtShapeHandle shape_handle, void* user_pointer);
void* cbtShapeGetUserPointer(CbtShapeHandle shape_handle);
void cbtShapeSetUserIndex(CbtShapeHandle shape_handle, int slot, int user_index); // slot can be 0 or 1
int cbtShapeGetUserIndex(CbtShapeHandle shape_handle, int slot); // slot can be 0 or 1

//
// Body
//
CbtBodyHandle cbtBodyAllocate(void);
void cbtBodyAllocateBatch(unsigned int num, CbtBodyHandle* body_handles);
void cbtBodyDeallocate(CbtBodyHandle body_handle);
void cbtBodyDeallocateBatch(unsigned int num, CbtBodyHandle* body_handles);

void cbtBodyCreate(
    CbtBodyHandle body_handle,
    float mass,
    const CbtVector3 transform[4],
    CbtShapeHandle shape_handle
);
void cbtBodyDestroy(CbtBodyHandle body_handle);
CbtBool cbtBodyIsCreated(CbtBodyHandle body_handle);

void cbtBodySetShape(CbtBodyHandle body_handle, CbtShapeHandle shape_handle);
CbtShapeHandle cbtBodyGetShape(CbtBodyHandle body_handle);

void cbtBodySetRestitution(CbtBodyHandle body_handle, float restitution);

void cbtBodySetFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetRollingFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetSpinningFriction(CbtBodyHandle body_handle, float friction);
void cbtBodySetAnisotropicFriction(CbtBodyHandle body_handle, const CbtVector3 friction, int mode);

void cbtBodySetContactStiffnessAndDamping(CbtBodyHandle body_handle, float stiffness, float damping);

void cbtBodySetMassProps(CbtBodyHandle body_handle, float mass, const CbtVector3 inertia);

void cbtBodySetDamping(CbtBodyHandle body_handle, float linear, float angular);

void cbtBodySetLinearVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity);
void cbtBodySetAngularVelocity(CbtBodyHandle body_handle, const CbtVector3 velocity);

void cbtBodySetLinearFactor(CbtBodyHandle body_handle, const CbtVector3 factor);
void cbtBodySetAngularFactor(CbtBodyHandle body_handle, const CbtVector3 factor);

void cbtBodyApplyCentralForce(CbtBodyHandle body_handle, const CbtVector3 force);
void cbtBodyApplyCentralImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse);
void cbtBodyApplyForce(CbtBodyHandle body_handle, const CbtVector3 force, const CbtVector3 rel_pos);
void cbtBodyApplyImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse, const CbtVector3 rel_pos);
void cbtBodyApplyTorque(CbtBodyHandle body_handle, const CbtVector3 torque);
void cbtBodyApplyTorqueImpulse(CbtBodyHandle body_handle, const CbtVector3 impulse);

float cbtBodyGetRestitution(CbtBodyHandle body_handle);

float cbtBodyGetFriction(CbtBodyHandle body_handle);
float cbtBodyGetRollingFriction(CbtBodyHandle body_handle);
float cbtBodyGetSpinningFriction(CbtBodyHandle body_handle);
void cbtBodyGetAnisotropicFriction(CbtBodyHandle body_handle, CbtVector3 friction);

float cbtBodyGetContactStiffness(CbtBodyHandle body_handle);
float cbtBodyGetContactDamping(CbtBodyHandle body_handle);

float cbtBodyGetMass(CbtBodyHandle body_handle);

float cbtBodyGetLinearDamping(CbtBodyHandle body_handle);
float cbtBodyGetAngularDamping(CbtBodyHandle body_handle);

void cbtBodyGetLinearVelocity(CbtBodyHandle body_handle, CbtVector3 velocity);
void cbtBodyGetAngularVelocity(CbtBodyHandle body_handle, CbtVector3 velocity);

void cbtBodyGetTotalForce(CbtBodyHandle body_handle, CbtVector3 force);
void cbtBodyGetTotalTorque(CbtBodyHandle body_handle, CbtVector3 torque);

CbtBool cbtBodyIsStatic(CbtBodyHandle body_handle);
CbtBool cbtBodyIsKinematic(CbtBodyHandle body_handle);
CbtBool cbtBodyIsStaticOrKinematic(CbtBodyHandle body_handle);

float cbtBodyGetDeactivationTime(CbtBodyHandle body_handle);
void cbtBodySetDeactivationTime(CbtBodyHandle body_handle, float time);
int cbtBodyGetActivationState(CbtBodyHandle body_handle);
void cbtBodySetActivationState(CbtBodyHandle body_handle, int state);
void cbtBodyForceActivationState(CbtBodyHandle body_handle, int state);
CbtBool cbtBodyIsActive(CbtBodyHandle body_handle);
CbtBool cbtBodyIsInWorld(CbtBodyHandle body_handle);

void cbtBodySetUserPointer(CbtBodyHandle body_handle, void* user_pointer);
void* cbtBodyGetUserPointer(CbtBodyHandle body_handle, int slot);
void cbtBodySetUserIndex(CbtBodyHandle body_handle, int slot, int user_index); // slot can be 0, 1 or 2
int cbtBodyGetUserIndex(CbtBodyHandle body_handle, int slot); // slot can be 0, 1 or 2

void cbtBodySetCenterOfMassTransform(CbtBodyHandle body_handle, const CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);
void cbtBodyGetCenterOfMassPosition(CbtBodyHandle body_handle, CbtVector3 position);
void cbtBodyGetInvCenterOfMassTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);
void cbtBodyGetGraphicsWorldTransform(CbtBodyHandle body_handle, CbtVector3 transform[4]);

//
// Constraints
//
CbtBodyHandle cbtConGetFixedBody(void);

CbtConstraintHandle cbtConAllocate(int con_type);
void cbtConDeallocate(CbtConstraintHandle con_handle);

void cbtConDestroy(CbtConstraintHandle con_handle);
CbtBool cbtConIsCreated(CbtConstraintHandle con_handle);
int cbtConGetType(CbtConstraintHandle con_handle);

void cbtConSetParam(CbtConstraintHandle con_handle, int param, float value, int axis /* -1 */);
float cbtConGetParam(CbtConstraintHandle con_handle, int param, int axis /* -1 */);

void cbtConSetEnabled(CbtConstraintHandle con_handle, CbtBool enabled);
CbtBool cbtConIsEnabled(CbtConstraintHandle con_handle);
CbtBodyHandle cbtConGetBodyA(CbtConstraintHandle con_handle);
CbtBodyHandle cbtConGetBodyB(CbtConstraintHandle con_handle);
void cbtConSetBreakingImpulseThreshold(CbtConstraintHandle con_handle, float threshold);
float cbtConGetBreakingImpulseThreshold(CbtConstraintHandle con_handle);

void cbtConSetDebugDrawSize(CbtConstraintHandle con_handle, float size);
float cbtConGetDebugDrawSize(CbtConstraintHandle con_handle);

// Point2Point
void cbtConPoint2PointCreate1(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    const CbtVector3 pivot_a
);
void cbtConPoint2PointCreate2(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b
);
void cbtConPoint2PointSetPivotA(CbtConstraintHandle con_handle, const CbtVector3 pivot);
void cbtConPoint2PointSetPivotB(CbtConstraintHandle con_handle, const CbtVector3 pivot);
void cbtConPoint2PointSetTau(CbtConstraintHandle con_handle, float tau);
void cbtConPoint2PointSetDamping(CbtConstraintHandle con_handle, float damping);
void cbtConPoint2PointSetImpulseClamp(CbtConstraintHandle con_handle, float impulse_clamp);

// Hinge
void cbtConHingeCreate1(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    const CbtVector3 pivot_a,
    const CbtVector3 axis_a,
    CbtBool use_reference_frame_a // CBT_FALSE
);
void cbtConHingeCreate2(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 pivot_a,
    const CbtVector3 pivot_b,
    const CbtVector3 axis_a,
    const CbtVector3 axis_b,
    CbtBool use_reference_frame_a // CBT_FALSE
);
void cbtConHingeCreate3(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    const CbtVector3 frame_a[4],
    CbtBool use_reference_frame_a // CBT_FALSE
);
void cbtConHingeSetAngularOnly(CbtConstraintHandle con_handle, CbtBool angular_only);
void cbtConHingeEnableAngularMotor(
    CbtConstraintHandle con_handle,
    CbtBool enable,
    float target_velocity,
    float max_motor_impulse
);
void cbtConHingeSetLimit(
    CbtConstraintHandle con_handle,
    float low,
    float high,
    float softness, // 0.9
    float bias_factor, // 0.3
    float relaxation_factor // 1.0
);

// Gear
void cbtConGearCreate(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 axis_a,
    const CbtVector3 axis_b,
    float ratio // 1.0
);
void cbtConGearSetAxisA(CbtConstraintHandle con_handle, const CbtVector3 axis);
void cbtConGearSetAxisB(CbtConstraintHandle con_handle, const CbtVector3 axis);
void cbtConGearSetRatio(CbtConstraintHandle con_handle, float ratio);
void cbtConGearGetAxisA(CbtConstraintHandle con_handle, CbtVector3 axis);
void cbtConGearGetAxisB(CbtConstraintHandle con_handle, CbtVector3 axis);
float cbtConGearGetRatio(CbtConstraintHandle con_handle);

// Slider
void cbtConSliderCreate1(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_b,
    const CbtVector3 frame_b[4],
    CbtBool use_reference_frame_a
);
void cbtConSliderCreate2(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 frame_a[4],
    const CbtVector3 frame_b[4],
    CbtBool use_reference_frame_a
);
void cbtConSliderSetLinearLowerLimit(CbtConstraintHandle con_handle, float limit);
void cbtConSliderSetLinearUpperLimit(CbtConstraintHandle con_handle, float limit);
float cbtConSliderGetLinearLowerLimit(CbtConstraintHandle con_handle);
float cbtConSliderGetLinearUpperLimit(CbtConstraintHandle con_handle);

void cbtConSliderSetAngularLowerLimit(CbtConstraintHandle con_handle, float limit);
void cbtConSliderSetAngularUpperLimit(CbtConstraintHandle con_handle, float limit);
float cbtConSliderGetAngularLowerLimit(CbtConstraintHandle con_handle);
float cbtConSliderGetAngularUpperLimit(CbtConstraintHandle con_handle);

// Generic 6Dof Spring Constraint (ver. 2)
void cbtConD6Spring2Create1(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_b,
    const CbtVector3 frame_b[4],
    int rotate_order // CBT_ROTATE_ORDER_XYZ
);
void cbtConD6Spring2Create2(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 frame_a[4],
    const CbtVector3 frame_b[4],
    int rotate_order // CBT_ROTATE_ORDER_XYZ
);
void cbtConD6Spring2SetLinearLowerLimit(CbtConstraintHandle con_handle, const CbtVector3 limit);
void cbtConD6Spring2SetLinearUpperLimit(CbtConstraintHandle con_handle, const CbtVector3 limit);
void cbtConD6Spring2GetLinearLowerLimit(CbtConstraintHandle con_handle, CbtVector3 limit);
void cbtConD6Spring2GetLinearUpperLimit(CbtConstraintHandle con_handle, CbtVector3 limit);

void cbtConD6Spring2SetAngularLowerLimit(CbtConstraintHandle con_handle, const CbtVector3 limit);
void cbtConD6Spring2SetAngularUpperLimit(CbtConstraintHandle con_handle, const CbtVector3 limit);
void cbtConD6Spring2GetAngularLowerLimit(CbtConstraintHandle con_handle, CbtVector3 limit);
void cbtConD6Spring2GetAngularUpperLimit(CbtConstraintHandle con_handle, CbtVector3 limit);

// Cone Twist
void cbtConConeTwistCreate1(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    const CbtVector3 frame_a[4]
);
void cbtConConeTwistCreate2(
    CbtConstraintHandle con_handle,
    CbtBodyHandle body_handle_a,
    CbtBodyHandle body_handle_b,
    const CbtVector3 frame_a[4],
    const CbtVector3 frame_b[4]
);
void cbtConConeTwistSetLimit(
    CbtConstraintHandle con_handle,
    float swing_span1,
    float swing_span2,
    float twist_span,
    float softness, // 1.0
    float bias_factor, // 0.3
    float relaxation_factor // 1.0
);

#ifdef __cplusplus
}
#endif
