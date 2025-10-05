/*
 * test.c
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
#include "rtwtypes.h"
#include <math.h>
#include "test_private.h"
#include <emmintrin.h>
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include <string.h>

/* Block signals (default storage) */
B_test_T test_B;

/* Continuous states */
X_test_T test_X;

/* Block states (default storage) */
DW_test_T test_DW;

/* Real-time model */
static RT_MODEL_test_T test_M_;
RT_MODEL_test_T *const test_M = &test_M_;

/* Forward declaration for local functions */
static real_T test_norm(const real_T x[3]);

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 13;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  test_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  test_step();
  test_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  test_step();
  test_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp, tmp_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Function for MATLAB Function: '<Root>/MATLAB Function5' */
static real_T test_norm(const real_T x[3])
{
  real_T absxk;
  real_T scale;
  real_T t;
  real_T y;
  scale = 3.3121686421112381E-170;
  absxk = fabs(x[0]);
  if (absxk > 3.3121686421112381E-170) {
    y = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    y = t * t;
  }

  absxk = fabs(x[1]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  absxk = fabs(x[2]);
  if (absxk > scale) {
    t = scale / absxk;
    y = y * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    y += t * t;
  }

  return scale * sqrt(y);
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Model step function */
void test_step(void)
{
  /* local block i/o variables */
  real_T rtb_FromWorkspace[5];
  __m128d tmp_3;
  __m128d tmp_4;
  __m128d tmp_5;
  __m128d tmp_6;
  __m128d tmp_7;
  __m128d tmp_8;
  __m128d tmp_9;
  real_T A[16];
  real_T rtb_current_state[12];
  real_T KM_0[9];
  real_T KM_1[9];
  real_T R[9];
  real_T Rd1[9];
  real_T Rd_dot[9];
  real_T b_R[9];
  real_T b_R_tmp[9];
  real_T b_R_tmp_0[9];
  real_T c_R[9];
  real_T e[9];
  real_T rtb_trajectory[4];
  real_T Fd[3];
  real_T Fd_2dot[3];
  real_T ea[3];
  real_T ev[3];
  real_T rtb_desired_state_jerk[3];
  real_T rtb_snap[3];
  real_T tmp[3];
  real_T tmp_0[3];
  real_T wd[3];
  real_T xcd_dot[3];
  real_T xcd_dot_tmp[3];
  real_T zbd_x_xcd_dot[3];
  real_T Fd_0;
  real_T Fd_1;
  real_T Fd_2;
  real_T Fd_3;
  real_T Fd_norm_dot;
  real_T Fd_tmp;
  real_T KF;
  real_T KM;
  real_T KM_tmp;
  real_T R_tmp;
  real_T b_b;
  real_T b_c;
  real_T b_c_tmp;
  real_T ev_0;
  real_T ev_tmp;
  real_T ev_tmp_0;
  real_T ev_tmp_1;
  real_T norm_q;
  real_T rtb_current_state_tmp;
  real_T rtb_current_state_tmp_0;
  real_T rtb_density;
  real_T rtb_w4;
  real_T xcd_dot_tmp_0;
  real_T xcd_idx_1;
  real_T ybd_dot_idx_0;
  real_T ybd_dot_idx_1;
  real_T ybd_dot_idx_2;
  real_T ybd_idx_0;
  real_T ybd_idx_1;
  real_T ybd_idx_2;
  real_T zbd;
  real_T zbd_dot_idx_0;
  real_T zbd_dot_idx_1;
  real_T zbd_dot_idx_2;
  real_T zbd_idx_1;
  real_T zbd_xcd_norm_dot;
  int32_T b_R_tmp_tmp;
  int32_T b_ix;
  int32_T ijA;
  int32_T iy;
  int32_T j;
  int32_T jj;
  int8_T ipiv[4];
  static const int8_T f[3] = { 0, 1, 0 };

  if (rtmIsMajorTimeStep(test_M)) {
    /* set solver stop time */
    if (!(test_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&test_M->solverInfo, ((test_M->Timing.clockTickH0 +
        1) * test_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&test_M->solverInfo, ((test_M->Timing.clockTick0 + 1)
        * test_M->Timing.stepSize0 + test_M->Timing.clockTickH0 *
        test_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(test_M)) {
    test_M->Timing.t[0] = rtsiGetT(&test_M->solverInfo);
  }

  /* FromWorkspace: '<Root>/From Workspace' */
  {
    real_T *pDataValues = (real_T *) test_DW.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *) test_DW.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = test_DW.FromWorkspace_IWORK.PrevIndex;
    real_T t = test_M->Timing.t[0];

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[43]) {
      currTimeIndex = 42;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    test_DW.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 5; ++elIdx) {
              (&rtb_FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex];
              pDataValues += 44;
            }
          }
        } else {
          {
            int_T elIdx;
            for (elIdx = 0; elIdx < 5; ++elIdx) {
              (&rtb_FromWorkspace[0])[elIdx] = pDataValues[currTimeIndex + 1];
              pDataValues += 44;
            }
          }
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex = currTimeIndex;

        {
          int_T elIdx;
          for (elIdx = 0; elIdx < 5; ++elIdx) {
            d1 = pDataValues[TimeIndex];
            d2 = pDataValues[TimeIndex + 1];
            (&rtb_FromWorkspace[0])[elIdx] = (real_T) rtInterpolate(d1, d2, f1,
              f2);
            pDataValues += 44;
          }
        }
      }
    }
  }

  if (rtmIsMajorTimeStep(test_M)) {
    /* MATLAB Function: '<Root>/MATLAB Function10' incorporates:
     *  Constant: '<Root>/Constant31'
     *  Constant: '<Root>/Constant32'
     *  Constant: '<Root>/Constant33'
     *  Constant: '<Root>/Constant34'
     */
    test_B.K.Kp = test_P.Constant31_Value;
    test_B.K.Kv = test_P.Constant32_Value;
    test_B.K.KR = test_P.Constant33_Value;
    test_B.K.K_omega = test_P.Constant34_Value;

    /* Constant: '<S21>/Constant1' */
    test_B.Constant1 = test_P.ModelInit_Att_qw;

    /* Constant: '<S21>/Constant4' */
    test_B.Constant4 = test_P.ModelInit_Att_qx;
  }

  /* Integrator: '<S21>/Integrator3' */
  if (test_DW.Integrator3_IWORK != 0) {
    test_X.Integrator3_CSTATE = test_B.Constant1;
  }

  /* Integrator: '<S21>/Integrator1' */
  if (test_DW.Integrator1_IWORK != 0) {
    test_X.Integrator1_CSTATE = test_B.Constant4;
  }

  if (rtmIsMajorTimeStep(test_M)) {
    /* Constant: '<S21>/Constant2' */
    test_B.Constant2 = test_P.ModelInit_Att_qy;

    /* Constant: '<S21>/Constant3' */
    test_B.Constant3 = test_P.ModelInit_Att_qz;
  }

  /* Integrator: '<S21>/Integrator4' */
  if (test_DW.Integrator4_IWORK != 0) {
    test_X.Integrator4_CSTATE = test_B.Constant2;
  }

  /* Integrator: '<S21>/Integrator5' */
  if (test_DW.Integrator5_IWORK != 0) {
    test_X.Integrator5_CSTATE = test_B.Constant3;
  }

  /* MATLAB Function: '<S21>/MATLAB Function1' incorporates:
   *  Integrator: '<S21>/Integrator1'
   *  Integrator: '<S21>/Integrator3'
   *  Integrator: '<S21>/Integrator4'
   *  Integrator: '<S21>/Integrator5'
   */
  norm_q = sqrt(((test_X.Integrator3_CSTATE * test_X.Integrator3_CSTATE +
                  test_X.Integrator1_CSTATE * test_X.Integrator1_CSTATE) +
                 test_X.Integrator4_CSTATE * test_X.Integrator4_CSTATE) +
                test_X.Integrator5_CSTATE * test_X.Integrator5_CSTATE);
  test_B.qw_unit = test_X.Integrator3_CSTATE / norm_q;
  test_B.qx_unit = test_X.Integrator1_CSTATE / norm_q;
  test_B.qy_unit = test_X.Integrator4_CSTATE / norm_q;
  norm_q = test_X.Integrator5_CSTATE / norm_q;
  if (rtmIsMajorTimeStep(test_M)) {
    /* Memory: '<Root>/Memory' */
    test_B.Memory[0] = test_DW.Memory_PreviousInput[0];

    /* Memory: '<Root>/Memory1' */
    test_B.Memory1[0] = test_DW.Memory1_PreviousInput[0];

    /* Memory: '<Root>/Memory2' */
    test_B.Memory2[0] = test_DW.Memory2_PreviousInput[0];

    /* Memory: '<Root>/Memory3' */
    test_B.Memory3[0] = test_DW.Memory3_PreviousInput[0];

    /* Memory: '<Root>/Memory' */
    test_B.Memory[1] = test_DW.Memory_PreviousInput[1];

    /* Memory: '<Root>/Memory1' */
    test_B.Memory1[1] = test_DW.Memory1_PreviousInput[1];

    /* Memory: '<Root>/Memory2' */
    test_B.Memory2[1] = test_DW.Memory2_PreviousInput[1];

    /* Memory: '<Root>/Memory3' */
    test_B.Memory3[1] = test_DW.Memory3_PreviousInput[1];

    /* Memory: '<Root>/Memory' */
    test_B.Memory[2] = test_DW.Memory_PreviousInput[2];

    /* Memory: '<Root>/Memory1' */
    test_B.Memory1[2] = test_DW.Memory1_PreviousInput[2];

    /* Memory: '<Root>/Memory2' */
    test_B.Memory2[2] = test_DW.Memory2_PreviousInput[2];

    /* Memory: '<Root>/Memory3' */
    test_B.Memory3[2] = test_DW.Memory3_PreviousInput[2];

    /* Constant: '<S22>/Constant3' */
    test_B.Constant3_o = test_P.ModelInit_Pos_x;
  }

  /* MATLAB Function: '<Root>/MATLAB Function12' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function13'
   */
  test_B.updated_prev_pos[0] = rtb_FromWorkspace[0];
  test_B.updated_prev_vel[0] = (test_B.updated_prev_pos[0] - test_B.Memory[0]) /
    0.1;
  test_B.updated_prev_acc[0] = (test_B.updated_prev_vel[0] - test_B.Memory1[0]) /
    0.1;
  test_B.updated_prev_jerk[0] = (test_B.updated_prev_acc[0] - test_B.Memory2[0])
    / 0.1;
  rtb_snap[0] = (test_B.updated_prev_jerk[0] - test_B.Memory3[0]) / 0.1;
  test_B.updated_prev_pos[1] = rtb_FromWorkspace[1];
  test_B.updated_prev_vel[1] = (test_B.updated_prev_pos[1] - test_B.Memory[1]) /
    0.1;
  test_B.updated_prev_acc[1] = (test_B.updated_prev_vel[1] - test_B.Memory1[1]) /
    0.1;
  test_B.updated_prev_jerk[1] = (test_B.updated_prev_acc[1] - test_B.Memory2[1])
    / 0.1;
  rtb_snap[1] = (test_B.updated_prev_jerk[1] - test_B.Memory3[1]) / 0.1;
  test_B.updated_prev_pos[2] = rtb_FromWorkspace[2];
  test_B.updated_prev_vel[2] = (test_B.updated_prev_pos[2] - test_B.Memory[2]) /
    0.1;
  test_B.updated_prev_acc[2] = (test_B.updated_prev_vel[2] - test_B.Memory1[2]) /
    0.1;
  test_B.updated_prev_jerk[2] = (test_B.updated_prev_acc[2] - test_B.Memory2[2])
    / 0.1;
  rtb_snap[2] = (test_B.updated_prev_jerk[2] - test_B.Memory3[2]) / 0.1;

  /* Integrator: '<S22>/Integrator' */
  if (test_DW.Integrator_IWORK != 0) {
    test_X.Integrator_CSTATE = test_B.Constant3_o;
  }

  /* Integrator: '<S22>/Integrator' */
  test_B.Integrator = test_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(test_M)) {
    /* Constant: '<S22>/Constant2' */
    test_B.Constant2_p = test_P.ModelInit_Pos_y;
  }

  /* Integrator: '<S22>/Integrator1' */
  if (test_DW.Integrator1_IWORK_l != 0) {
    test_X.Integrator1_CSTATE_j = test_B.Constant2_p;
  }

  /* Integrator: '<S22>/Integrator1' */
  test_B.Integrator1 = test_X.Integrator1_CSTATE_j;
  if (rtmIsMajorTimeStep(test_M)) {
    /* Constant: '<S22>/Constant1' */
    test_B.Constant1_k = test_P.ModelInit_Pos_z;
  }

  /* Integrator: '<S22>/Integrator2' */
  if (test_DW.Integrator2_IWORK != 0) {
    test_X.Integrator2_CSTATE = test_B.Constant1_k;
  }

  /* Integrator: '<S22>/Integrator2' */
  test_B.Integrator2 = test_X.Integrator2_CSTATE;

  /* Integrator: '<S18>/Integrator6' */
  test_B.Integrator6 = test_X.Integrator6_CSTATE;

  /* Integrator: '<S18>/Integrator1' */
  test_B.Integrator1_b = test_X.Integrator1_CSTATE_i;

  /* Integrator: '<S18>/Integrator3' */
  test_B.Integrator3 = test_X.Integrator3_CSTATE_k;

  /* MATLAB Function: '<Root>/MATLAB Function7' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function9'
   *  SignalConversion generated from: '<S7>/ SFunction '
   */
  rtb_current_state[0] = test_B.Integrator;
  rtb_current_state[1] = test_B.Integrator1;
  rtb_current_state[2] = test_B.Integrator2;
  rtb_current_state[3] = test_B.Integrator6;
  rtb_current_state[4] = test_B.Integrator1_b;
  rtb_current_state[5] = test_B.Integrator3;

  /* MATLAB Function: '<Root>/MATLAB Function11' incorporates:
   *  MATLAB Function: '<S18>/MATLAB Function'
   */
  rtb_current_state_tmp = test_B.qy_unit * test_B.qy_unit;
  rtb_current_state_tmp_0 = test_B.qx_unit * test_B.qx_unit;

  /* MATLAB Function: '<Root>/MATLAB Function7' incorporates:
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator8'
   *  Integrator: '<S17>/Integrator9'
   *  MATLAB Function: '<Root>/MATLAB Function11'
   */
  rtb_current_state[6] = rt_atan2d_snf((test_B.qw_unit * test_B.qx_unit +
    test_B.qy_unit * norm_q) * 2.0, 1.0 - (rtb_current_state_tmp_0 +
    rtb_current_state_tmp) * 2.0);
  rtb_current_state[7] = asin((test_B.qw_unit * test_B.qy_unit - norm_q *
    test_B.qx_unit) * 2.0);
  rtb_current_state[8] = rt_atan2d_snf((test_B.qw_unit * norm_q + test_B.qx_unit
    * test_B.qy_unit) * 2.0, 1.0 - (norm_q * norm_q + rtb_current_state_tmp) *
    2.0);
  rtb_current_state[9] = test_X.Integrator8_CSTATE;
  rtb_current_state[10] = test_X.Integrator9_CSTATE;
  rtb_current_state[11] = test_X.Integrator10_CSTATE;

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   */
  KF = test_P.model_param.mass * test_P.model_param.g;

  /* MATLAB Function: '<Root>/MATLAB Function8' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function12'
   */
  rtb_desired_state_jerk[0] = test_B.updated_prev_jerk[0];

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   *  MATLAB Function: '<Root>/MATLAB Function12'
   *  MATLAB Function: '<Root>/MATLAB Function7'
   *  MATLAB Function: '<Root>/MATLAB Function8'
   *  MATLAB Function: '<Root>/MATLAB Function9'
   *  SignalConversion generated from: '<S7>/ SFunction '
   */
  ev_0 = test_B.Integrator6 - test_B.updated_prev_vel[0];
  Fd[0] = (((test_B.Integrator - test_B.updated_prev_pos[0]) * -test_B.K.Kp -
            test_B.K.Kv * ev_0) + KF * 0.0) + test_P.model_param.mass *
    test_B.updated_prev_acc[0];
  ev[0] = ev_0;

  /* MATLAB Function: '<Root>/MATLAB Function8' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function12'
   */
  rtb_desired_state_jerk[1] = test_B.updated_prev_jerk[1];

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   *  MATLAB Function: '<Root>/MATLAB Function12'
   *  MATLAB Function: '<Root>/MATLAB Function7'
   *  MATLAB Function: '<Root>/MATLAB Function8'
   *  MATLAB Function: '<Root>/MATLAB Function9'
   *  SignalConversion generated from: '<S7>/ SFunction '
   */
  ev_0 = test_B.Integrator1_b - test_B.updated_prev_vel[1];
  Fd[1] = (((test_B.Integrator1 - test_B.updated_prev_pos[1]) * -test_B.K.Kp -
            test_B.K.Kv * ev_0) + KF * 0.0) + test_P.model_param.mass *
    test_B.updated_prev_acc[1];
  ev[1] = ev_0;

  /* MATLAB Function: '<Root>/MATLAB Function8' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function12'
   */
  rtb_desired_state_jerk[2] = test_B.updated_prev_jerk[2];

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   *  MATLAB Function: '<Root>/MATLAB Function12'
   *  MATLAB Function: '<Root>/MATLAB Function13'
   *  MATLAB Function: '<Root>/MATLAB Function7'
   *  MATLAB Function: '<Root>/MATLAB Function8'
   *  MATLAB Function: '<Root>/MATLAB Function9'
   *  SignalConversion generated from: '<S7>/ SFunction '
   */
  ev_0 = test_B.Integrator3 - test_B.updated_prev_vel[2];
  Fd[2] = (((test_B.Integrator2 - test_B.updated_prev_pos[2]) * -test_B.K.Kp -
            test_B.K.Kv * ev_0) + KF) + test_P.model_param.mass *
    test_B.updated_prev_acc[2];
  KF = sin(rtb_current_state[8]);
  KM = cos(rtb_current_state[8]);
  zbd_xcd_norm_dot = sin(rtb_current_state[6]);
  rtb_w4 = cos(rtb_current_state[6]);
  rtb_density = sin(rtb_current_state[7]);
  R_tmp = cos(rtb_current_state[7]);
  KM_0[0] = KM;
  KM_0[3] = -KF;
  KM_0[6] = 0.0;
  KM_0[1] = KF;
  KM_0[4] = KM;
  KM_0[7] = 0.0;
  KM_0[2] = 0.0;
  e[0] = 1.0;
  KM_0[5] = 0.0;
  e[3] = 0.0;
  KM_0[8] = 1.0;
  e[6] = 0.0;
  e[1] = 0.0;
  e[4] = rtb_w4;
  e[7] = -zbd_xcd_norm_dot;
  e[2] = 0.0;
  e[5] = zbd_xcd_norm_dot;
  e[8] = rtb_w4;
  Rd1[0] = R_tmp;
  Rd1[3] = 0.0;
  Rd1[6] = rtb_density;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj <= 0; jj += 2) {
      iy = 3 * j + jj;
      _mm_storeu_pd(&KM_1[iy], _mm_set1_pd(0.0));
      tmp_8 = _mm_loadu_pd(&KM_0[jj]);
      tmp_9 = _mm_loadu_pd(&KM_1[iy]);
      _mm_storeu_pd(&KM_1[iy], _mm_add_pd(tmp_9, _mm_mul_pd(_mm_set1_pd(e[3 * j]),
        tmp_8)));
      tmp_8 = _mm_loadu_pd(&KM_0[jj + 3]);
      tmp_9 = _mm_loadu_pd(&KM_1[iy]);
      _mm_storeu_pd(&KM_1[iy], _mm_add_pd(_mm_mul_pd(_mm_set1_pd(e[3 * j + 1]),
        tmp_8), tmp_9));
      tmp_8 = _mm_loadu_pd(&KM_0[jj + 6]);
      tmp_9 = _mm_loadu_pd(&KM_1[iy]);
      _mm_storeu_pd(&KM_1[iy], _mm_add_pd(_mm_mul_pd(_mm_set1_pd(e[3 * j + 2]),
        tmp_8), tmp_9));
    }

    for (jj = 2; jj < 3; jj++) {
      iy = 3 * j + jj;
      KM_1[iy] = 0.0;
      KM_1[iy] += e[3 * j] * KM_0[jj];
      KM_1[iy] += e[3 * j + 1] * KM_0[jj + 3];
      KM_1[iy] += e[3 * j + 2] * KM_0[jj + 6];
    }

    Rd1[3 * j + 1] = f[j];
  }

  Rd1[2] = -rtb_density;
  Rd1[5] = 0.0;
  Rd1[8] = R_tmp;
  Fd_0 = 0.0;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      R[iy] = 0.0;
      R[iy] += Rd1[3 * jj] * KM_1[j];
      R[iy] += Rd1[3 * jj + 1] * KM_1[j + 3];
      R[iy] += Rd1[3 * jj + 2] * KM_1[j + 6];
    }

    Fd_0 += R[j + 6] * Fd[j];
  }

  R_tmp = test_norm(Fd);
  rtb_density = -test_P.model_param.g * 0.0;
  KM = (Fd_0 * R[6] + rtb_density) - test_B.updated_prev_acc[0];
  ev_tmp = (-test_B.K.Kp * ev[0] - test_B.K.Kv * KM) + test_P.model_param.mass *
    test_B.updated_prev_jerk[0];
  ea[0] = KM;
  KF = Fd[0] / R_tmp;
  ev[0] = ev_tmp;
  KM = (Fd_0 * R[7] + rtb_density) - test_B.updated_prev_acc[1];
  ev_tmp_0 = (-test_B.K.Kp * ev[1] - test_B.K.Kv * KM) + test_P.model_param.mass
    * test_B.updated_prev_jerk[1];
  ea[1] = KM;
  zbd_idx_1 = Fd[1] / R_tmp;
  ev[1] = ev_tmp_0;
  KM = (Fd_0 * R[8] - test_P.model_param.g) - test_B.updated_prev_acc[2];
  ev_tmp_1 = (-test_B.K.Kp * ev_0 - test_B.K.Kv * KM) + test_P.model_param.mass *
    test_B.updated_prev_jerk[2];
  zbd = Fd[2] / R_tmp;
  ea[2] = KM;
  ev[2] = ev_tmp_1;
  rtb_w4 = sin(rtb_FromWorkspace[3]);
  rtb_density = cos(rtb_FromWorkspace[3]);
  xcd_idx_1 = rtb_w4;
  KM_1[0] = 0.0;
  KM_1[3] = -zbd;
  KM_1[6] = zbd_idx_1;
  KM_1[1] = zbd;
  KM_1[4] = 0.0;
  KM_1[7] = -KF;
  KM_1[2] = -zbd_idx_1;
  KM_1[5] = KF;
  KM_1[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 6]);
    _mm_storeu_pd(&xcd_dot_tmp[j], _mm_add_pd(_mm_mul_pd(tmp_7, tmp_9),
      _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd(rtb_w4)), _mm_add_pd(_mm_mul_pd
      (tmp_8, _mm_set1_pd(rtb_density)), tmp_9))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' */
  for (j = 2; j < 3; j++) {
    xcd_dot_tmp[j] = (KM_1[j + 3] * rtb_w4 + KM_1[j] * rtb_density) + KM_1[j + 6]
      * 0.0;
  }

  KM_tmp = test_norm(xcd_dot_tmp);
  ybd_idx_0 = xcd_dot_tmp[0] / KM_tmp;
  ybd_idx_1 = xcd_dot_tmp[1] / KM_tmp;
  ybd_idx_2 = xcd_dot_tmp[2] / KM_tmp;
  KM_1[0] = 0.0;
  KM_1[3] = -ybd_idx_2;
  KM_1[6] = ybd_idx_1;
  KM_1[1] = ybd_idx_2;
  KM_1[4] = 0.0;
  KM_1[7] = -ybd_idx_0;
  KM_1[2] = -ybd_idx_1;
  KM_1[5] = ybd_idx_0;
  KM_1[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_6 = _mm_loadu_pd(&KM_1[j + 6]);
    _mm_storeu_pd(&tmp[j], _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd(zbd)),
      _mm_add_pd(_mm_mul_pd(tmp_9, _mm_set1_pd(zbd_idx_1)), _mm_add_pd
                 (_mm_mul_pd(tmp_8, _mm_set1_pd(KF)), _mm_set1_pd(0.0)))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function12'
   */
  for (j = 2; j < 3; j++) {
    tmp[j] = (KM_1[j + 3] * zbd_idx_1 + KM_1[j] * KF) + KM_1[j + 6] * zbd;
  }

  Rd1[0] = tmp[0];
  Rd1[3] = ybd_idx_0;
  Rd1[6] = KF;
  Rd1[1] = tmp[1];
  Rd1[4] = ybd_idx_1;
  Rd1[7] = zbd_idx_1;
  Rd1[2] = tmp[2];
  Rd1[5] = ybd_idx_2;
  Rd1[8] = zbd;
  for (j = 0; j < 3; j++) {
    b_R_tmp[3 * j] = Rd1[j];
    KM_0[3 * j] = R[j];
    iy = 3 * j + 1;
    b_R_tmp[iy] = Rd1[j + 3];
    KM_0[iy] = R[j + 3];
    iy = 3 * j + 2;
    b_R_tmp[iy] = Rd1[j + 6];
    KM_0[iy] = R[j + 6];
  }

  Fd_1 = 0.0;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      e[iy] = 0.0;
      e[iy] += Rd1[3 * jj] * KM_0[j];
      b_ix = 3 * jj + 1;
      e[iy] += KM_0[j + 3] * Rd1[b_ix];
      b_R_tmp_tmp = 3 * jj + 2;
      e[iy] += KM_0[j + 6] * Rd1[b_R_tmp_tmp];
      b_R[iy] = ((b_R_tmp[j + 3] * R[b_ix] + R[3 * jj] * b_R_tmp[j]) + b_R_tmp[j
                 + 6] * R[b_R_tmp_tmp]) - e[iy];
    }

    Fd_1 += Fd[j] * ev[j];
  }

  Fd_norm_dot = Fd_1 / R_tmp;
  KM_tmp = R_tmp * R_tmp;
  zbd_dot_idx_0 = (ev_tmp * R_tmp - Fd[0] * Fd_norm_dot) / KM_tmp;
  zbd_dot_idx_1 = (ev_tmp_0 * R_tmp - Fd[1] * Fd_norm_dot) / KM_tmp;
  zbd_dot_idx_2 = (ev_tmp_1 * R_tmp - Fd[2] * Fd_norm_dot) / KM_tmp;
  xcd_dot[0] = -rtb_w4 * 0.0;
  xcd_dot[1] = rtb_density * 0.0;
  KM_1[0] = 0.0;
  KM_1[3] = -zbd_dot_idx_2;
  KM_1[6] = zbd_dot_idx_1;
  KM_1[1] = zbd_dot_idx_2;
  KM_1[4] = 0.0;
  KM_1[7] = -zbd_dot_idx_0;
  KM_1[2] = -zbd_dot_idx_1;
  KM_1[5] = zbd_dot_idx_0;
  KM_1[8] = 0.0;
  c_R[0] = 0.0;
  c_R[3] = -zbd;
  c_R[6] = zbd_idx_1;
  c_R[1] = zbd;
  c_R[4] = 0.0;
  c_R[7] = -KF;
  c_R[2] = -zbd_idx_1;
  c_R[5] = KF;
  c_R[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&c_R[j]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_3 = _mm_loadu_pd(&c_R[j + 3]);
    tmp_4 = _mm_loadu_pd(&KM_1[j + 6]);
    tmp_5 = _mm_loadu_pd(&c_R[j + 6]);
    _mm_storeu_pd(&zbd_x_xcd_dot[j], _mm_add_pd(_mm_add_pd(_mm_mul_pd(tmp_4,
      tmp_9), _mm_add_pd(_mm_mul_pd(tmp_7, _mm_set1_pd(rtb_w4)), _mm_add_pd
                         (_mm_mul_pd(tmp_8, _mm_set1_pd(rtb_density)), tmp_9))),
      _mm_add_pd(_mm_mul_pd(tmp_5, tmp_9), _mm_add_pd(_mm_mul_pd(tmp_3,
      _mm_set1_pd(xcd_dot[1])), _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd
      (xcd_dot[0])), tmp_9)))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' */
  for (j = 2; j < 3; j++) {
    zbd_x_xcd_dot[j] = ((KM_1[j + 3] * rtb_w4 + KM_1[j] * rtb_density) + KM_1[j
                        + 6] * 0.0) + ((c_R[j + 3] * xcd_dot[1] + c_R[j] *
      xcd_dot[0]) + c_R[j + 6] * 0.0);
  }

  KM_1[0] = 0.0;
  KM_1[3] = -zbd;
  KM_1[6] = zbd_idx_1;
  KM_1[1] = zbd;
  KM_1[4] = 0.0;
  KM_1[7] = -KF;
  KM_1[2] = -zbd_idx_1;
  KM_1[5] = KF;
  KM_1[8] = 0.0;
  xcd_dot_tmp_0 = 0.0;
  for (j = 0; j < 3; j++) {
    xcd_dot_tmp_0 += xcd_dot_tmp[j] * zbd_x_xcd_dot[j];
    tmp[j] = (KM_1[j + 3] * rtb_w4 + KM_1[j] * rtb_density) + KM_1[j + 6] * 0.0;
  }

  KM = test_norm(tmp);
  zbd_xcd_norm_dot = xcd_dot_tmp_0 / KM;
  b_c_tmp = KM * KM;
  ybd_dot_idx_0 = (zbd_x_xcd_dot[0] * KM - xcd_dot_tmp[0] * zbd_xcd_norm_dot) /
    b_c_tmp;
  ybd_dot_idx_1 = (zbd_x_xcd_dot[1] * KM - xcd_dot_tmp[1] * zbd_xcd_norm_dot) /
    b_c_tmp;
  ybd_dot_idx_2 = (zbd_x_xcd_dot[2] * KM - xcd_dot_tmp[2] * zbd_xcd_norm_dot) /
    b_c_tmp;
  KM_1[0] = 0.0;
  KM_1[3] = -ybd_dot_idx_2;
  KM_1[6] = ybd_dot_idx_1;
  KM_1[1] = ybd_dot_idx_2;
  KM_1[4] = 0.0;
  KM_1[7] = -ybd_dot_idx_0;
  KM_1[2] = -ybd_dot_idx_1;
  KM_1[5] = ybd_dot_idx_0;
  KM_1[8] = 0.0;
  c_R[0] = 0.0;
  c_R[3] = -ybd_idx_2;
  c_R[6] = ybd_idx_1;
  c_R[1] = ybd_idx_2;
  c_R[4] = 0.0;
  c_R[7] = -ybd_idx_0;
  c_R[2] = -ybd_idx_1;
  c_R[5] = ybd_idx_0;
  c_R[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&c_R[j]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_3 = _mm_loadu_pd(&c_R[j + 3]);
    tmp_4 = _mm_loadu_pd(&KM_1[j + 6]);
    tmp_5 = _mm_loadu_pd(&c_R[j + 6]);
    _mm_storeu_pd(&tmp[j], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(zbd)),
      _mm_add_pd(_mm_mul_pd(tmp_7, _mm_set1_pd(zbd_idx_1)), _mm_add_pd
                 (_mm_mul_pd(tmp_8, _mm_set1_pd(KF)), tmp_9))));
    _mm_storeu_pd(&tmp_0[j], _mm_add_pd(_mm_mul_pd(tmp_5, _mm_set1_pd
      (zbd_dot_idx_2)), _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd(zbd_dot_idx_1)),
      _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd(zbd_dot_idx_0)), tmp_9))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator8'
   *  Integrator: '<S17>/Integrator9'
   *  MATLAB Function: '<Root>/MATLAB Function8'
   */
  for (j = 2; j < 3; j++) {
    tmp[j] = (KM_1[j + 3] * zbd_idx_1 + KM_1[j] * KF) + KM_1[j + 6] * zbd;
    tmp_0[j] = (c_R[j + 3] * zbd_dot_idx_1 + c_R[j] * zbd_dot_idx_0) + c_R[j + 6]
      * zbd_dot_idx_2;
  }

  Rd_dot[0] = tmp[0] + tmp_0[0];
  Rd_dot[3] = ybd_dot_idx_0;
  Rd_dot[6] = zbd_dot_idx_0;
  Rd_dot[1] = tmp[1] + tmp_0[1];
  Rd_dot[4] = ybd_dot_idx_1;
  Rd_dot[7] = zbd_dot_idx_1;
  Rd_dot[2] = tmp[2] + tmp_0[2];
  Rd_dot[5] = ybd_dot_idx_2;
  Rd_dot[8] = zbd_dot_idx_2;
  ev_0 = 0.0;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      c_R[iy] = 0.0;
      c_R[iy] += Rd_dot[3 * jj] * b_R_tmp[j];
      c_R[iy] += Rd_dot[3 * jj + 1] * b_R_tmp[j + 3];
      c_R[iy] += Rd_dot[3 * jj + 2] * b_R_tmp[j + 6];
    }

    ev_0 += R[j + 6] * ev[j];
  }

  wd[0] = -c_R[7];
  wd[1] = c_R[6];
  wd[2] = -c_R[3];
  KM_1[0] = 0.0;
  KM_1[3] = -test_X.Integrator10_CSTATE;
  KM_1[6] = test_X.Integrator9_CSTATE;
  KM_1[1] = test_X.Integrator10_CSTATE;
  KM_1[4] = 0.0;
  KM_1[7] = -test_X.Integrator8_CSTATE;
  KM_1[2] = -test_X.Integrator9_CSTATE;
  KM_1[5] = test_X.Integrator8_CSTATE;
  KM_1[8] = 0.0;
  Fd_1 = 0.0;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      c_R[iy] = 0.0;
      c_R[iy] += KM_1[3 * jj] * R[j];
      c_R[iy] += KM_1[3 * jj + 1] * R[j + 3];
      c_R[iy] += KM_1[3 * jj + 2] * R[j + 6];
    }

    Fd_1 += c_R[j + 6] * Fd[j];
  }

  b_c = ev_0 + Fd_1;
  KM_1[0] = 0.0;
  KM_1[3] = -test_X.Integrator10_CSTATE;
  KM_1[6] = test_X.Integrator9_CSTATE;
  KM_1[1] = test_X.Integrator10_CSTATE;
  KM_1[4] = 0.0;
  KM_1[7] = -test_X.Integrator8_CSTATE;
  KM_1[2] = -test_X.Integrator9_CSTATE;
  KM_1[5] = test_X.Integrator8_CSTATE;
  KM_1[8] = 0.0;
  ev_0 = 0.0;
  Fd_1 = 0.0;
  xcd_dot_tmp_0 = 0.0;
  b_b = rt_powd_snf(R_tmp, 3.0);
  Fd_2 = 0.0;
  Fd_3 = 0.0;
  for (j = 0; j < 3; j++) {
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      c_R[iy] = 0.0;
      c_R[iy] += KM_1[3 * jj] * R[j];
      c_R[iy] += KM_1[3 * jj + 1] * R[j + 3];
      c_R[iy] += KM_1[3 * jj + 2] * R[j + 6];
    }

    Fd_2dot[j] = (-test_B.K.Kp * ea[j] - ((R[j + 6] * b_c + c_R[j + 6] * Fd_0) /
      test_P.model_param.mass - rtb_desired_state_jerk[j]) * test_B.K.Kv) +
      test_P.model_param.mass * rtb_snap[j];
    ev_0 += ev[j] * ev[j];
    Fd_1 += Fd[j] * Fd_2dot[j];
    Fd_tmp = Fd[j] * ev[j];
    xcd_dot_tmp_0 += Fd_tmp;
    Fd_2 += Fd_tmp;
    Fd_3 += Fd_tmp;
  }

  real_T Fd_2dot_tmp;
  ev_0 += Fd_1;
  b_c = rt_powd_snf(R_tmp, 4.0);
  Fd_tmp = Fd_2 * Fd[0] * 3.0 * R_tmp * Fd_3;
  Fd_1 = (ev_0 * Fd[0] + xcd_dot_tmp_0 * ev_tmp) * b_b;
  Fd_2dot_tmp = (Fd_2dot[0] * R_tmp - ev_tmp * Fd_norm_dot) / KM_tmp;
  ev_tmp = Fd_2dot_tmp - (Fd_1 - Fd_tmp) / b_c;
  Fd_2dot[0] = Fd_2dot_tmp - (Fd_1 - Fd_tmp) / b_c;
  Fd_tmp = (Fd_2dot[1] * R_tmp - ev_tmp_0 * Fd_norm_dot) / KM_tmp - ((ev_0 * Fd
    [1] + xcd_dot_tmp_0 * ev_tmp_0) * b_b - Fd_2 * Fd[1] * 3.0 * R_tmp * Fd_3) /
    b_c;
  Fd_2dot[1] = Fd_tmp;
  b_b = (Fd_2dot[2] * R_tmp - ev_tmp_1 * Fd_norm_dot) / KM_tmp - ((ev_0 * Fd[2]
    + xcd_dot_tmp_0 * ev_tmp_1) * b_b - Fd_2 * Fd[2] * 3.0 * R_tmp * Fd_3) / b_c;
  Fd_2dot[2] = b_b;
  KM_1[0] = 0.0;
  KM_1[3] = -b_b;
  KM_1[6] = Fd_tmp;
  KM_1[1] = b_b;
  KM_1[4] = 0.0;
  KM_1[7] = -ev_tmp;
  KM_1[2] = -Fd_tmp;
  KM_1[5] = ev_tmp;
  KM_1[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 6]);
    _mm_storeu_pd(&rtb_snap[j], _mm_add_pd(_mm_mul_pd(tmp_7, tmp_9), _mm_add_pd
      (_mm_mul_pd(tmp_6, _mm_set1_pd(rtb_w4)), _mm_add_pd(_mm_mul_pd(tmp_8,
      _mm_set1_pd(rtb_density)), tmp_9))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function12'
   *  MATLAB Function: '<Root>/MATLAB Function13'
   */
  for (j = 2; j < 3; j++) {
    rtb_snap[j] = (KM_1[j + 3] * rtb_w4 + KM_1[j] * rtb_density) + KM_1[j + 6] *
      0.0;
  }

  KM_1[0] = 0.0;
  KM_1[3] = -zbd;
  KM_1[6] = zbd_idx_1;
  KM_1[1] = zbd;
  KM_1[4] = 0.0;
  KM_1[7] = -KF;
  KM_1[2] = -zbd_idx_1;
  KM_1[5] = KF;
  KM_1[8] = 0.0;
  Fd_norm_dot = -rtb_density * 0.0 - rtb_w4 * 0.0;
  ev_tmp_0 = -sin(rtb_FromWorkspace[3]) * 0.0 + cos(rtb_FromWorkspace[3]) * 0.0;
  R_tmp = 0.0;
  for (j = 0; j < 3; j++) {
    ev_tmp_1 = zbd_x_xcd_dot[j];
    R_tmp += ev_tmp_1 * ev_tmp_1;
    rtb_desired_state_jerk[j] = (KM_1[j + 3] * ev_tmp_0 + KM_1[j] * Fd_norm_dot)
      + KM_1[j + 6] * 0.0;
  }

  KM_1[0] = 0.0;
  KM_1[3] = 2.0 * -zbd_dot_idx_2;
  KM_1[6] = 2.0 * zbd_dot_idx_1;
  KM_1[1] = 2.0 * zbd_dot_idx_2;
  KM_1[4] = 0.0;
  KM_1[7] = 2.0 * -zbd_dot_idx_0;
  KM_1[2] = 2.0 * -zbd_dot_idx_1;
  KM_1[5] = 2.0 * zbd_dot_idx_0;
  KM_1[8] = 0.0;
  xcd_dot_tmp_0 = 0.0;
  for (j = 0; j < 3; j++) {
    xcd_dot_tmp_0 += ((((KM_1[j + 3] * xcd_dot[1] + KM_1[j] * xcd_dot[0]) +
                        KM_1[j + 6] * 0.0) + rtb_snap[j]) +
                      rtb_desired_state_jerk[j]) * xcd_dot_tmp[j];
  }

  KM_1[0] = 0.0;
  KM_1[3] = -zbd;
  KM_1[6] = zbd_idx_1;
  KM_1[1] = zbd;
  KM_1[4] = 0.0;
  KM_1[7] = -KF;
  KM_1[2] = -zbd_idx_1;
  KM_1[5] = KF;
  KM_1[8] = 0.0;
  KM_tmp = 0.0;
  Fd_norm_dot = zbd_xcd_norm_dot * zbd_xcd_norm_dot;
  b_c = rt_powd_snf(KM, 4.0);
  for (j = 0; j < 3; j++) {
    ev_tmp_1 = zbd_x_xcd_dot[j];
    KM_tmp += ((KM_1[j + 3] * rtb_w4 + KM_1[j] * rtb_density) + KM_1[j + 6] *
               0.0) * ev_tmp_1;
    zbd_x_xcd_dot[j] = ev_tmp_1 * zbd_xcd_norm_dot;
  }

  rtb_w4 = ((R_tmp + xcd_dot_tmp_0) * KM - KM_tmp * zbd_xcd_norm_dot) / b_c_tmp;
  KM_1[0] = 0.0;
  KM_1[3] = 2.0 * -zbd;
  KM_1[6] = 2.0 * zbd_idx_1;
  KM_1[1] = 2.0 * zbd;
  KM_1[4] = 0.0;
  KM_1[7] = 2.0 * -KF;
  KM_1[2] = 2.0 * -zbd_idx_1;
  KM_1[5] = 2.0 * KF;
  KM_1[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    __m128d tmp_1;
    __m128d tmp_2;
    __m128d tmp_a;

    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 6]);
    tmp_3 = _mm_loadu_pd(&rtb_snap[j]);
    tmp_4 = _mm_loadu_pd(&rtb_desired_state_jerk[j]);
    tmp_5 = _mm_set1_pd(KM);

    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_1 = _mm_loadu_pd(&zbd_x_xcd_dot[j]);
    tmp_2 = _mm_loadu_pd(&xcd_dot_tmp[j]);
    tmp_a = _mm_set1_pd(b_c_tmp);

    /* MATLAB Function: '<Root>/MATLAB Function5' */
    _mm_storeu_pd(&xcd_dot[j], _mm_sub_pd(_mm_div_pd(_mm_sub_pd(_mm_mul_pd
      (_mm_add_pd(tmp_3, tmp_4), tmp_5), tmp_1), tmp_a), _mm_div_pd(_mm_sub_pd
      (_mm_mul_pd(_mm_add_pd(_mm_mul_pd(tmp_2, _mm_set1_pd(rtb_w4)), tmp_1),
                  tmp_a), _mm_mul_pd(_mm_mul_pd(_mm_add_pd(_mm_mul_pd(tmp_7,
      tmp_9), _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd(xcd_idx_1)), _mm_add_pd
                         (_mm_mul_pd(tmp_8, _mm_set1_pd(rtb_density)), tmp_9))),
      tmp_5), _mm_set1_pd(Fd_norm_dot))), _mm_set1_pd(b_c))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' */
  for (j = 2; j < 3; j++) {
    R_tmp = zbd_x_xcd_dot[j];
    xcd_dot[j] = ((rtb_snap[j] + rtb_desired_state_jerk[j]) * KM - R_tmp) /
      b_c_tmp - ((xcd_dot_tmp[j] * rtb_w4 + R_tmp) * b_c_tmp - ((KM_1[j + 3] *
      xcd_idx_1 + KM_1[j] * rtb_density) + KM_1[j + 6] * 0.0) * KM * Fd_norm_dot)
      / b_c;
  }

  KM_1[0] = 0.0;
  KM_1[3] = -xcd_dot[2];
  KM_1[6] = xcd_dot[1];
  KM_1[1] = xcd_dot[2];
  KM_1[4] = 0.0;
  KM_1[7] = -xcd_dot[0];
  KM_1[2] = -xcd_dot[1];
  KM_1[5] = xcd_dot[0];
  KM_1[8] = 0.0;
  c_R[0] = 0.0;
  c_R[3] = 2.0 * -ybd_dot_idx_2;
  c_R[6] = 2.0 * ybd_dot_idx_1;
  c_R[1] = 2.0 * ybd_dot_idx_2;
  c_R[4] = 0.0;
  c_R[7] = 2.0 * -ybd_dot_idx_0;
  c_R[2] = 2.0 * -ybd_dot_idx_1;
  c_R[5] = 2.0 * ybd_dot_idx_0;
  c_R[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j]);
    tmp_9 = _mm_set1_pd(0.0);
    tmp_6 = _mm_loadu_pd(&c_R[j]);
    tmp_7 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_3 = _mm_loadu_pd(&c_R[j + 3]);
    tmp_4 = _mm_loadu_pd(&KM_1[j + 6]);
    tmp_5 = _mm_loadu_pd(&c_R[j + 6]);
    _mm_storeu_pd(&tmp[j], _mm_add_pd(_mm_mul_pd(tmp_4, _mm_set1_pd(zbd)),
      _mm_add_pd(_mm_mul_pd(tmp_7, _mm_set1_pd(zbd_idx_1)), _mm_add_pd
                 (_mm_mul_pd(tmp_8, _mm_set1_pd(KF)), tmp_9))));
    _mm_storeu_pd(&tmp_0[j], _mm_add_pd(_mm_mul_pd(tmp_5, _mm_set1_pd
      (zbd_dot_idx_2)), _mm_add_pd(_mm_mul_pd(tmp_3, _mm_set1_pd(zbd_dot_idx_1)),
      _mm_add_pd(_mm_mul_pd(tmp_6, _mm_set1_pd(zbd_dot_idx_0)), tmp_9))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' */
  for (j = 2; j < 3; j++) {
    tmp[j] = (KM_1[j + 3] * zbd_idx_1 + KM_1[j] * KF) + KM_1[j + 6] * zbd;
    tmp_0[j] = (c_R[j + 3] * zbd_dot_idx_1 + c_R[j] * zbd_dot_idx_0) + c_R[j + 6]
      * zbd_dot_idx_2;
  }

  KM_1[0] = 0.0;
  KM_1[3] = -ybd_idx_2;
  KM_1[6] = ybd_idx_1;
  KM_1[1] = ybd_idx_2;
  KM_1[4] = 0.0;
  KM_1[7] = -ybd_idx_0;
  KM_1[2] = -ybd_idx_1;
  KM_1[5] = ybd_idx_0;
  KM_1[8] = 0.0;
  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&KM_1[j + 3]);
    tmp_9 = _mm_loadu_pd(&KM_1[j]);
    tmp_6 = _mm_loadu_pd(&KM_1[j + 6]);
    tmp_7 = _mm_loadu_pd(&tmp[j]);
    tmp_3 = _mm_loadu_pd(&tmp_0[j]);
    _mm_storeu_pd(&rtb_snap[j], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_mul_pd
      (tmp_8, _mm_set1_pd(Fd_tmp)), _mm_mul_pd(tmp_9, _mm_set1_pd(Fd_2dot[0]))),
      _mm_mul_pd(tmp_6, _mm_set1_pd(b_b))), _mm_add_pd(tmp_7, tmp_3)));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' */
  for (j = 2; j < 3; j++) {
    rtb_snap[j] = ((KM_1[j + 3] * Fd_tmp + KM_1[j] * Fd_2dot[0]) + KM_1[j + 6] *
                   b_b) + (tmp[j] + tmp_0[j]);
  }

  for (j = 0; j < 3; j++) {
    KM_1[j] = rtb_snap[j];
    KM_1[j + 3] = xcd_dot[j];
    KM_1[j + 6] = Fd_2dot[j];
  }

  for (jj = 0; jj < 3; jj++) {
    for (j = 0; j < 3; j++) {
      iy = 3 * jj + j;
      R[iy] = 0.0;
      R[iy] += Rd_dot[3 * j] * Rd_dot[3 * jj];
      R[iy] += Rd_dot[3 * j + 1] * Rd_dot[3 * jj + 1];
      R[iy] += Rd_dot[3 * j + 2] * Rd_dot[3 * jj + 2];
    }

    for (j = 0; j <= 0; j += 2) {
      iy = 3 * jj + j;
      _mm_storeu_pd(&b_R_tmp_0[iy], _mm_set1_pd(0.0));
      tmp_8 = _mm_loadu_pd(&b_R_tmp[j]);
      tmp_9 = _mm_loadu_pd(&b_R_tmp_0[iy]);
      _mm_storeu_pd(&b_R_tmp_0[iy], _mm_add_pd(tmp_9, _mm_mul_pd(_mm_set1_pd
        (KM_1[3 * jj]), tmp_8)));
      tmp_8 = _mm_loadu_pd(&b_R_tmp[j + 3]);
      tmp_9 = _mm_loadu_pd(&b_R_tmp_0[iy]);
      _mm_storeu_pd(&b_R_tmp_0[iy], _mm_add_pd(_mm_mul_pd(_mm_set1_pd(KM_1[3 *
        jj + 1]), tmp_8), tmp_9));
      tmp_8 = _mm_loadu_pd(&b_R_tmp[j + 6]);
      tmp_9 = _mm_loadu_pd(&b_R_tmp_0[iy]);
      _mm_storeu_pd(&b_R_tmp_0[iy], _mm_add_pd(_mm_mul_pd(_mm_set1_pd(KM_1[3 *
        jj + 2]), tmp_8), tmp_9));
    }

    for (j = 2; j < 3; j++) {
      iy = 3 * jj + j;
      b_R_tmp_0[iy] = 0.0;
      b_R_tmp_0[iy] += KM_1[3 * jj] * b_R_tmp[j];
      b_R_tmp_0[iy] += KM_1[3 * jj + 1] * b_R_tmp[j + 3];
      b_R_tmp_0[iy] += KM_1[3 * jj + 2] * b_R_tmp[j + 6];
    }
  }

  for (j = 0; j <= 6; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' */
    tmp_8 = _mm_loadu_pd(&R[j]);
    tmp_9 = _mm_loadu_pd(&b_R_tmp_0[j]);
    _mm_storeu_pd(&c_R[j], _mm_add_pd(tmp_8, tmp_9));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator8'
   *  Integrator: '<S17>/Integrator9'
   */
  for (j = 8; j < 9; j++) {
    c_R[j] = R[j] + b_R_tmp_0[j];
  }

  tmp[0] = 0.5 * -b_R[7] * -test_B.K.KR;
  tmp[1] = 0.5 * b_R[6] * -test_B.K.KR;
  tmp[2] = 0.5 * -b_R[3] * -test_B.K.KR;
  KM_1[0] = 0.0;
  KM_1[3] = -test_X.Integrator10_CSTATE;
  KM_1[6] = test_X.Integrator9_CSTATE;
  KM_1[1] = test_X.Integrator10_CSTATE;
  KM_1[4] = 0.0;
  KM_1[7] = -test_X.Integrator8_CSTATE;
  KM_1[2] = -test_X.Integrator9_CSTATE;
  KM_1[5] = test_X.Integrator8_CSTATE;
  KM_1[8] = 0.0;
  R[0] = 0.0;
  R[3] = -test_X.Integrator10_CSTATE;
  R[6] = test_X.Integrator9_CSTATE;
  R[1] = test_X.Integrator10_CSTATE;
  R[4] = 0.0;
  R[7] = -test_X.Integrator8_CSTATE;
  R[2] = -test_X.Integrator9_CSTATE;
  R[5] = test_X.Integrator8_CSTATE;
  R[8] = 0.0;
  xcd_dot[0] = -c_R[7];
  xcd_dot[1] = c_R[6];
  xcd_dot[2] = -c_R[3];
  for (j = 0; j < 3; j++) {
    KM_tmp = 0.0;
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      KM_tmp += e[iy] * wd[jj];
      c_R[iy] = 0.0;
      b_R_tmp[iy] = 0.0;
      c_R[iy] += test_P.model_param.I[3 * jj] * KM_1[j];
      b_R_tmp[iy] += KM_0[3 * jj] * R[j];
      b_ix = 3 * jj + 1;
      c_R[iy] += KM_1[j + 3] * test_P.model_param.I[b_ix];
      b_R_tmp[iy] += R[j + 3] * KM_0[b_ix];
      b_ix = 3 * jj + 2;
      c_R[iy] += KM_1[j + 6] * test_P.model_param.I[b_ix];
      b_R_tmp[iy] += R[j + 6] * KM_0[b_ix];
    }

    tmp_0[j] = 0.0;
    rtb_desired_state_jerk[j] = 0.0;
    zbd_idx_1 = 0.0;
    for (jj = 0; jj < 3; jj++) {
      iy = 3 * jj + j;
      b_R[iy] = 0.0;
      b_R[iy] += Rd1[3 * jj] * b_R_tmp[j];
      b_R[iy] += Rd1[3 * jj + 1] * b_R_tmp[j + 3];
      b_R[iy] += Rd1[3 * jj + 2] * b_R_tmp[j + 6];
      zbd_idx_1 += rtb_current_state[jj + 9] * c_R[iy];
      tmp_0[j] += b_R[iy] * wd[jj];
      rtb_desired_state_jerk[j] += e[iy] * xcd_dot[jj];
    }

    rtb_snap[j] = tmp_0[j] - rtb_desired_state_jerk[j];
    Fd_2dot[j] = (tmp[j] - (rtb_current_state[j + 9] - KM_tmp) *
                  test_B.K.K_omega) + zbd_idx_1;
  }

  for (j = 0; j <= 0; j += 2) {
    /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
     *  Constant: '<Root>/Constant30'
     */
    tmp_8 = _mm_loadu_pd(&Fd_2dot[j]);
    _mm_storeu_pd(&xcd_dot[j], _mm_sub_pd(tmp_8, _mm_add_pd(_mm_mul_pd
      (_mm_loadu_pd(&test_P.model_param.I[j + 6]), _mm_set1_pd(rtb_snap[2])),
      _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&test_P.model_param.I[j + 3]),
      _mm_set1_pd(rtb_snap[1])), _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
      (&test_P.model_param.I[j]), _mm_set1_pd(rtb_snap[0])), _mm_set1_pd(0.0))))));
  }

  /* MATLAB Function: '<Root>/MATLAB Function5' incorporates:
   *  Constant: '<Root>/Constant30'
   */
  for (j = 2; j < 3; j++) {
    xcd_dot[j] = Fd_2dot[j] - ((test_P.model_param.I[j + 3] * rtb_snap[1] +
      test_P.model_param.I[j] * rtb_snap[0]) + test_P.model_param.I[j + 6] *
      rtb_snap[2]);
  }

  /* MATLAB Function: '<Root>/MATLAB Function6' incorporates:
   *  Constant: '<Root>/Constant35'
   *  Constant: '<Root>/Constant36'
   *  Constant: '<Root>/Constant37'
   *  Constant: '<Root>/Constant38'
   *  Constant: '<Root>/Constant39'
   *  MATLAB Function: '<Root>/MATLAB Function5'
   */
  rtb_trajectory[0] = Fd_0;
  rtb_trajectory[1] = xcd_dot[0];
  rtb_trajectory[2] = xcd_dot[1];
  rtb_trajectory[3] = xcd_dot[2];
  KF = test_P.C_T * test_P.p * rt_powd_snf(test_P.D_p, 4.0) / 39.478417604357432;
  KM = test_P.C_M * test_P.p * rt_powd_snf(test_P.D_p, 5.0) / 39.478417604357432;
  A[0] = KF;
  A[4] = KF;
  A[8] = KF;
  A[12] = KF;
  A[1] = 0.0;
  rtb_density = KF * test_P.ModelParam_d;
  A[5] = rtb_density;
  A[9] = 0.0;
  zbd_xcd_norm_dot = -KF * test_P.ModelParam_d;
  A[13] = zbd_xcd_norm_dot;
  A[2] = zbd_xcd_norm_dot;
  A[6] = 0.0;
  A[10] = rtb_density;
  A[14] = 0.0;
  A[3] = KM;
  A[7] = -KM;
  A[11] = KM;
  A[15] = -KM;
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  for (j = 0; j < 3; j++) {
    int32_T vectorUB;
    jj = j * 5;
    iy = 4 - j;
    b_ix = 0;
    KF = fabs(A[jj]);
    for (b_R_tmp_tmp = 2; b_R_tmp_tmp <= iy; b_R_tmp_tmp++) {
      KM = fabs(A[(jj + b_R_tmp_tmp) - 1]);
      if (KM > KF) {
        b_ix = b_R_tmp_tmp - 1;
        KF = KM;
      }
    }

    if (A[jj + b_ix] != 0.0) {
      if (b_ix != 0) {
        iy = j + b_ix;
        ipiv[j] = (int8_T)(iy + 1);
        KF = A[j];
        A[j] = A[iy];
        A[iy] = KF;
        KF = A[j + 4];
        A[j + 4] = A[iy + 4];
        A[iy + 4] = KF;
        KF = A[j + 8];
        A[j + 8] = A[iy + 8];
        A[iy + 8] = KF;
        KF = A[j + 12];
        A[j + 12] = A[iy + 12];
        A[iy + 12] = KF;
      }

      iy = (jj - j) + 4;
      b_R_tmp_tmp = (((((iy - jj) - 1) / 2) << 1) + jj) + 2;
      vectorUB = b_R_tmp_tmp - 2;
      for (b_ix = jj + 2; b_ix <= vectorUB; b_ix += 2) {
        tmp_8 = _mm_loadu_pd(&A[b_ix - 1]);
        _mm_storeu_pd(&A[b_ix - 1], _mm_div_pd(tmp_8, _mm_set1_pd(A[jj])));
      }

      for (b_ix = b_R_tmp_tmp; b_ix <= iy; b_ix++) {
        A[b_ix - 1] /= A[jj];
      }
    }

    iy = 2 - j;
    b_ix = jj + 6;
    for (b_R_tmp_tmp = 0; b_R_tmp_tmp <= iy; b_R_tmp_tmp++) {
      KM_tmp = A[((b_R_tmp_tmp << 2) + jj) + 4];
      if (KM_tmp != 0.0) {
        vectorUB = (b_ix - j) + 2;
        for (ijA = b_ix; ijA <= vectorUB; ijA++) {
          A[ijA - 1] += A[((jj + ijA) - b_ix) + 1] * -KM_tmp;
        }
      }

      b_ix += 4;
    }
  }

  if (ipiv[0] != 1) {
    rtb_trajectory[0] = rtb_trajectory[ipiv[0] - 1];
    rtb_trajectory[ipiv[0] - 1] = Fd_0;
  }

  if (ipiv[1] != 2) {
    KF = rtb_trajectory[1];
    rtb_trajectory[1] = rtb_trajectory[ipiv[1] - 1];
    rtb_trajectory[ipiv[1] - 1] = KF;
  }

  if (ipiv[2] != 3) {
    KF = rtb_trajectory[2];
    rtb_trajectory[2] = rtb_trajectory[ipiv[2] - 1];
    rtb_trajectory[ipiv[2] - 1] = KF;
  }

  for (j = 0; j < 4; j++) {
    jj = j << 2;
    if (rtb_trajectory[j] != 0.0) {
      for (iy = j + 2; iy < 5; iy++) {
        rtb_trajectory[iy - 1] -= A[(iy + jj) - 1] * rtb_trajectory[j];
      }
    }
  }

  for (j = 3; j >= 0; j--) {
    jj = j << 2;
    if (rtb_trajectory[j] != 0.0) {
      rtb_trajectory[j] /= A[j + jj];
      for (b_ix = 0; b_ix < j; b_ix++) {
        rtb_trajectory[b_ix] -= A[b_ix + jj] * rtb_trajectory[j];
      }
    }
  }

  KF = sqrt(rtb_trajectory[0]);
  KM = sqrt(rtb_trajectory[1]);
  zbd_xcd_norm_dot = sqrt(rtb_trajectory[2]);
  rtb_w4 = sqrt(rtb_trajectory[3]);

  /* End of MATLAB Function: '<Root>/MATLAB Function6' */
  if (rtmIsMajorTimeStep(test_M)) {
  }

  /* MATLAB Function: '<S11>/MATLAB Function' */
  b_c = sqrt((test_B.Integrator - 10.0) * (test_B.Integrator - 10.0) +
             (test_B.Integrator1 - 10.0) * (test_B.Integrator1 - 10.0));
  rtb_density = (exp(-(b_c * b_c) / 2.0) * -0.14700000000000002 + 1.225) / 1.225;

  /* MATLAB Function: '<S12>/MATLAB Function' */
  if (rtIsNaN(KF)) {
    R_tmp = (rtNaN);
  } else {
    R_tmp = (KF > 0.0);
  }

  if (rtIsNaN(KM)) {
    KM_tmp = (rtNaN);
  } else {
    KM_tmp = (KM > 0.0);
  }

  if (rtIsNaN(zbd_xcd_norm_dot)) {
    zbd_idx_1 = (rtNaN);
  } else {
    zbd_idx_1 = (zbd_xcd_norm_dot > 0.0);
  }

  if (rtIsNaN(rtb_w4)) {
    Fd_norm_dot = (rtNaN);
  } else {
    Fd_norm_dot = (rtb_w4 > 0.0);
  }

  Fd_0 = KM * KM;
  zbd = rtb_w4 * rtb_w4;
  xcd_idx_1 = KF * KF;
  ybd_idx_0 = zbd_xcd_norm_dot * zbd_xcd_norm_dot;

  /* MATLAB Function: '<S18>/MATLAB Function' incorporates:
   *  Constant: '<S12>/Constant'
   *  Constant: '<S18>/Constant7'
   *  Constant: '<S18>/Constant8'
   *  MATLAB Function: '<S12>/MATLAB Function'
   */
  Fd_norm_dot = (((xcd_idx_1 * R_tmp + Fd_0 * KM_tmp) + ybd_idx_0 * zbd_idx_1) +
                 zbd * Fd_norm_dot) * (rtb_density * test_P.ModelParam_c_T)
    * (1.0 / test_P.ModelParam_m);
  test_B.v_x_dot = (2.0 * test_B.qx_unit * norm_q + 2.0 * test_B.qy_unit *
                    test_B.qw_unit) * Fd_norm_dot;
  test_B.v_y_dot = (2.0 * test_B.qy_unit * norm_q - 2.0 * test_B.qx_unit *
                    test_B.qw_unit) * Fd_norm_dot;
  test_B.v_z_dot = ((1.0 - rtb_current_state_tmp_0 * 2.0) -
                    rtb_current_state_tmp * 2.0) * Fd_norm_dot -
    test_P.ModelParam_g;

  /* MATLAB Function: '<S17>/MATLAB Function' */
  Fd_norm_dot = ((-KF + KM) - zbd_xcd_norm_dot) + rtb_w4;

  /* MATLAB Function: '<S12>/MATLAB Function' incorporates:
   *  Constant: '<S12>/Constant'
   *  Constant: '<S12>/Constant2'
   */
  if (rtIsNaN(KM)) {
    KM_tmp = (rtNaN);
  } else {
    KM_tmp = (KM > 0.0);
  }

  if (rtIsNaN(rtb_w4)) {
    zbd_idx_1 = (rtNaN);
  } else {
    zbd_idx_1 = (rtb_w4 > 0.0);
  }

  rtb_current_state_tmp = rtb_density * test_P.ModelParam_d *
    test_P.ModelParam_c_T;

  /* MATLAB Function: '<S17>/MATLAB Function' incorporates:
   *  Constant: '<S17>/Constant10'
   *  Constant: '<S17>/Constant11'
   *  Constant: '<S17>/Constant12'
   *  Constant: '<S17>/Constant9'
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator9'
   *  MATLAB Function: '<S12>/MATLAB Function'
   */
  test_B.p_dot = (((Fd_0 * KM_tmp - zbd * zbd_idx_1) * rtb_current_state_tmp +
                   test_X.Integrator9_CSTATE * test_X.Integrator10_CSTATE *
                   (test_P.ModelParam_I_yy - test_P.ModelParam_I_zz)) -
                  test_P.ModelParam_J_RP * test_X.Integrator9_CSTATE *
                  Fd_norm_dot) * (1.0 / test_P.ModelParam_I_xx);

  /* MATLAB Function: '<S12>/MATLAB Function' */
  if (rtIsNaN(zbd_xcd_norm_dot)) {
    KM_tmp = (rtNaN);
  } else {
    KM_tmp = (zbd_xcd_norm_dot > 0.0);
  }

  /* MATLAB Function: '<S17>/MATLAB Function' incorporates:
   *  Constant: '<S17>/Constant10'
   *  Constant: '<S17>/Constant11'
   *  Constant: '<S17>/Constant12'
   *  Constant: '<S17>/Constant9'
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator8'
   *  MATLAB Function: '<S12>/MATLAB Function'
   */
  test_B.q_dot = (((xcd_idx_1 * -R_tmp + ybd_idx_0 * KM_tmp) *
                   rtb_current_state_tmp + test_X.Integrator8_CSTATE *
                   test_X.Integrator10_CSTATE * (test_P.ModelParam_I_zz -
    test_P.ModelParam_I_xx)) + test_P.ModelParam_J_RP *
                  test_X.Integrator8_CSTATE * Fd_norm_dot) * (1.0 /
    test_P.ModelParam_I_yy);

  /* MATLAB Function: '<S12>/MATLAB Function' */
  if (rtIsNaN(KF)) {
    KM_tmp = (rtNaN);
  } else {
    KM_tmp = (KF > 0.0);
  }

  if (rtIsNaN(KM)) {
    zbd_idx_1 = (rtNaN);
  } else {
    zbd_idx_1 = (KM > 0.0);
  }

  if (rtIsNaN(zbd_xcd_norm_dot)) {
    Fd_norm_dot = (rtNaN);
  } else {
    Fd_norm_dot = (zbd_xcd_norm_dot > 0.0);
  }

  if (rtIsNaN(rtb_w4)) {
    rtb_current_state_tmp = (rtNaN);
  } else {
    rtb_current_state_tmp = (rtb_w4 > 0.0);
  }

  /* MATLAB Function: '<S17>/MATLAB Function' incorporates:
   *  Constant: '<S12>/Constant1'
   *  Constant: '<S17>/Constant10'
   *  Constant: '<S17>/Constant11'
   *  Constant: '<S17>/Constant9'
   *  Integrator: '<S17>/Integrator8'
   *  Integrator: '<S17>/Integrator9'
   *  MATLAB Function: '<S12>/MATLAB Function'
   */
  test_B.r_dot = ((((xcd_idx_1 * KM_tmp - Fd_0 * zbd_idx_1) + ybd_idx_0 *
                    Fd_norm_dot) - zbd * rtb_current_state_tmp) * (rtb_density *
    test_P.ModelParam_c_M) + test_X.Integrator8_CSTATE *
                  test_X.Integrator9_CSTATE * (test_P.ModelParam_I_xx -
    test_P.ModelParam_I_yy)) * (1.0 / test_P.ModelParam_I_zz);
  if (rtmIsMajorTimeStep(test_M)) {
  }

  /* MATLAB Function: '<S21>/MATLAB Function2' incorporates:
   *  Integrator: '<S17>/Integrator10'
   *  Integrator: '<S17>/Integrator8'
   *  Integrator: '<S17>/Integrator9'
   */
  test_B.qw_dot = (((0.0 * test_B.qw_unit - test_X.Integrator8_CSTATE *
                     test_B.qx_unit) - test_X.Integrator9_CSTATE *
                    test_B.qy_unit) - test_X.Integrator10_CSTATE * norm_q) * 0.5;
  test_B.qx_dot = ((0.0 * test_B.qx_unit + test_B.qw_unit *
                    test_X.Integrator8_CSTATE) + (test_X.Integrator9_CSTATE *
    norm_q - test_B.qy_unit * test_X.Integrator10_CSTATE)) * 0.5;
  test_B.qy_dot = ((0.0 * test_B.qy_unit + test_B.qw_unit *
                    test_X.Integrator9_CSTATE) + (test_B.qx_unit *
    test_X.Integrator10_CSTATE - test_X.Integrator8_CSTATE * norm_q)) * 0.5;
  test_B.qz_dot = ((0.0 * norm_q + test_B.qw_unit * test_X.Integrator10_CSTATE)
                   + (test_X.Integrator8_CSTATE * test_B.qy_unit -
                      test_B.qx_unit * test_X.Integrator9_CSTATE)) * 0.5;
  if (rtmIsMajorTimeStep(test_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(test_M->rtwLogInfo, (test_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(test_M)) {
    /* Update for Integrator: '<S21>/Integrator3' */
    test_DW.Integrator3_IWORK = 0;

    /* Update for Integrator: '<S21>/Integrator1' */
    test_DW.Integrator1_IWORK = 0;

    /* Update for Integrator: '<S21>/Integrator4' */
    test_DW.Integrator4_IWORK = 0;

    /* Update for Integrator: '<S21>/Integrator5' */
    test_DW.Integrator5_IWORK = 0;
    if (rtmIsMajorTimeStep(test_M)) {
      /* Update for Memory: '<Root>/Memory' */
      test_DW.Memory_PreviousInput[0] = test_B.updated_prev_pos[0];

      /* Update for Memory: '<Root>/Memory1' */
      test_DW.Memory1_PreviousInput[0] = test_B.updated_prev_vel[0];

      /* Update for Memory: '<Root>/Memory2' */
      test_DW.Memory2_PreviousInput[0] = test_B.updated_prev_acc[0];

      /* Update for Memory: '<Root>/Memory3' */
      test_DW.Memory3_PreviousInput[0] = test_B.updated_prev_jerk[0];

      /* Update for Memory: '<Root>/Memory' */
      test_DW.Memory_PreviousInput[1] = test_B.updated_prev_pos[1];

      /* Update for Memory: '<Root>/Memory1' */
      test_DW.Memory1_PreviousInput[1] = test_B.updated_prev_vel[1];

      /* Update for Memory: '<Root>/Memory2' */
      test_DW.Memory2_PreviousInput[1] = test_B.updated_prev_acc[1];

      /* Update for Memory: '<Root>/Memory3' */
      test_DW.Memory3_PreviousInput[1] = test_B.updated_prev_jerk[1];

      /* Update for Memory: '<Root>/Memory' */
      test_DW.Memory_PreviousInput[2] = test_B.updated_prev_pos[2];

      /* Update for Memory: '<Root>/Memory1' */
      test_DW.Memory1_PreviousInput[2] = test_B.updated_prev_vel[2];

      /* Update for Memory: '<Root>/Memory2' */
      test_DW.Memory2_PreviousInput[2] = test_B.updated_prev_acc[2];

      /* Update for Memory: '<Root>/Memory3' */
      test_DW.Memory3_PreviousInput[2] = test_B.updated_prev_jerk[2];
    }

    /* Update for Integrator: '<S22>/Integrator' */
    test_DW.Integrator_IWORK = 0;

    /* Update for Integrator: '<S22>/Integrator1' */
    test_DW.Integrator1_IWORK_l = 0;

    /* Update for Integrator: '<S22>/Integrator2' */
    test_DW.Integrator2_IWORK = 0;
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(test_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(test_M)!=-1) &&
          !((rtmGetTFinal(test_M)-(((test_M->Timing.clockTick1+
               test_M->Timing.clockTickH1* 4294967296.0)) * 0.1)) >
            (((test_M->Timing.clockTick1+test_M->Timing.clockTickH1*
               4294967296.0)) * 0.1) * (DBL_EPSILON))) {
        rtmSetErrorStatus(test_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&test_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++test_M->Timing.clockTick0)) {
      ++test_M->Timing.clockTickH0;
    }

    test_M->Timing.t[0] = rtsiGetSolverStopTime(&test_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.1s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.1, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      test_M->Timing.clockTick1++;
      if (!test_M->Timing.clockTick1) {
        test_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void test_derivatives(void)
{
  XDot_test_T *_rtXdot;
  _rtXdot = ((XDot_test_T *) test_M->derivs);

  /* Derivatives for Integrator: '<S21>/Integrator3' */
  _rtXdot->Integrator3_CSTATE = test_B.qw_dot;

  /* Derivatives for Integrator: '<S21>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = test_B.qx_dot;

  /* Derivatives for Integrator: '<S21>/Integrator4' */
  _rtXdot->Integrator4_CSTATE = test_B.qy_dot;

  /* Derivatives for Integrator: '<S21>/Integrator5' */
  _rtXdot->Integrator5_CSTATE = test_B.qz_dot;

  /* Derivatives for Integrator: '<S22>/Integrator' */
  _rtXdot->Integrator_CSTATE = test_B.Integrator6;

  /* Derivatives for Integrator: '<S22>/Integrator1' */
  _rtXdot->Integrator1_CSTATE_j = test_B.Integrator1_b;

  /* Derivatives for Integrator: '<S22>/Integrator2' */
  _rtXdot->Integrator2_CSTATE = test_B.Integrator3;

  /* Derivatives for Integrator: '<S18>/Integrator6' */
  _rtXdot->Integrator6_CSTATE = test_B.v_x_dot;

  /* Derivatives for Integrator: '<S18>/Integrator1' */
  _rtXdot->Integrator1_CSTATE_i = test_B.v_y_dot;

  /* Derivatives for Integrator: '<S18>/Integrator3' */
  _rtXdot->Integrator3_CSTATE_k = test_B.v_z_dot;

  /* Derivatives for Integrator: '<S17>/Integrator8' */
  _rtXdot->Integrator8_CSTATE = test_B.p_dot;

  /* Derivatives for Integrator: '<S17>/Integrator9' */
  _rtXdot->Integrator9_CSTATE = test_B.q_dot;

  /* Derivatives for Integrator: '<S17>/Integrator10' */
  _rtXdot->Integrator10_CSTATE = test_B.r_dot;
}

/* Model initialize function */
void test_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)test_M, 0,
                sizeof(RT_MODEL_test_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&test_M->solverInfo, &test_M->Timing.simTimeStep);
    rtsiSetTPtr(&test_M->solverInfo, &rtmGetTPtr(test_M));
    rtsiSetStepSizePtr(&test_M->solverInfo, &test_M->Timing.stepSize0);
    rtsiSetdXPtr(&test_M->solverInfo, &test_M->derivs);
    rtsiSetContStatesPtr(&test_M->solverInfo, (real_T **) &test_M->contStates);
    rtsiSetNumContStatesPtr(&test_M->solverInfo, &test_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&test_M->solverInfo,
      &test_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&test_M->solverInfo,
      &test_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&test_M->solverInfo,
      &test_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&test_M->solverInfo, (&rtmGetErrorStatus(test_M)));
    rtsiSetRTModelPtr(&test_M->solverInfo, test_M);
  }

  rtsiSetSimTimeStep(&test_M->solverInfo, MAJOR_TIME_STEP);
  test_M->intgData.y = test_M->odeY;
  test_M->intgData.f[0] = test_M->odeF[0];
  test_M->intgData.f[1] = test_M->odeF[1];
  test_M->intgData.f[2] = test_M->odeF[2];
  test_M->contStates = ((X_test_T *) &test_X);
  rtsiSetSolverData(&test_M->solverInfo, (void *)&test_M->intgData);
  rtsiSetIsMinorTimeStepWithModeChange(&test_M->solverInfo, false);
  rtsiSetSolverName(&test_M->solverInfo,"ode3");
  rtmSetTPtr(test_M, &test_M->Timing.tArray[0]);
  rtmSetTFinal(test_M, 100.0);
  test_M->Timing.stepSize0 = 0.1;
  rtmSetFirstInitCond(test_M, 1);

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    test_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(test_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(test_M->rtwLogInfo, (NULL));
    rtliSetLogT(test_M->rtwLogInfo, "tout");
    rtliSetLogX(test_M->rtwLogInfo, "");
    rtliSetLogXFinal(test_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(test_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(test_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(test_M->rtwLogInfo, 0);
    rtliSetLogDecimation(test_M->rtwLogInfo, 1);
    rtliSetLogY(test_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(test_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(test_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &test_B), 0,
                sizeof(B_test_T));

  /* states (continuous) */
  {
    (void) memset((void *)&test_X, 0,
                  sizeof(X_test_T));
  }

  /* states (dwork) */
  (void) memset((void *)&test_DW, 0,
                sizeof(DW_test_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(test_M->rtwLogInfo, 0.0, rtmGetTFinal(test_M),
    test_M->Timing.stepSize0, (&rtmGetErrorStatus(test_M)));

  /* Start for FromWorkspace: '<Root>/From Workspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
      9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
      21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0,
      33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0 } ;

    static real_T pDataValues0[] = { 0.34562211981566948, 0.34962706959733086,
      0.40147026072114367, 0.59074567347457119, 1.0135947934766936,
      1.743658494584998, 2.8160330850750777, 4.2218741147276049,
      5.9112915404569684, 7.8021798481159106, 9.7926267281106014,
      9.7926267301651535, 11.774544436224952, 13.646231730709696,
      15.322207294945816, 16.739939491667545, 17.863730341407251,
      18.686075348989988, 19.226821347929167, 19.5304445305204,
      19.661770831430339, 19.70046083257764, 19.700460826364054,
      19.734581375893946, 19.853589157289559, 20.14108162399036,
      20.667731951542205, 21.48485483883519, 22.61905303306342,
      24.06839447808634, 25.8005699858713, 27.753481330697, 29.838709665797545,
      29.838709665702655, 31.948313536032767, 33.965331059850151,
      35.777619201524786, 37.293158748455852, 38.45421195411128,
      39.247645108683777, 39.708727672742505, 39.915719609259206,
      39.9725585493879, 39.97695842737712, 0.14577259475219151,
      0.14983497621237957, 0.20244452530669788, 0.39461776058410303,
      0.82418902174155739, 1.56632422183915, 2.657161278987588,
      4.0882101606842429, 5.809145473973949, 7.7366245346107005,
      9.7667638483965042, 9.76676385317312, 11.788907473051157,
      13.69838425356024, 15.406581889774266, 16.847945334067379,
      17.984138049187468, 18.805668114450164, 19.331281732191684,
      19.605426681618926, 19.694088267195035, 19.679300308698917,
      19.679300278841634, 19.652633725197774, 19.707970143988437,
      19.933913409207126, 20.406286551199607, 21.181206804753536,
      22.289236560018931, 23.731107956173503, 25.475518857746845,
      27.459497953517442, 29.591836717896481, 29.591836807694055,
      31.760085402264398, 33.841529821571271, 35.71777600633208,
      37.29096077678561, 38.498817855630918, 39.325747478307093,
      39.807037425651259, 40.023383313972232, 40.082855977577047,
      40.087463778787964, -0.14577259475219151, -0.14077624369170288,
      -0.076786632715876046, 0.15380204920327917, 0.66128039642552938,
      1.5227449481667157, 2.7642589944196843, 4.3574497493128028,
      6.2269386149470192, 8.2650012587524255, 10.349854227405242,
      10.349854242291185, 12.364035765930913, 14.209729306012973,
      15.817346123461579, 17.147769577069575, 18.190573026614917,
      18.959185340473383, 19.48386013251173, 19.803304853045731,
      19.955825858647351, 19.970845585585423, 19.970845501289936,
      19.861832561388944, 19.623726389831376, 19.235623162106119,
      18.666060494078849, 17.880526479681034, 16.850161459589945,
      15.560497012932517, 14.019076664045816, 12.26080279632694,
      10.349854265205144, 10.349854165764526, 8.37702000286543,
      6.4514036790776386, 4.6865343800134207, 3.1831169768017666,
      2.0117343745992713, 1.1989249859121553, 0.72005897859287593,
      0.502436948374875, 0.4420346658089116, 0.43731754746453255, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
      -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 } ;

    test_DW.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    test_DW.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    test_DW.FromWorkspace_IWORK.PrevIndex = 0;
  }

  /* Start for Constant: '<S21>/Constant1' */
  test_B.Constant1 = test_P.ModelInit_Att_qw;

  /* Start for Constant: '<S21>/Constant4' */
  test_B.Constant4 = test_P.ModelInit_Att_qx;

  /* Start for Constant: '<S21>/Constant2' */
  test_B.Constant2 = test_P.ModelInit_Att_qy;

  /* Start for Constant: '<S21>/Constant3' */
  test_B.Constant3 = test_P.ModelInit_Att_qz;

  /* Start for Constant: '<S22>/Constant3' */
  test_B.Constant3_o = test_P.ModelInit_Pos_x;

  /* Start for Constant: '<S22>/Constant2' */
  test_B.Constant2_p = test_P.ModelInit_Pos_y;

  /* Start for Constant: '<S22>/Constant1' */
  test_B.Constant1_k = test_P.ModelInit_Pos_z;

  /* InitializeConditions for Integrator: '<S21>/Integrator3' incorporates:
   *  Integrator: '<S21>/Integrator1'
   */
  if (rtmIsFirstInitCond(test_M)) {
    test_X.Integrator3_CSTATE = 1.0;
    test_X.Integrator1_CSTATE = 0.0;
  }

  test_DW.Integrator3_IWORK = 1;

  /* End of InitializeConditions for Integrator: '<S21>/Integrator3' */

  /* InitializeConditions for Integrator: '<S21>/Integrator1' */
  test_DW.Integrator1_IWORK = 1;

  /* InitializeConditions for Integrator: '<S21>/Integrator4' incorporates:
   *  Integrator: '<S21>/Integrator5'
   */
  if (rtmIsFirstInitCond(test_M)) {
    test_X.Integrator4_CSTATE = 0.0;
    test_X.Integrator5_CSTATE = 0.0;
  }

  test_DW.Integrator4_IWORK = 1;

  /* End of InitializeConditions for Integrator: '<S21>/Integrator4' */

  /* InitializeConditions for Integrator: '<S21>/Integrator5' */
  test_DW.Integrator5_IWORK = 1;

  /* InitializeConditions for Memory: '<Root>/Memory' */
  test_DW.Memory_PreviousInput[0] = test_P.Memory_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory1' */
  test_DW.Memory1_PreviousInput[0] = test_P.Memory1_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory2' */
  test_DW.Memory2_PreviousInput[0] = test_P.Memory2_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory3' */
  test_DW.Memory3_PreviousInput[0] = test_P.Memory3_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory' */
  test_DW.Memory_PreviousInput[1] = test_P.Memory_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory1' */
  test_DW.Memory1_PreviousInput[1] = test_P.Memory1_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory2' */
  test_DW.Memory2_PreviousInput[1] = test_P.Memory2_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory3' */
  test_DW.Memory3_PreviousInput[1] = test_P.Memory3_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory' */
  test_DW.Memory_PreviousInput[2] = test_P.Memory_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory1' */
  test_DW.Memory1_PreviousInput[2] = test_P.Memory1_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory2' */
  test_DW.Memory2_PreviousInput[2] = test_P.Memory2_InitialCondition;

  /* InitializeConditions for Memory: '<Root>/Memory3' */
  test_DW.Memory3_PreviousInput[2] = test_P.Memory3_InitialCondition;

  /* InitializeConditions for Integrator: '<S22>/Integrator' incorporates:
   *  Integrator: '<S22>/Integrator1'
   */
  if (rtmIsFirstInitCond(test_M)) {
    test_X.Integrator_CSTATE = 0.0;
    test_X.Integrator1_CSTATE_j = 0.0;
  }

  test_DW.Integrator_IWORK = 1;

  /* End of InitializeConditions for Integrator: '<S22>/Integrator' */

  /* InitializeConditions for Integrator: '<S22>/Integrator1' */
  test_DW.Integrator1_IWORK_l = 1;

  /* InitializeConditions for Integrator: '<S22>/Integrator2' */
  if (rtmIsFirstInitCond(test_M)) {
    test_X.Integrator2_CSTATE = 0.0;
  }

  test_DW.Integrator2_IWORK = 1;

  /* End of InitializeConditions for Integrator: '<S22>/Integrator2' */

  /* InitializeConditions for Integrator: '<S18>/Integrator6' */
  test_X.Integrator6_CSTATE = test_P.Integrator6_IC;

  /* InitializeConditions for Integrator: '<S18>/Integrator1' */
  test_X.Integrator1_CSTATE_i = test_P.Integrator1_IC;

  /* InitializeConditions for Integrator: '<S18>/Integrator3' */
  test_X.Integrator3_CSTATE_k = test_P.Integrator3_IC;

  /* InitializeConditions for Integrator: '<S17>/Integrator8' */
  test_X.Integrator8_CSTATE = test_P.Integrator8_IC;

  /* InitializeConditions for Integrator: '<S17>/Integrator9' */
  test_X.Integrator9_CSTATE = test_P.Integrator9_IC;

  /* InitializeConditions for Integrator: '<S17>/Integrator10' */
  test_X.Integrator10_CSTATE = test_P.Integrator10_IC;

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(test_M)) {
    rtmSetFirstInitCond(test_M, 0);
  }
}

/* Model terminate function */
void test_terminate(void)
{
  /* (no terminate code required) */
}
