/*
 * test_types.h
 *
 * Code generation for model "test".
 *
 * Model version              : 1.19
 * Simulink Coder version : 9.8 (R2022b) 13-May-2022
 * C source code generated on : Mon May 27 13:58:19 2024
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_test_types_h_
#define RTW_HEADER_test_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_model_param_bus_
#define DEFINED_TYPEDEF_FOR_model_param_bus_

typedef struct {
  real_T I[9];
  real_T mass;
  real_T g;
  real_T c_T;
  real_T c_M;
  real_T d;
  real_T J_RP;
  real_T init_pos[3];
  real_T init_att[3];
  real_T C_T;
  real_T C_M;
  real_T air_density;
  real_T prop_diameter;
  real_T f_max;
  real_T tau_max;
} model_param_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_K_bus_
#define DEFINED_TYPEDEF_FOR_K_bus_

typedef struct {
  real_T Kp;
  real_T Kv;
  real_T KR;
  real_T K_omega;
} K_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_desired_state_bus_
#define DEFINED_TYPEDEF_FOR_desired_state_bus_

typedef struct {
  real_T pos[3];
  real_T vel[3];
  real_T acc[3];
  real_T jerk[3];
  real_T snap[3];
  real_T yaw;
  real_T yawdot;
  real_T yawddot;
} desired_state_bus;

#endif

/* Parameters (default storage) */
typedef struct P_test_T_ P_test_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_test_T RT_MODEL_test_T;

#endif                                 /* RTW_HEADER_test_types_h_ */
