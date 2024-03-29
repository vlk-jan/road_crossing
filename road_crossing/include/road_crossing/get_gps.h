#ifndef GET_GPS
#define GET_GPS

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"

#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"


class GPS_nodes
{
    public:
        GPS_nodes(){}
        virtual ~GPS_nodes(){}

        /**
         * @brief Initialize service clients for functions in this class.
         * 
         * @param nh ROS NodeHandle.
         */
        static void init_service(ros::NodeHandle& nh);

        /**
         * @brief Callback for the gps topic subscriber.
         * 
         * @param msg Read message from the gps topic.
         */
        static void callback_gps(const sensor_msgs::NavSatFix::ConstPtr& msg);

        /**
         * @brief Request information from road cost node, if the current robot's position is suitable for road crossing.
         */
        static int place_suitability();

        /**
         * @brief Request the coordinates of latest recorded robot's position.
         */
        static void req_position(double& easting, double& northing);

        /**
         * @brief Condition node that check if the robot is close enough to the road to start crossing. 
         */
        class cross_road : public BT::ConditionNode
        {
            public:
                cross_road(const std::string& name, const BT::NodeConfiguration& config) :
                    BT::ConditionNode(name, config)
                {}

                virtual ~cross_road(){}

                BT::NodeStatus tick() override;

                static BT::PortsList providedPorts();
        };

        /**
         * @brief Condition node returning if the current robot's position is suitable for crossing.
        */
        class place_suitable : public BT::ConditionNode
        {
            public:
                place_suitable(const std::string& name, const BT::NodeConfiguration& config) :
                    BT::ConditionNode(name, config)
                {}

                virtual ~place_suitable(){}

                BT::NodeStatus tick() override;

                static BT::PortsList providedPorts();
        };

        /**
         * @brief Condition node returning if a better place for crossing was found.
         */
        class better_place : public BT::ConditionNode
        {
            public:
                better_place(const std::string& name, const BT::NodeConfiguration& config) :
                    BT::ConditionNode(name, config)
                {}

                virtual ~better_place(){}

                BT::NodeStatus tick() override;

                static BT::PortsList providedPorts();
        };

        /**
         * @brief Action node that saves the coordinates of the latest recorded robot's position to the blackboard.
         */
        class get_position : public BT::SyncActionNode
        {
            public:
                get_position(const std::string& name, const BT::NodeConfiguration& config) :
                    BT::SyncActionNode(name, config)
                {}

                virtual ~get_position(){}

                BT::NodeStatus tick() override;

                static BT::PortsList providedPorts();
        };

        /**
         * @brief Actio node that tries to find a better place for crossing.
        */
        class get_better_place : public BT::SyncActionNode
        {
            public:
                get_better_place(const std::string& name, const BT::NodeConfiguration& config) :
                    BT::SyncActionNode(name, config)
                {}

                virtual ~get_better_place(){}

                BT::NodeStatus tick() override;

                static BT::PortsList providedPorts();
        };
    
    private:
        static ros::ServiceClient place_suitability_client, get_better_place_client;
        static double easting, northing;
        static double better_easting, better_northing;
        static bool is_valid, suitable, new_place;
};

ros::ServiceClient GPS_nodes::place_suitability_client;
ros::ServiceClient GPS_nodes::get_better_place_client;
double GPS_nodes::easting = 0;
double GPS_nodes::northing = 0;
double GPS_nodes::better_easting = 0;
double GPS_nodes::better_northing = 0;
bool GPS_nodes::is_valid = false;
bool GPS_nodes::suitable = false;
bool GPS_nodes::new_place = false;

#endif
