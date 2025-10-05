/*
 * test_data.c
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

#include "test.h"

/* Block parameters (default storage) */
P_test_T test_P = {
  /* Variable: model_param
   * Referenced by: '<Root>/Constant30'
   */
  {
    { 0.0211, 0.0, 0.0, 0.0, 0.0219, 0.0, 0.0, 0.0, 0.0366 },
    1.4,
    9.8,
    1.105e-5,
    3.558e-7,
    0.225,
    0.0001287,

    { 0.0, 0.0, 0.0 },

    { 0.0, 0.0, 0.0 },
    3.5611,
    1.1366,
    1.225,
    0.1,
    10.0,
    0.1
  },

  /* Variable: C_M
   * Referenced by: '<Root>/Constant36'
   */
  1.1366,

  /* Variable: C_T
   * Referenced by: '<Root>/Constant35'
   */
  3.5611,

  /* Variable: D_p
   * Referenced by: '<Root>/Constant38'
   */
  0.1,

  /* Variable: ModelInit_Att_qw
   * Referenced by: '<S21>/Constant1'
   */
  1.0,

  /* Variable: ModelInit_Att_qx
   * Referenced by: '<S21>/Constant4'
   */
  0.0,

  /* Variable: ModelInit_Att_qy
   * Referenced by: '<S21>/Constant2'
   */
  0.0,

  /* Variable: ModelInit_Att_qz
   * Referenced by: '<S21>/Constant3'
   */
  0.0,

  /* Variable: ModelInit_Pos_x
   * Referenced by: '<S22>/Constant3'
   */
  0.0,

  /* Variable: ModelInit_Pos_y
   * Referenced by: '<S22>/Constant2'
   */
  0.0,

  /* Variable: ModelInit_Pos_z
   * Referenced by: '<S22>/Constant1'
   */
  0.0,

  /* Variable: ModelParam_I_xx
   * Referenced by: '<S17>/Constant9'
   */
  0.0211,

  /* Variable: ModelParam_I_yy
   * Referenced by: '<S17>/Constant10'
   */
  0.0219,

  /* Variable: ModelParam_I_zz
   * Referenced by: '<S17>/Constant11'
   */
  0.0366,

  /* Variable: ModelParam_J_RP
   * Referenced by: '<S17>/Constant12'
   */
  0.0001287,

  /* Variable: ModelParam_c_M
   * Referenced by: '<S12>/Constant1'
   */
  3.558E-7,

  /* Variable: ModelParam_c_T
   * Referenced by: '<S12>/Constant'
   */
  1.105E-5,

  /* Variable: ModelParam_d
   * Referenced by:
   *   '<Root>/Constant39'
   *   '<S12>/Constant2'
   */
  0.225,

  /* Variable: ModelParam_g
   * Referenced by: '<S18>/Constant8'
   */
  9.8,

  /* Variable: ModelParam_m
   * Referenced by: '<S18>/Constant7'
   */
  1.4,

  /* Variable: p
   * Referenced by: '<Root>/Constant37'
   */
  1.225,

  /* Expression: 1
   * Referenced by: '<Root>/Constant31'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<Root>/Constant32'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<Root>/Constant33'
   */
  1.0,

  /* Expression: 1
   * Referenced by: '<Root>/Constant34'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<Root>/Memory'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Memory1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Memory2'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Memory3'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S18>/Integrator6'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S18>/Integrator1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S18>/Integrator3'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S17>/Integrator8'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S17>/Integrator9'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S17>/Integrator10'
   */
  0.0
};
