% 定义 Vector3 总线对象
elems_4(1) = Simulink.BusElement;
elems_4(1).Name = 'x';
elems_4(1).Dimensions = 1;
elems_4(1).DataType = 'double';

elems_4(2) = Simulink.BusElement;
elems_4(2).Name = 'y';
elems_4(2).Dimensions = 1;
elems_4(2).DataType = 'double';

elems_4(3) = Simulink.BusElement;
elems_4(3).Name = 'z';
elems_4(3).Dimensions = 1;
elems_4(3).DataType = 'double';

Vector3Bus = Simulink.Bus;
Vector3Bus.Elements = elems_4;

% 定义 Wrench 总线对象
elems_5(1) = Simulink.BusElement;
elems_5(1).Name = 'thrust';
elems_5(1).DataType = 'Bus: Vector3Bus';

elems_5(2) = Simulink.BusElement;
elems_5(2).Name = 'torque';
elems_5(2).DataType = 'Bus: Vector3Bus';

WrenchBus = Simulink.Bus;
WrenchBus.Elements = elems_5;

% 创建 MATLAB 结构体
wrenchStruct = Simulink.Bus.createMATLABStruct('WrenchBus');
