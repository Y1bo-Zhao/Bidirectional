#include <px4_msgs/msg/offboard_control_mode.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include <px4_msgs/msg/vehicle_command.hpp>
#include <px4_msgs/msg/vehicle_control_mode.hpp>
#include <px4_msgs/msg/actuator_motors.hpp>
#include <px4_msgs/msg/vehicle_odometry.hpp>
#include <px4_msgs/msg/vehicle_attitude_setpoint.hpp>
#include <px4_msgs/msg/vehicle_rates_setpoint.hpp>
#include <px4_msgs/msg/vehicle_status.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <stdint.h>

#include <chrono>
#include <iostream>

#include <Eigen/Dense>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace px4_msgs::msg;


using Eigen::Vector3d;
using Eigen::Vector4d;
using Eigen::VectorXd;
using Eigen::Matrix3d;

std::ofstream output_file("vehicle_position.txt", std::ios::app); // 以追加模式打开文件


class OffboardControl : public rclcpp::Node
{
public:
        OffboardControl() : Node("offboard_control")    
        {
                rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
                auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 5), qos_profile);
                //auto qos_1 = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 10), qos_profile);

                //p_z和v_z是反着的
                subscription_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("/vrpn_mocap/AIMS_YIBO/pose", qos,
                [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg){
                        // 提取消息数据并更新 current_state
                        position_ << msg->pose.position.x, msg->pose.position.y, msg->pose.position.z;
                        quaternion_ << msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z;
                        //Eigen::Vector3d position(msg->pose.position.x, msg->pose.position.y, -msg->pose.position.z);
                        //Eigen::Vector4d quaternion(msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.y, -msg->pose.orientation.z);
                });
                //如果在架子上被遮挡，可以先用自己的姿态，但速度和角速度必须要vicon
                subscription_1 = this->create_subscription<geometry_msgs::msg::TwistStamped>("/vrpn_mocap/AIMS_YIBO/twist", qos,
                [this](const geometry_msgs::msg::TwistStamped::SharedPtr msg){
                        // 提取消息数据并更新 current_state
                        velocity_ << msg->twist.linear.x, msg->twist.linear.y, msg->twist.linear.z;
                        angular_velocity_ << msg->twist.angular.x, msg->twist.angular.y, msg->twist.angular.z;
                        //Eigen::Vector3d velocity(msg->velocity[0], msg->velocity[1], -msg->velocity[2]);
                        //Eigen::Vector3d angular_velocity(msg->angular_velocity[0], msg->angular_velocity[1], -msg->angular_velocity[2]);
                });

                vehicle_status_sub_ = this->create_subscription<px4_msgs::msg::VehicleStatus>("/fmu/out/vehicle_status", qos,
                [this](const px4_msgs::msg::VehicleStatus::SharedPtr msg) {
                        arming_state_ = msg->arming_state;
                        std::cout << "Received VehicleStatus: arming_state = " << static_cast<int>(arming_state_) 
                                  << ", timestamp = " << msg->timestamp << std::endl;
                });
                

                offboard_control_mode_publisher_ = this->create_publisher<OffboardControlMode>("/fmu/in/offboard_control_mode", 10);
                actuator_motors_publisher_ = this->create_publisher<ActuatorMotors>("/fmu/in/actuator_motors", 10);
                trajectory_setpoint_publisher_ = this->create_publisher<TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 10);
                attitude_publisher_ = this->create_publisher<VehicleAttitudeSetpoint>("/fmu/in/vehicle_attitude_setpoint", 10);
                vehicle_command_publisher_ = this->create_publisher<VehicleCommand>("/fmu/in/vehicle_command", 10);
                attitude_setpoint_publisher_ = this->create_publisher<VehicleRatesSetpoint>("/fmu/in/vehicle_rates_setpoint", 10);

                offboard_setpoint_counter_ = 0;
                K_ = generateK();
                model_param_ = initializeModelParam();
                desired_state_= generateReferencePath();
                
                auto timer_callback = [this]() -> void {
                        std::cout << "Timer callback triggered" << std::endl;
                        current_state_ = initializeCurrentState(position_, quaternion_, velocity_, angular_velocity_);//只有整理成一个向量没有变换
                        //std::cout << "xiba:" << current_state_ << std::endl;
                        // 将位置信息写入文件
                        /*if (output_file.is_open()) {
                            output_file << "Position: " 
                                        << current_state_(0) << ", " 
                                        << current_state_(1) << ", " 
                                        << current_state_(2) << std::endl;
                                        //std::cout << "1" << std::endl;
                        }*/

                        if (offboard_setpoint_counter_ == 10) {
                                // Change to Offboard mode after 10 setpoints
                                this->publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);

                                // Arm the vehicle
                                this->arm();
                        }

                        if (arming_state_ != 2) { // 2 = ARMED
                                this->publish_vehicle_command(VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);
                                publish_offboard_control_mode();
                                this->arm();  // 发送解锁指令
                                std::cout << "Trying to arm... Current state: " << static_cast<int>(arming_state_) << std::endl;
                                return;  // **不往下执行**
                        }

                        if (offboard_setpoint_counter_ >= 10){

                                if (!has_taken_off_) {
                                    // 检查当前高度是否达到5m，挂着测试时可以小于0,省的改代码结构
                                    if (current_state_(2) < -1) { // current_state_(2) 表示高度
                                        // 发布上升控制命令
                                        publish_offboard_control_mode_position();
                                        //publish_attitude_setpoint();
                                        publish_trajectory_setpoint();
                                        
                                    } else {
                                        // 达到5m，标记已升空
                                        has_taken_off_ = true;
                                        std::cout << "take_off is finished" << std::endl;
                                    }
                                }else{
                                        if((offboard_setpoint_counter_-10) < desired_state_.size() && offboard_setpoint_counter_ >= 10 ){
                                                // 提取当前时间步对应的 desired_state 中的向量
                                                //std::cout << "count:" << offboard_setpoint_counter_ << std::endl;
                                                
                                                Eigen::VectorXd current_desired_state = desired_state_[offboard_setpoint_counter_-10];
                                                //std::cout << "size:" << current_desired_state.size() << std::endl;
                                                // 创建一个新的 Eigen::VectorXd 来存放前四个元素和最后一个元素
                                                Eigen::VectorXd reduced_desired_state(4);

                                                reduced_desired_state = current_desired_state.segment<15>(0);  // 前四个元素
                                                Eta = current_desired_state(current_desired_state.size() - 1);  // 最后一个元素
                                                //std::cout << "size1:" << current_state_.size() <<","<<reduced_desired_state.size() <<","<<model_param_.size()<<","<<K_.size()<< std::endl;
                                                
                                                // 调用控制器函数
                                                std::cout << "desired:" << current_desired_state << std::endl;
                                                std::cout << "current:" << current_state_ << std::endl;
                                                control_output = controller(current_state_, reduced_desired_state, model_param_, K_, Eta);

                                                std::cout << "out:" << control_output << std::endl;

                                                double u1 = control_output(0);
                                                double u2 = control_output(1);
                                                double u3 = control_output(2);
                                                double u4 = control_output(3);
                                                u1 = limitValue(u1, -20, 20);
                                                u2 = limitValue(u2, -0.1, 0.1);
                                                u3 = limitValue(u3, -0.1, 0.1);
                                                u4 = limitValue(u4, -0.1, 0.1);
                                                control_output << u1, u2, u3, u4; 
                                                std::cout << "limit_out:" << control_output << std::endl;

                                                F = control_output(0);
                                                roll_rate_dot = control_output(1) / model_param_(0)*0.01;
                                                pitch_rate_dot = control_output(2) / model_param_(1)*0.01;
                                                yaw_rate_dot = control_output(3) / model_param_(2)*0.01;
                                                control_output << F,roll_rate_dot,pitch_rate_dot,yaw_rate_dot;
                                                std::cout << "acceleration:" << control_output << std::endl;
                                                roll_rate_dot = limitValue(roll_rate_dot, -0.1, 0.1);
                                                pitch_rate_dot = limitValue(pitch_rate_dot, -0.1, 0.1);
                                                yaw_rate_dot = limitValue(yaw_rate_dot, -0.1, 0.1);
                                                control_output << F,roll_rate_dot,pitch_rate_dot,yaw_rate_dot;
                                                std::cout << "acceleration_limit:" << control_output << std::endl;

                                                roll_rate_ += roll_rate_dot;
                                                pitch_rate_ += pitch_rate_dot;
                                                yaw_rate_ += yaw_rate_dot;
                                                roll_rate_ = limitValue(roll_rate_, -0.3, 0.3);
                                                pitch_rate_ = limitValue(pitch_rate_, -0.3, 0.3);
                                                yaw_rate_ = limitValue(yaw_rate_, -0.3, 0.3);

                                                //转速转换内置在publishActuatorMotorsMsg里了
                                                //motor_speeds = calculateMotorSpeeds(control_output);
                                                // 发布控制模式
                                                publish_offboard_control_mode();
                                                
                                                //Eigen::VectorXd control_output(4);
                                                //control_output << 10.0, 0.0, 0.0, 0.0; 
                                                
                                                //publishActuatorMotorsMsg(control_output);
                                                //下：角速率控制，上：电机控制 默认-0.027
                                                control_output << F*(-0.027), -roll_rate_, pitch_rate_, -yaw_rate_; 
                                                std::cout << "final_out:" << control_output << std::endl;
                                                publish_body_rate_setpoint(control_output);
                                        }
                                        else
                                        {
                                                std::cout << "Time is over!" << std::endl;
                                        }
                                }
                        }
                        // 计数器增加
                        offboard_setpoint_counter_++;
                };
                timer_ = this->create_wall_timer(10ms, timer_callback);
        }

        void arm();
        void disarm();

private:
        Eigen::Vector3d position_;
        Eigen::Vector4d quaternion_;
        Eigen::Vector3d velocity_;
        Eigen::Vector3d angular_velocity_;


        rclcpp::TimerBase::SharedPtr timer_;

        uint8_t arming_state_ = 1; // 初始值为 DISARMED
        rclcpp::Subscription<px4_msgs::msg::VehicleStatus>::SharedPtr vehicle_status_sub_;
        rclcpp::Publisher<OffboardControlMode>::SharedPtr offboard_control_mode_publisher_;
        rclcpp::Publisher<TrajectorySetpoint>::SharedPtr trajectory_setpoint_publisher_;
        rclcpp::Publisher<VehicleAttitudeSetpoint>::SharedPtr attitude_publisher_;
        rclcpp::Publisher<px4_msgs::msg::ActuatorMotors>::SharedPtr actuator_motors_publisher_;
        rclcpp::Publisher<VehicleCommand>::SharedPtr vehicle_command_publisher_;
        rclcpp::Publisher<VehicleRatesSetpoint>::SharedPtr attitude_setpoint_publisher_;

        double limitValue(double value, double min_value, double max_value);

        
        Eigen::VectorXd initializeCurrentState(const Eigen::Vector3d& position,const Eigen::Vector4d& quaternion,const Eigen::Vector3d& velocity,const Eigen::Vector3d& angular_velocity); 
        Eigen::VectorXd initializeModelParam();
        Eigen::VectorXd generateK();
        std::vector<Eigen::VectorXd> generateReferencePath();//生成desired_state
        Eigen::VectorXd calculateMotorSpeeds(const Eigen::VectorXd& control_output);
        std::atomic<uint64_t> timestamp_;   //!< common synced timestamped

        uint64_t offboard_setpoint_counter_;   //!< counter for the number of setpoints sent

        bool has_taken_off_ = false;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_;
        rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subscription_1;
        Eigen::VectorXd current_state_;
        std::vector<Eigen::VectorXd> desired_state_;
        Eigen::VectorXd model_param_;
        Eigen::VectorXd K_;
        Eigen::VectorXd control_output;
        double F;
        double roll_rate_dot;
        double pitch_rate_dot;
        double yaw_rate_dot;
        double roll_rate_ = 0, pitch_rate_ = 0, yaw_rate_ = 0; // 角速率
        double dt_ = 0.02; // 时间步长，单位：秒

        int Eta;
        void publish_offboard_control_mode_position();
        void publish_offboard_control_mode();
        void publish_trajectory_setpoint();
        void publish_attitude_setpoint();
        void publishActuatorMotorsMsg(const Eigen::VectorXd& control_output);
        void publish_body_rate_setpoint(const Eigen::VectorXd& control_output);
        void publish_vehicle_command(uint16_t command, float param1 = 0.0, float param2 = 0.0);
        Eigen::VectorXd controller(const Eigen::VectorXd& current_state, const Eigen::VectorXd& desired_state, const Eigen::VectorXd& model_param, const Eigen::VectorXd& K, int Eta);
        Eigen::Matrix3d qToR(double qw, double qx, double qy, double qz);
        Eigen::Vector4d quatmultiply(const Eigen::Vector4d& q1, const Eigen::Vector4d& q2);
        Eigen::Vector4d q_abc(double a, double b, double c);
        Eigen::Vector4d q_abc_1(double a, double b, double c);
        Eigen::Vector4d q_psi(double psi);
        Eigen::Vector3d vee_optr(const Eigen::Matrix3d& R);
        char switchMode(const Eigen::Vector3d& b3);
         
};

/**
 * @brief Send a command to Arm the vehicle
 */
void OffboardControl::arm()
{
        publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0);

        RCLCPP_INFO(this->get_logger(), "Arm command send");
}

/**
 * @brief Send a command to Disarm the vehicle
 */
void OffboardControl::disarm()
{
        publish_vehicle_command(VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0.0);

        RCLCPP_INFO(this->get_logger(), "Disarm command send");
}

/**
 * @brief Publish the offboard control mode.
 *        For this example, only position and altitude controls are active.
 */
void OffboardControl::publish_offboard_control_mode()
{
        OffboardControlMode msg{};
        msg.position = false;
        msg.velocity = false;
        msg.acceleration = false;
        msg.attitude = false;
        msg.body_rate = true;
        msg.thrust_and_torque = false;
        msg.direct_actuator = false;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        offboard_control_mode_publisher_->publish(msg);
}

void OffboardControl::publish_offboard_control_mode_position()
{
        OffboardControlMode msg{};
        msg.position = false;
        msg.velocity = false;
        msg.acceleration = false;
        msg.attitude = true;
        msg.body_rate = false;
        msg.thrust_and_torque = false;
        msg.direct_actuator = false;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        offboard_control_mode_publisher_->publish(msg);
}
/**
 * @brief Publish a trajectory setpoint
 *        For this example, it sends a trajectory setpoint to make the
 *        vehicle hover at 5 meters with a yaw angle of 180 degrees.
 */
void OffboardControl::publish_trajectory_setpoint()
{
        TrajectorySetpoint msg{};
        msg.position = {0.0, 0.0, -1.2};
        msg.yaw = 1.5708; // [-PI:PI]
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        trajectory_setpoint_publisher_->publish(msg);
        
}

void OffboardControl::publish_attitude_setpoint()
{
        auto msg = px4_msgs::msg::VehicleAttitudeSetpoint();

        // 设置时间戳（单位：微秒）
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;

        msg.yaw_sp_move_rate = 0.0f;



        // 设置目标四元数 (q_d)
        msg.q_d = {1.0, 0.0, 0.0, 0.0}; // 示例：无旋转

        // 设置推力（单位：[-1, 1]）
        msg.thrust_body = {0.0, 0.0, -0.2};  // 推力为50%

        // 发布消息
        attitude_publisher_->publish(msg);
}

void OffboardControl::publishActuatorMotorsMsg(const Eigen::VectorXd& control_output) {
    Eigen::Vector4d motor_speeds = calculateMotorSpeeds(control_output);

    std::cout << "motor_speed:" << motor_speeds << std::endl;
    px4_msgs::msg::ActuatorMotors actuator_motors_msg;
    //actuator_motors_msg.control.resize(4);
    
    for (size_t i = 0; i < 4; ++i) {
        actuator_motors_msg.control[i] = motor_speeds[i] / 1000.0; // 转速比例，最大值为 1
    }
    actuator_motors_msg.reversible_flags = 0b1111;
    actuator_motors_msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    actuator_motors_msg.timestamp_sample = actuator_motors_msg.timestamp;
    
    actuator_motors_publisher_->publish(actuator_motors_msg);
}

void OffboardControl::publish_body_rate_setpoint(const Eigen::VectorXd& control_output) {
        
    auto msg = VehicleRatesSetpoint();

    // Set timestamp (in microseconds)
    msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;

    // Set desired body rates (rad/s)
    msg.roll = control_output(1);  // Example roll rate
    msg.pitch = control_output(2); // Example pitch rate
    msg.yaw = control_output(3);   // Example yaw rate

    // Set thrust (normalized, [-1, 1])
    msg.thrust_body = {0.0, 0.0, control_output(0)}; // Example thrust (50% downward)
    //std::cout << "xiba" << std::endl;
    attitude_setpoint_publisher_->publish(msg);
}


/*
void OffboardControl::publishActuatorMotorsMsg()
{
    // Lockstep should be disabled from PX4 and from the model.sdf file
    // direct motor throttles control
    // Prepare msg
    // 直接电机推力控制
    px4_msgs::msg::ActuatorMotors actuator_motors_msg;
    
    // 设置推力值，默认四个电机的推力为 0.1，其余通道设为 NaN
    actuator_motors_msg.control = {0.75f, 0.75f, 0.75f, 0.75f, 
                                   std::nanf("1"), std::nanf("1"), std::nanf("1"), std::nanf("1"),
                                   std::nanf("1"), std::nanf("1"), std::nanf("1"), std::nanf("1")};
    
    actuator_motors_msg.reversible_flags = 0b1111; // 如果电机不需要反向推力，可以保持0
    actuator_motors_msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
    actuator_motors_msg.timestamp_sample = actuator_motors_msg.timestamp;

    // 发布消息
    actuator_motors_publisher_->publish(actuator_motors_msg);

    //RCLCPP_INFO(this->get_logger(), "Published actuator motors message with 0.1 thrust for each motor");
}
*/


/**
 * @brief Publish vehicle commands
 * @param command   Command code (matches VehicleCommand and MAVLink MAV_CMD codes)
 * @param param1    Command parameter 1
 * @param param2    Command parameter 2
 */
void OffboardControl::publish_vehicle_command(uint16_t command, float param1, float param2)
{
        VehicleCommand msg{};
        msg.param1 = param1;
        msg.param2 = param2;
        msg.command = command;
        msg.target_system = 1;
        msg.target_component = 1;
        msg.source_system = 1;
        msg.source_component = 1;
        msg.from_external = true;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        vehicle_command_publisher_->publish(msg);
}


Eigen::VectorXd OffboardControl::initializeModelParam()
{       
        Eigen::VectorXd model_param(5);
        model_param(0)=0.0036; //I_x
        model_param(1)=0.0032; //I_y
        model_param(2)=0.00525;   //I_z
        model_param(3)=0.878;      //m（确认一下是哪个电池下的重量）
        model_param(4)=9.81;   //g
        return model_param;
}

Eigen::VectorXd OffboardControl::initializeCurrentState(const Eigen::Vector3d& position,const Eigen::Vector4d& quaternion,const Eigen::Vector3d& velocity,const Eigen::Vector3d& angular_velocity) 
{
        Eigen::VectorXd current_state(13);

        current_state.segment<3>(0) = position;
        current_state.segment<3>(3) = velocity;
        current_state.segment<4>(6) = quaternion;
        current_state.segment<3>(10) = angular_velocity;

        return current_state;
}

Eigen::VectorXd OffboardControl::generateK() 
{
        // 定义每个矩阵的对角线元素
        Eigen::Vector3d Kp_diag(1.0, 1.0, 1.0);       // Kp 的对角线元素1
        Eigen::Vector3d Kv_diag(0.5, 0.5, 0.5);       // Kv 的对角线元素0.5
        Eigen::Vector3d KR_diag(0.4, 0.4, 0.4);       // KR 的对角线元素0.3
        Eigen::Vector3d K_omega_diag(0.2, 0.2, 0.2);  // K_omega 的对角线元素0.1

        // 创建长度为 12 的向量 K
        Eigen::VectorXd K(12);

        // 按顺序填充 K
        K.segment<3>(0) = Kp_diag;       // Kp 的对角线元素
        K.segment<3>(3) = Kv_diag;       // Kv 的对角线元素
        K.segment<3>(6) = KR_diag;       // KR 的对角线元素
        K.segment<3>(9) = K_omega_diag;  // K_omega 的对角线元素

        return K;
}

Eigen::VectorXd OffboardControl::controller(const Eigen::VectorXd& current_state, const Eigen::VectorXd& desired_state, const Eigen::VectorXd& model_param, const Eigen::VectorXd& K, int Eta)
{
        Matrix3d J = model_param.segment<3>(0).asDiagonal(); // Assuming model_param stores inertia in diagonal form
        // Position error
        Vector3d ep = current_state.segment<3>(0) - desired_state.segment<3>(0); // position - desired position
        // Velocity error
        Vector3d ev = current_state.segment<3>(3) - desired_state.segment<3>(3); // velocity - desired velocity
        // 提取 K
        Matrix3d Kp = K.segment<3>(0).asDiagonal(); // Assuming K contains Kp, Kv, KR, and K_omega in order
        Matrix3d Kv = K.segment<3>(3).asDiagonal();
        Matrix3d KR = K.segment<3>(6).asDiagonal();
        Matrix3d K_omega = K.segment<3>(9).asDiagonal();
        double m = model_param(3);
        double g = model_param(4);
        // Desired acceleration and force
        Vector3d acc = -Kp * ep - Kv * ev + g * Vector3d(0, 0, 1) + desired_state.segment<3>(6); // Assuming acc at index 6-8 in desired_state
        Vector3d Fd = m * acc;
        std::cout << "Fd:" << Fd << std::endl;
        // Current quaternion
        double qw = current_state(6), qx = current_state(7), qy = current_state(8), qz = current_state(9);
        Matrix3d R = qToR(qw, qx, qy, qz);
         // Current z-axis in body frame
        Vector3d zb = R.col(2);
        
        double u1 = Fd.dot(zb);
        // Compute current acceleration
        Vector3d current_acc = -g * Vector3d(0, 0, 1) + u1 * zb;
        // Acceleration error
        Vector3d ea = current_acc - desired_state.segment<3>(6);
        std::cout << "ea:" << ea << std::endl;
        // Desired force derivative
        Vector3d acc_dot = -Kp * ev - Kv * ea + desired_state.segment<3>(9); // Assuming jerk at index 9-11 in desired_state
        Vector3d Fd_dot = m * acc_dot;
        // Calculate desired rotation matrix
        Vector3d zbd = acc.normalized();
        double a = zbd(0), b = zbd(1), c = zbd(2);
        if (Eta == 1)
        {
                zbd = acc.normalized();
                a = zbd(0), b = zbd(1), c = zbd(2);
        }
        if (Eta == -1)
        {
                zbd = -acc.normalized();
                a = zbd(0), b = zbd(1), c = zbd(2);
        }
        Eigen::Vector3d vec(a, b, c);
        char Mode = switchMode(vec);
        // Derivative of zbd
        //double Fd_norm_dot = Fd.transpose() * Fd_dot / Fd.norm();
        //Vector3d zbd_dot = (Fd_dot * Fd.norm() - Fd * Fd_norm_dot) / (Fd.norm() * Fd.norm());
        // Assuming acc and acc_dot are both Vector3d
        double acc_norm = acc.norm(); // Compute the norm of acc
        Matrix3d I = Matrix3d::Identity(); // Identity matrix

        // Compute the matrix term: (acc'*acc * I - acc * acc.transpose())
        Matrix3d matrix_term = (acc.dot(acc)) * I - acc * acc.transpose();

        // Calculate zbd_dot using the updated formula
        Vector3d zbd_dot = (matrix_term * acc_dot) / pow(acc_norm, 3);
        double a_dot = zbd_dot(0), b_dot = zbd_dot(1), c_dot = zbd_dot(2);
        if (Eta == 1)
        {
                zbd_dot = (matrix_term * acc_dot) / pow(acc_norm, 3);
                a_dot = zbd_dot(0), b_dot = zbd_dot(1), c_dot = zbd_dot(2);
        }
        if (Eta == -1)
        {
                zbd_dot = -(matrix_term * acc_dot) / pow(acc_norm, 3);
                a_dot = zbd_dot(0), b_dot = zbd_dot(1), c_dot = zbd_dot(2);
        }

        //模式N
        double yawd = desired_state(12);
        double yawd_dot = desired_state(13);
        double yawd_2dot = desired_state(14);
        //模式S
        double yawd_1 = 2 * std::atan2(a, b) + yawd;
        double yawd_1_dot;
        if (a == 0 && b == 0) {
            yawd_1_dot = 0; // 或者定义一个其他合理的值
        } else {
            yawd_1_dot = 2 * (a_dot * b - a * b_dot) / (a * a + b * b);
        }





        // Initialize outputs
        double u2 = 0, u3 = 0, u4 = 0;

        if (Mode == 'N') {
                Vector4d q_1 = q_abc(a, b, c);
                Vector4d q_2 = q_psi(yawd); // Assuming yaw at index 12 in desired_state
                Vector4d q_3 = quatmultiply(q_1, q_2);
                Matrix3d Rd = qToR(q_3(0), q_3(1), q_3(2), q_3(3));
                std::cout << "Rd:" << Rd << std::endl;
                std::cout << "R:" << R << std::endl;
                Matrix3d R1 = Rd.transpose() * R - R.transpose() * Rd;
                Vector3d eR = 0.5 * vee_optr(R1);
                std::cout << "eR:" << eR << std::endl;

                Vector3d current_omega = current_state.segment<3>(10);
                Vector3d wd;
                wd << sin(yawd) * a_dot - cos(yawd) * b_dot - (a * sin(yawd) - b * cos(yawd)) * (c_dot / (c + 1)),
                      cos(yawd) * a_dot + sin(yawd) * b_dot - (a * cos(yawd) + b * sin(yawd)) * (c_dot / (c + 1)),
                      (b * a_dot - a * b_dot) / (1 + c) + yawd_dot;

                Vector3d ew = current_omega - R.transpose() * Rd * wd;
                Vector3d M = -KR * eR - K_omega * ew;
                std::cout << "ew:" << ew << std::endl;
                u2 = M(0); u3 = M(1); u4 = M(2);
        } else if (Mode == 'S') {
                Eigen::Vector4d q_1 = q_abc_1(a, b, c);
                Eigen::Vector4d q_2 = q_psi(yawd);
                Eigen::Vector4d q_3 = quatmultiply(q_1, q_2);
                Eigen::Matrix3d Rd = qToR(q_3(0), q_3(1), q_3(2), q_3(3));
                Eigen::Matrix3d R1 = Rd.transpose() * R - R.transpose() * Rd;
                Eigen::Vector4d q = {qw, qx, qy, qz};
                Vector3d eR = 0.5 * vee_optr(R1);
                //为了debug暂时顶替一下，后面eR的算法还要用下面这个
                //Eigen::Vector3d eR = calculateAttitudeError(q, q_3);

                // 7. 计算期望角速度 wd
                Eigen::Vector3d wd;
                wd << std::sin(yawd_1) * a_dot + std::cos(yawd_1) * b_dot - (a * std::sin(yawd_1) + b * std::cos(yawd_1)) * (c_dot / (c - 1)),
                      std::cos(yawd_1) * a_dot - std::sin(yawd_1) * b_dot - (a * std::cos(yawd_1) - b * std::sin(yawd_1)) * (c_dot / (c - 1)),
                      (b * a_dot - a * b_dot) / (-1 + c) + yawd_1_dot;

                // 8. 计算角速度误差 ew
                Eigen::Vector3d current_omega = current_state.segment<3>(10);
                Eigen::Vector3d ew = current_omega - R.transpose() * Rd * wd;

                // 9. 计算控制矩 M
                Eigen::Vector3d M = -KR * eR - K_omega * ew;

                // 10. 将控制矩的结果赋值给 u2, u3, u4
                double u2 = M(0);
                double u3 = M(1);
                double u4 = M(2);
        // Repeat the logic for Mode 'S'
        // The implementation is similar to 'N', but with different calculations for yawd_1 and related variables
        }
        Eigen::VectorXd control_output(4);
        control_output << u1, u2, u3, u4;
        return control_output;


}

double OffboardControl::limitValue(double value, double min_value, double max_value)
{
    if (value < min_value) {
        return min_value;
    } else if (value > max_value) {
        return max_value;
    } else {
        return value;
    }
}

Eigen::Matrix3d OffboardControl::qToR(double qw, double qx, double qy, double qz) {
    Eigen::Matrix3d R;

    // 计算旋转矩阵的每个元素
    R(0, 0) = 1 - 2 * (qy * qy + qz * qz);
    R(0, 1) = 2 * (qx * qy - qz * qw);
    R(0, 2) = 2 * (qx * qz + qy * qw);
    R(1, 0) = 2 * (qx * qy + qz * qw);
    R(1, 1) = 1 - 2 * (qx * qx + qz * qz);
    R(1, 2) = 2 * (qy * qz - qx * qw);
    R(2, 0) = 2 * (qx * qz - qy * qw);
    R(2, 1) = 2 * (qy * qz + qx * qw);
    R(2, 2) = 1 - 2 * (qx * qx + qy * qy);

    return R;
}

// 定义四元数乘法函数
Eigen::Vector4d OffboardControl::quatmultiply(const Eigen::Vector4d& q1, const Eigen::Vector4d& q2) {
    Eigen::Vector4d q3;
    
    double w1 = q1(0), x1 = q1(1), y1 = q1(2), z1 = q1(3);
    double w2 = q2(0), x2 = q2(1), y2 = q2(2), z2 = q2(3);
    
    // 计算四元数乘法结果
    q3(0) = w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2;
    q3(1) = w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2;
    q3(2) = w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2;
    q3(3) = w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2;

    return q3;
}

Eigen::Vector4d OffboardControl::q_abc(double a, double b, double c) {
    Eigen::Vector4d q;

    // 计算四元数的各个分量
    q(0) = (1 + c) / std::sqrt(2 * (1 + c)); // w 分量
    q(1) = -b / std::sqrt(2 * (1 + c));      // x 分量
    q(2) = a / std::sqrt(2 * (1 + c));       // y 分量
    q(3) = 0.0;                              // z 分量

    return q;
}

Eigen::Vector4d OffboardControl::q_psi(double psi) {
    Eigen::Vector4d q;

    // 计算四元数的各个分量
    q(0) = std::cos(psi / 2); // w 分量
    q(1) = 0.0;               // x 分量
    q(2) = 0.0;               // y 分量
    q(3) = std::sin(psi / 2); // z 分量

    return q;
}


Eigen::Vector4d OffboardControl::q_abc_1(double a, double b, double c) {
    Eigen::Vector4d q;

    // 计算四元数的各个分量
    q(0) = (-b) / std::sqrt(2 * (1 - c)); // w 分量
    q(1) = (1-c) / std::sqrt(2 * (1 - c));      // x 分量
    q(2) = 0;       // y 分量
    q(3) = a / std::sqrt(2 * (1 - c));                              // z 分量

    return q;
}

// 将反对称矩阵恢复为其对应的向量
Eigen::Vector3d OffboardControl::vee_optr(const Eigen::Matrix3d& R) {
    Eigen::Vector3d w;

    // 从反对称矩阵提取向量分量
    w << -R(1, 2),  // -R(2, 3) 对应的是 -R(1, 2) 因为 Eigen 是从 0 开始索引
         R(0, 2),   // R(1, 3)
        -R(0, 1);   // -R(1, 2)

    return w;
}

// C++ 版本的 switchMode 函数
char OffboardControl::switchMode(const Eigen::Vector3d& b3) {
    // 静态变量，用于保存上一次的模式和点积
    static char prev_mode = 'N'; // 初始模式为 'N'
    static double prev_dot_product = 1.0; // 初始点积为 1.0

    // 设置 alpha 的值
    double alpha = 0.5; // 可以根据需要调整

    // 计算 b3 与 (0, 0, 1) 的点积，即 b3 的 z 分量
    double dot_product = b3(2); // b3 与 (0, 0, 1) 的点积等于 b3 的 z 分量

    // 根据点积的变化情况切换模式
    char mode;
    if (prev_dot_product > -alpha && dot_product < -alpha) {
        mode = 'S';
    } else if (prev_dot_product < alpha && dot_product > alpha) {
        mode = 'N';
    } else {
        mode = prev_mode; // 保持前一个模式
    }

    // 更新静态变量
    prev_mode = mode;
    prev_dot_product = dot_product;

    return mode;
}

Eigen::VectorXd OffboardControl::calculateMotorSpeeds(const Eigen::VectorXd& control_output) 
{
        double u1 = control_output(0);
        double u2 = control_output(1);
        double u3 = control_output(2);
        double u4 = control_output(3);
        // 参数：电机距离中心的距离和扭矩系数
        double d = 0.246;  // 电机到中心的距离 (m)
        double c = 0.016; // 扭矩系数
        // 电机的推力常数 (N·m/(rad/s)^2)
        double motor_constant = 8.54858e-06;




        Eigen::Matrix4d allocation_matrix;
        // 四旋翼的力矩和推力分配矩阵
        allocation_matrix <<  1,  1,  1,  1,
                          0,  d,  0, -d,
                         -d,  0,  d,  0,
                          c/motor_constant, -c/motor_constant,  c/motor_constant, -c/motor_constant;

        Eigen::Vector4d outs;
        outs << u1, u2, u3, u4;

        // 计算每个电机的推力
        Eigen::Vector4d motor_forces = allocation_matrix.inverse() * outs;

        // 转换为电机转速 (rad/s)
        Eigen::Vector4d motor_speeds;
        for (int i = 0; i < 4; ++i) {
                //motor_speeds[i] = std::sqrt(motor_forces[i] / motor_constant);
                motor_speeds[i] = (motor_forces[i] < 0 ? -1 : 1) * std::sqrt(std::abs(motor_forces[i]) / motor_constant);

        }

        return motor_speeds;
}


std::vector<Eigen::VectorXd> OffboardControl::generateReferencePath() 
{
        int zero_length = 10000;
        int steps_to_target = 100;
        double x_target = -1.288;
        double y_target = -1.441;
        double z_target = 0.926;
        double delta_t = 0.1;
        // 定义时间间隔
        double time_step = delta_t;
        Eigen::VectorXd time_vector = Eigen::VectorXd::LinSpaced(zero_length, 0, (zero_length - 1) * time_step);

        // 初始化矩阵，包含 x, y, z, yaw, eta
        Eigen::MatrixXd matrix(zero_length, 5);
        matrix.setZero(); // 初始化为 0

        // 设置 x, y, z 在前 steps_to_target 步内逐渐增加到目标值
        for (int i = 0; i < steps_to_target; ++i) {
        double t = static_cast<double>(i) / steps_to_target;
        //matrix(i, 0) = t * x_target;
        //matrix(i, 1) = t * y_target;
        //matrix(i, 2) = t * z_target;
        matrix(i, 0) = x_target;
        matrix(i, 1) = y_target;
        matrix(i, 2) = z_target;
        }

        // 设置 x, y, z 在 steps_to_target 之后保持目标值不变
        for (int i = steps_to_target; i < zero_length; ++i) {
        matrix(i, 0) = x_target;
        matrix(i, 1) = y_target;
        matrix(i, 2) = z_target;
        }

        // 设置 yaw 和 eta
        matrix.col(3).setConstant(0); // yaw 始终为 0
        matrix.col(4).setConstant(1.0); // eta 始终为 1

        
        std::ofstream file("desired_state_output.txt");
        
        // 生成参考路径
        std::vector<Eigen::VectorXd> referencePath(zero_length);

        for (int i = 0; i < zero_length; ++i) {
                //多一维给Eta
                Eigen::VectorXd desired_state(16);
                desired_state.setZero(); // 初始化为 0

                // 设置位置部分
                desired_state.segment<3>(0) = matrix.row(i).head<3>();

                // 计算速度（中心差分法）
                if (i > 0 && i < zero_length - 1) {
                    desired_state.segment<3>(3) = (matrix.row(i + 1).head<3>() - matrix.row(i - 1).head<3>()) / (2 * delta_t);
                } else if (i == 0) {
                    desired_state.segment<3>(3) = (matrix.row(i + 1).head<3>() - matrix.row(i).head<3>()) / delta_t;
                } else {
                    desired_state.segment<3>(3) = (matrix.row(i).head<3>() - matrix.row(i - 1).head<3>()) / delta_t;
                }

                // 计算加速度（速度的变化率）
                if (i > 0) {
                        desired_state.segment<3>(6) = (desired_state.segment<3>(3) - referencePath[i - 1].segment<3>(3)) / delta_t;
                } else {
                        desired_state.segment<3>(6).setZero(); // 第一个时间步的加速度设为 0
                }
                
                // 计算 jerk（加速度的变化率）
                if (i > 1) {
                        desired_state.segment<3>(9) = (desired_state.segment<3>(6) - referencePath[i - 1].segment<3>(6)) / delta_t;
                } else {
                        desired_state.segment<3>(9).setZero(); // 前两个时间步的 jerk 设为 0
                }

                // 设置偏航角
                desired_state(12) = matrix(i, 3); // yaw
                //13、14、15：yawd, yawdd, Eta
                // 设置 eta（在 C++ 中，eta 可用于其他逻辑）
                desired_state(15) = matrix(i, 4); 
                // 添加到参考路径中
                referencePath[i] = desired_state;
                // 将 desired_state 输出到文件
                file << desired_state.transpose() << "\n";
        }
        // 关闭文件
        file.close();
        std::cout << "ReferencePath has been generated" << std::endl;
        return referencePath;
}


int main(int argc, char *argv[])
{
        std::cout << "Starting offboard control node..." << std::endl;
        setvbuf(stdout, NULL, _IONBF, BUFSIZ);
        rclcpp::init(argc, argv);
        rclcpp::spin(std::make_shared<OffboardControl>());      
        output_file.close();

        rclcpp::shutdown();
        return 0;
}
