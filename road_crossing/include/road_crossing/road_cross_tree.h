#ifndef ROAD_CROSS_TREE
#define ROAD_CROSS_TREE

#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"


class Road_cross_tree
{
    public:
        Road_cross_tree(std::string filename);
        virtual ~Road_cross_tree(){}

        BT::Tree tree;

        /**
         * @brief Tick the tree once.
         */
        int run_tree();

    private:
        BT::BehaviorTreeFactory factory;
};

#endif
