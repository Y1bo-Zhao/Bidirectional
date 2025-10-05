function [sys,x0,str,ts,simStateCompliance] = NLSEF_fal(t,x,u,flag,CtrlPara)
%% flag 取值
switch flag,    % flag 默认值为 0
  case 0,       % 初始化
    [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes;  
  case 3,       % 计算 S-function 的输出 
    sys=mdlOutputs(t,x,u,CtrlPara); 
  case {1,2,4,9}   % 模块中不需要的 flag 值 
    sys=[];      
  otherwise     % 错误值处理
    DAStudio.error('Simulink:blocks:unhandledFlag', num2str(flag));
end

%% mdlInitializeSizes  初始化子函数
%  flag = 0 时执行，定义 S-funciton 模块的基本特性
function [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes
sizes = simsizes;           % 调用 simsizes 以装载初始化信息结构
sizes.NumContStates  = 0;   % 连续状态的数量
sizes.NumDiscStates  = 0;   % 离散状态的数量
sizes.NumOutputs     = 1;   % 输出的数量
sizes.NumInputs      = 2;   % 输入的数量
sizes.DirFeedthrough = 1;   % 设置输入端口的信号是否在输出子函数中使用，1 表示有直接联系，即在输出的 sys 中与 u 有直接的联系
sizes.NumSampleTimes = 1;   % 采样时间的数量
sys = simsizes(sizes);      % 再次调用 simsizes ，将 sizes 结构中的信息传递给 sys，sys 是一个保持 Simulink 所用信息的向量
x0  = [];                   % 状态变量的初始值
str = [];                   % 保留变量
ts  = [-1 0];               % 采样时间
simStateCompliance = 'UnknownSimState';  % 指定仿真状态的保存和创建方法

%% mdlOutputs  输出子函数
%  flag = 3 时执行，计算 S-function 的输出
function sys=mdlOutputs(t,x,u,CtrlPara)
fe1 = fal(u(1),CtrlPara.NLSEF.a1,CtrlPara.NLSEF.delta);
fe2 = fal(u(2),CtrlPara.NLSEF.a2,CtrlPara.NLSEF.delta);
u0 = CtrlPara.NLSEF.beta1 * fe1 +  CtrlPara.NLSEF.beta2 * fe2;
sys = u0;

%% coustom function  自定义函数
%  fal 函数
function y = fal(x,a,delta) 
    if abs(x) < delta   
        y = x / (delta^(1-a));
    else
        y = sign(x) * (abs(x)^a); 
    end