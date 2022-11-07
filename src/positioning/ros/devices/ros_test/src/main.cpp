#include "ros/ros.h"
#include <thread>
#include "ros_test.h"

bool threadTerminating = false;

void threadFunction(void)
{
   try
   {
       ros_test();
   }
   catch (std::runtime_error e)
   {
       std::cout << "Caught exception: " << e.what() << std::endl;
   }
   catch (...)
   {
       std::cout << "Caught unknown exception, terminating the program." << std::endl;
   }
    threadTerminating = true;
    ros::shutdown();
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ros_test");
    ros::NodeHandlePtr MLROSNodePtr = ros::NodeHandlePtr(new ros::NodeHandle);
    std::thread threadObj(threadFunction);

    ros::spin();
    if (threadTerminating) {
    threadObj.join();
    }

    return 0;
}
