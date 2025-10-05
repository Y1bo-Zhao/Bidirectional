/*
 * test.h
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

#ifndef RTW_HEADER_test_h_
#define RTW_HEADER_test_h_
#ifndef test_COMMON_INCLUDES_
#define test_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                                 /* test_COMMON_INCLUDES_ */

#include "test_types.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <float.h>
#include <string.h>
#include <stddef.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  K_bus K;                             /* '<Root>/MATLAB Function10' */
  real_T Constant1;                    /* '<S21>/Constant1' */
  real_T Constant4;                    /* '<S21>/Constant4' */
  real_T Constant2;                    /* '<S21>/Constant2' */
  real_T Constant3;                    /* '<S21>/Constant3' */
  real_T Memory[3];                    /* '<Root>/Memory' */
  real_T Memory1[3];                   /* '<Root>/Memory1' */
  real_T Memory2[3];                   /* '<Root>/Memory2' */
  real_T Memory3[3];                   /* '<Root>/Memory3' */
  real_T Constant3_o;                  /* '<S22>/Constant3' */
  real_T Integrator;                   /* '<S22>/Integrator' */
  real_T Constant2_p;                  /* '<S22>/Constant2' */
  real_T Integrator1;                  /* '<S22>/Integrator1' */
  real_T Constant1_k;                  /* '<S22>/Constant1' */
  real_T Integrator2;                  /* '<S22>/Integrator2' */
  real_T Integrator6;                  /* '<S18>/Integrator6' */
  real_T Integrator1_b;                /* '<S18>/Integrator1' */
  real_T Integrator3;                  /* '<S18>/Integrator3' */
  real_T qw_dot;                       /* '<S21>/MATLAB Function2' */
  real_T qx_dot;                       /* '<S21>/MATLAB Function2' */
  real_T qy_dot;                       /* '<S21>/MATLAB Function2' */
  real_T qz_dot;                       /* '<S21>/MATLAB Function2' */
  real_T qw_unit;                      /* '<S21>/MATLAB Function1' */
  real_T qx_unit;                      /* '<S21>/MATLAB Function1' */
  real_T qy_unit;                      /* '<S21>/MATLAB Function1' */
  real_T v_x_dot;                      /* '<S18>/MATLAB Function' */
  real_T v_y_dot;                      /* '<S18>/MATLAB Function' */
  real_T v_z_dot;                      /* '<S18>/MATLAB Function' */
  real_T p_dot;                        /* '<S17>/MATLAB Function' */
  real_T q_dot;                        /* '<S17>/MATLAB Function' */
  real_T r_dot;                        /* '<S17>/MATLAB Function' */
  real_T updated_prev_pos[3];          /* '<Root>/MATLAB Function12' */
  real_T updated_prev_vel[3];          /* '<Root>/MATLAB Function12' */
  real_T updated_prev_acc[3];          /* '<Root>/MATLAB Function12' */
  real_T updated_prev_jerk[3];         /* '<Root>/MATLAB Function12' */
} B_test_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Memory_PreviousInput[3];      /* '<Root>/Memory' */
  real_T Memory1_PreviousInput[3];     /* '<Root>/Memory1' */
  real_T Memory2_PreviousInput[3];     /* '<Root>/Memory2' */
  real_T Memory3_PreviousInput[3];     /* '<Root>/Memory3' */
  struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } FromWorkspace_PWORK;               /* '<Root>/From Workspace' */

  struct {
    int_T PrevIndex;
  } FromWorkspace_IWORK;               /* '<Root>/From Workspace' */

  int_T Integrator3_IWORK;             /* '<S21>/Integrator3' */
  int_T Integrator1_IWORK;             /* '<S21>/Integrator1' */
  int_T Integrator4_IWORK;             /* '<S21>/Integrator4' */
  int_T Integrator5_IWORK;             /* '<S21>/Integrator5' */
  int_T Integrator_IWORK;              /* '<S22>/Integrator' */
  int_T Integrator1_IWORK_l;           /* '<S22>/Integrator1' */
  int_T Integrator2_IWORK;             /* '<S22>/Integrator2' */
} DW_test_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator3_CSTATE;           /* '<S21>/Integrator3' */
  real_T Integrator1_CSTATE;           /* '<S21>/Integrator1' */
  real_T Integrator4_CSTATE;           /* '<S21>/Integrator4' */
  real_T Integrator5_CSTATE;           /* '<S21>/Integrator5' */
  real_T Integrator_CSTATE;            /* '<S22>/Integrator' */
  real_T Integrator1_CSTATE_j;         /* '<S22>/Integrator1' */
  real_T Integrator2_CSTATE;           /* '<S22>/Integrator2' */
  real_T Integrator6_CSTATE;           /* '<S18>/Integrator6' */
  real_T Integrator1_CSTATE_i;         /* '<S18>/Integrator1' */
  real_T Integrator3_CSTATE_k;         /* '<S18>/Integrator3' */
  real_T Integrator8_CSTATE;           /* '<S17>/Integrator8' */
  real_T Integrator9_CSTATE;           /* '<S17>/Integrator9' */
  real_T Integrator10_CSTATE;          /* '<S17>/Integrator10' */
} X_test_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator3_CSTATE;           /* '<S21>/Integrator3' */
  real_T Integrator1_CSTATE;           /* '<S21>/Integrator1' */
  real_T Integrator4_CSTATE;           /* '<S21>/Integrator4' */
  real_T Integrator5_CSTATE;           /* '<S21>/Integrator5' */
  real_T Integrator_CSTATE;            /* '<S22>/Integrator' */
  real_T Integrator1_CSTATE_j;         /* '<S22>/Integrator1' */
  real_T Integrator2_CSTATE;           /* '<S22>/Integrator2' */
  real_T Integrator6_CSTATE;           /* '<S18>/Integrator6' */
  real_T Integrator1_CSTATE_i;         /* '<S18>/Integrator1' */
  real_T Integrator3_CSTATE_k;         /* '<S18>/Integrator3' */
  real_T Integrator8_CSTATE;           /* '<S17>/Integrator8' */
  real_T Integrator9_CSTATE;           /* '<S17>/Integrator9' */
  real_T Integrator10_CSTATE;          /* '<S17>/Integrator10' */
} XDot_test_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator3_CSTATE;        /* '<S21>/Integrator3' */
  boolean_T Integrator1_CSTATE;        /* '<S21>/Integrator1' */
  boolean_T Integrator4_CSTATE;        /* '<S21>/Integrator4' */
  boolean_T Integrator5_CSTATE;        /* '<S21>/Integrator5' */
  boolean_T Integrator_CSTATE;         /* '<S22>/Integrator' */
  boolean_T Integrator1_CSTATE_j;      /* '<S22>/Integrator1' */
  boolean_T Integrator2_CSTATE;        /* '<S22>/Integrator2' */
  boolean_T Integrator6_CSTATE;        /* '<S18>/Integrator6' */
  boolean_T Integrator1_CSTATE_i;      /* '<S18>/Integrator1' */
  boolean_T Integrator3_CSTATE_k;      /* '<S18>/Integrator3' */
  boolean_T Integrator8_CSTATE;        /* '<S17>/Integrator8' */
  boolean_T Integrator9_CSTATE;        /* '<S17>/Integrator9' */
  boolean_T Integrator10_CSTATE;       /* '<S17>/Integrator10' */
} XDis_test_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Parameters (default storage) */
struct P_test_T_ {
  model_param_bus model_param;         /* Variable: model_param
                                        * Referenced by: '<Root>/Constant30'
                                        */
  real_T C_M;                          /* Variable: C_M
                                        * Referenced by: '<Root>/Constant36'
                                        */
  real_T C_T;                          /* Variable: C_T
                                        * Referenced by: '<Root>/Constant35'
                                        */
  real_T D_p;                          /* Variable: D_p
                                        * Referenced by: '<Root>/Constant38'
                                        */
  real_T ModelInit_Att_qw;             /* Variable: ModelInit_Att_qw
                                        * Referenced by: '<S21>/Constant1'
                                        */
  real_T ModelInit_Att_qx;             /* Variable: ModelInit_Att_qx
                                        * Referenced by: '<S21>/Constant4'
                                        */
  real_T ModelInit_Att_qy;             /* Variable: ModelInit_Att_qy
                                        * Referenced by: '<S21>/Constant2'
                                        */
  real_T ModelInit_Att_qz;             /* Variable: ModelInit_Att_qz
                                        * Referenced by: '<S21>/Constant3'
                                        */
  real_T ModelInit_Pos_x;              /* Variable: ModelInit_Pos_x
                                        * Referenced by: '<S22>/Constant3'
                                        */
  real_T ModelInit_Pos_y;              /* Variable: ModelInit_Pos_y
                                        * Referenced by: '<S22>/Constant2'
                                        */
  real_T ModelInit_Pos_z;              /* Variable: ModelInit_Pos_z
                                        * Referenced by: '<S22>/Constant1'
                                        */
  real_T ModelParam_I_xx;              /* Variable: ModelParam_I_xx
                                        * Referenced by: '<S17>/Constant9'
                                        */
  real_T ModelParam_I_yy;              /* Variable: ModelParam_I_yy
                                        * Referenced by: '<S17>/Constant10'
                                        */
  real_T ModelParam_I_zz;              /* Variable: ModelParam_I_zz
                                        * Referenced by: '<S17>/Constant11'
                                        */
  real_T ModelParam_J_RP;              /* Variable: ModelParam_J_RP
                                        * Referenced by: '<S17>/Constant12'
                                        */
  real_T ModelParam_c_M;               /* Variable: ModelParam_c_M
                                        * Referenced by: '<S12>/Constant1'
                                        */
  real_T ModelParam_c_T;               /* Variable: ModelParam_c_T
                                        * Referenced by: '<S12>/Constant'
                                        */
  real_T ModelParam_d;                 /* Variable: ModelParam_d
                                        * Referenced by:
                                        *   '<Root>/Constant39'
                                        *   '<S12>/Constant2'
                                        */
  real_T ModelParam_g;                 /* Variable: ModelParam_g
                                        * Referenced by: '<S18>/Constant8'
                                        */
  real_T ModelParam_m;                 /* Variable: ModelParam_m
                                        * Referenced by: '<S18>/Constant7'
                                        */
  real_T p;                            /* Variable: p
                                        * Referenced by: '<Root>/Constant37'
                                        */
  real_T Constant31_Value;             /* Expression: 1
                                        * Referenced by: '<Root>/Constant31'
                                        */
  real_T Constant32_Value;             /* Expression: 1
                                        * Referenced by: '<Root>/Constant32'
                                        */
  real_T Constant33_Value;             /* Expression: 1
                                        * Referenced by: '<Root>/Constant33'
                                        */
  real_T Constant34_Value;             /* Expression: 1
                                        * Referenced by: '<Root>/Constant34'
                                        */
  real_T Memory_InitialCondition;      /* Expression: 0
                                        * Referenced by: '<Root>/Memory'
                                        */
  real_T Memory1_InitialCondition;     /* Expression: 0
                                        * Referenced by: '<Root>/Memory1'
                                        */
  real_T Memory2_InitialCondition;     /* Expression: 0
                                        * Referenced by: '<Root>/Memory2'
                                        */
  real_T Memory3_InitialCondition;     /* Expression: 0
                                        * Referenced by: '<Root>/Memory3'
                                        */
  real_T Integrator6_IC;               /* Expression: 0
                                        * Referenced by: '<S18>/Integrator6'
                                        */
  real_T Integrator1_IC;               /* Expression: 0
                                        * Referenced by: '<S18>/Integrator1'
                                        */
  real_T Integrator3_IC;               /* Expression: 0
                                        * Referenced by: '<S18>/Integrator3'
                                        */
  real_T Integrator8_IC;               /* Expression: 0
                                        * Referenced by: '<S17>/Integrator8'
                                        */
  real_T Integrator9_IC;               /* Expression: 0
                                        * Referenced by: '<S17>/Integrator9'
                                        */
  real_T Integrator10_IC;              /* Expression: 0
                                        * Referenced by: '<S17>/Integrator10'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_test_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_test_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_test_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[13];
  real_T odeF[3][13];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    boolean_T firstInitCondFlag;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_test_T test_P;

/* Block signals (default storage) */
extern B_test_T test_B;

/* Continuous states (default storage) */
extern X_test_T test_X;

/* Block states (default storage) */
extern DW_test_T test_DW;

/* Model entry point functions */
extern void test_initialize(void);
extern void test_step(void);
extern void test_terminate(void);

/* Real-time Model object */
extern RT_MODEL_test_T *const test_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'test'
 * '<S1>'   : 'test/MATLAB Function10'
 * '<S2>'   : 'test/MATLAB Function11'
 * '<S3>'   : 'test/MATLAB Function12'
 * '<S4>'   : 'test/MATLAB Function13'
 * '<S5>'   : 'test/MATLAB Function5'
 * '<S6>'   : 'test/MATLAB Function6'
 * '<S7>'   : 'test/MATLAB Function7'
 * '<S8>'   : 'test/MATLAB Function8'
 * '<S9>'   : 'test/MATLAB Function9'
 * '<S10>'  : 'test/混合无人机模型'
 * '<S11>'  : 'test/混合无人机模型/Quadcopter Dynamics Model'
 * '<S12>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Control Effectiveness Model'
 * '<S13>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/MATLAB Function'
 * '<S14>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Dynamic Model'
 * '<S15>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Kinematic Model'
 * '<S16>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Control Effectiveness Model/MATLAB Function'
 * '<S17>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Dynamic Model/Attitude Dynamic Model'
 * '<S18>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Dynamic Model/Position Dynamic Model1'
 * '<S19>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Dynamic Model/Attitude Dynamic Model/MATLAB Function'
 * '<S20>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Dynamic Model/Position Dynamic Model1/MATLAB Function'
 * '<S21>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Kinematic Model/Attitude Kinematic Model1'
 * '<S22>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Kinematic Model/Position Kinematic Model'
 * '<S23>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Kinematic Model/Attitude Kinematic Model1/MATLAB Function1'
 * '<S24>'  : 'test/混合无人机模型/Quadcopter Dynamics Model/Rigid-Body Kinematic Model/Attitude Kinematic Model1/MATLAB Function2'
 */
#endif                                 /* RTW_HEADER_test_h_ */
