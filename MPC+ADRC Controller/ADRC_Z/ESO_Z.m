function [sys,x0,str,ts,simStateCompliance] = ESO(t,x,u,flag,CtrlPara)
%% flag 取值
switch flag,    % flag 默认值为 0
  case 0,       % 初始化
    [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes;  
  case 2,       % 离散状态的更新 
    sys=mdlUpdate(t,x,u,CtrlPara);
  case 3,       % 计算 S-function 的输出 
    sys=mdlOutputs(t,x,u);
  case {1,4,9}  % 模块中不需要的 flag 值 
    sys=[];   
  otherwise     % 错误值处理
    DAStudio.error('Simulink:blocks:unhandledFlag', num2str(flag));
end

%% mdlInitializeSizes  初始化子函数
%  flag = 0 时执行，定义 S-funciton 模块的基本特性
function [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes
sizes = simsizes;           % 调用 simsizes 以装载初始化信息结构
sizes.NumContStates  = 0;   % 连续状态的数量
sizes.NumDiscStates  = 3;   % 离散状态的数量
sizes.NumOutputs     = 3;   % 输出的数量
sizes.NumInputs      = 2;   % 输入的数量
sizes.DirFeedthrough = 0;   % 设置输入端口的信号是否在输出子函数中使用，1 表示有直接联系，即在输出的 sys 中与 u 有直接的联系
sizes.NumSampleTimes = 1;   % 采样时间的数量
sys = simsizes(sizes);      % 再次调用 simsizes ，将 sizes 结构中的信息传递给 sys，sys 是一个保持 Simulink 所用信息的向量
x0  = [0;0;0];                   % 状态变量的初始值
str = [];                   % 保留变量
ts  = [-1 0];                % 采样时间
simStateCompliance = 'UnknownSimState';  % 指定仿真状态的保存和创建方法

%% mdlUpdate  离散状态子函数
%  flag = 2 时执行，更新离散状态、采样时间、主步长等必需条件
function sys=mdlUpdate(t,x,u,CtrlPara)
e1 = x(1) - u(2);
sys(1) = x(1) + CtrlPara.T * (x(2) - CtrlPara.ESO.beta01 * e1);
sys(2) = x(2) + CtrlPara.T * (x(3) - CtrlPara.ESO.beta02 * fal(e1,1/2,CtrlPara.ESO.delta) + CtrlPara.b * u(1));
sys(3) = x(3) - CtrlPara.T * CtrlPara.ESO.beta03 * fal(e1,1/4,CtrlPara.ESO.delta);

%% mdlOutputs  输出子函数
%  flag = 3 时执行，计算 S-function 的输出
function sys=mdlOutputs(t,x,u)
sys = x;

%% coustom function  自定义函数
%  fal 函数
function y = fal(x,a,delta) 
    if abs(x) < delta   
        y = x / (delta^(1-a));
    else
        y = sign(x) * (abs(x)^a); 
    end
    
%% end