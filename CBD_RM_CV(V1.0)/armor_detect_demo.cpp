#include "MVCameraInput.h"
#include "ArmorDetect.h"

int main(int argc, char *argv[])
{

    cv::namedWindow("frame");
    cv::namedWindow("show");

    #ifdef WINDOWS
    cv::VideoCapture cap(1);
    if(!cap.isOpened())
    {
        std::cout << " can't open camera "<< std::endl;
    }
    #else
        MVCameraInput cap;
    #endif
    cv::Mat frame;
    cap >> frame;
    std::cout << frame.size();
    
    //cv::waitKey(0);

    ArmorDetect inst(ArmorDetect::Mode::FIND_BLUE);

    while (true)
    {
        cv::waitKey(10);
        cap >> frame;
        cv::imshow("frame", frame);

        cv::Mat show;
        frame.copyTo(show);
        
        for (const auto &bar : inst.getLightBars())
        {
            cv::Point2f pts[4];
            bar.box.points(pts);
            drawTetragon(show, pts, cv::Scalar(0, 255, 0));
        }

        auto aimArea = inst.process(frame);
        if (aimArea.empty())
            std::cout << "No target found.\n";
        else
            drawTetragon(show, aimArea.data(), cv::Scalar(255, 255, 255));

        // for (const auto &armor : inst.getResults())
        // {
        //     cv::Point2f pts[4];
        //     armor.points(pts);
        //     drawTetragon(show, pts, cv::Scalar(0, 255, 0));
        // }

        cv::imshow("show", show);
        if (cv::waitKey(30) == 27) break;
    }
}
