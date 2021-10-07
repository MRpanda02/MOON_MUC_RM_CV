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
    double fps;
    double t = 0;
    char string[10];            // 存放帧率的字符串
    
    //cv::waitKey(0);

    ArmorDetect inst(ArmorDetect::Mode::FIND_BLUE);

    while (true)
    {
        t = (double)cv::getTickCount();
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
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
        fps = 1.0 / t;
        sprintf(string, "%.2f", fps);      
        std::string fpsString("FPS:");
        fpsString += string;      
        putText(show,                   // 图像矩阵
            fpsString,                  // string型文字内容
            cv::Point(5, 20),           // 文字坐标，以左下角为原点
            cv::FONT_HERSHEY_SIMPLEX,   // 字体类型
            0.5, // 字体大小
            cv::Scalar(0, 0, 0));       // 字体颜色
            cv::imshow("show", show);
        if (cv::waitKey(30) == 27) break;
    }
}
